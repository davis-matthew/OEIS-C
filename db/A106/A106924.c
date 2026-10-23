/*
 * A106924  Primes of the form 4x^2 + xy + 4y^2, with x and y any integer.
 * https://oeis.org/A106924
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   Primes of the form 4*x^2 + x*y + 4*y^2. The function generates
 *            all primes of this form in ascending order and returns the n-th
 *            one (1‑based)
 * Range:     u8 n <= 7, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* Integer square root (floor) for uint64_t.
   Uses binary search with __int128 intermediate multiplication. */
static uint64_t isqrt_uint64(uint64_t n)
{
    uint64_t low = 0, high = 0xFFFFFFFFULL; /* sqrt(2^64-1) < 2^32 */
    while (low < high) {
        uint64_t mid = (low + high + 1) >> 1;
        if ((unsigned __int128)mid * mid <= n)
            low = mid;
        else
            high = mid - 1;
    }
    return low;
}

/* Check whether a prime p can be written as 4*x^2 + x*y + 4*y^2
   for some integers x,y.  The test uses the derived condition
   that 16*p – 63*y^2 must be a perfect square and that
   (sqrtD – y) or (sqrtD + y) is divisible by 8. */
static int is_rep_prime(uint64_t p)
{
    unsigned __int128 t16 = (unsigned __int128)16 * p;
    uint64_t max_y_sq = (uint64_t)(t16 / 63);
    uint64_t max_y = isqrt_uint64(max_y_sq);

    for (uint64_t y = 0; y <= max_y; ++y) {
        unsigned __int128 d128 = t16 - (unsigned __int128)63 * y * y;
        if (d128 > UINT64_MAX) continue; /* overflow guard */
        uint64_t d = (uint64_t)d128;
        uint64_t s = isqrt_uint64(d);
        if ((unsigned __int128)s * s != d) continue; /* not a square */

        /* Check divisibility by 8 for the two possible numerators. */
        if (s >= y && ((s - y) & 7) == 0) return 1;
        if (((s + y) & 7) == 0) return 1;
    }
    return 0;
}

/* Dynamic storage for the list of primes of the form. */
static uint64_t *rep_primes = NULL;
static size_t rep_count = 0;
static size_t rep_cap   = 0;

/* Ensure the list contains at least n elements. */
static void ensure_primes(size_t n)
{
    static uint64_t sieve_limit = 2;     /* current sieve upper bound */
    static uint64_t last_checked = 1;   /* last prime index processed */

    while (rep_count < n) {
        size_t sz = sieve_limit + 1;
        char *is_prime = (char *)malloc(sz);
        if (!is_prime) return; /* allocation failed */

        memset(is_prime, 1, sz);
        is_prime[0] = 0;
        if (sz > 1) is_prime[1] = 0;

        /* Basic sieve of Eratosthenes */
        for (uint64_t i = 2; i * i <= sieve_limit; ++i) {
            if (is_prime[i]) {
                for (uint64_t j = i * i; j <= sieve_limit; j += i)
                    is_prime[j] = 0;
            }
        }

        /* Scan primes above the last processed value and
           store those representable. */
        for (uint64_t i = last_checked + 1; i <= sieve_limit; ++i) {
            if (is_prime[i] && is_rep_prime(i)) {
                if (rep_count >= rep_cap) {
                    size_t new_cap = rep_cap ? rep_cap * 2 : 16;
                    uint64_t *tmp = (uint64_t *)realloc(rep_primes, new_cap * sizeof(uint64_t));
                    if (!tmp) return; /* allocation failed */
                    rep_primes = tmp;
                    rep_cap = new_cap;
                }
                rep_primes[rep_count++] = i;
            }
        }

        free(is_prime);
        last_checked = sieve_limit;
        if (sieve_limit <= UINT64_MAX / 2)
            sieve_limit *= 2;
        else
            break; /* cannot increase further */
    }
}

/* The required function: return the n-th term (1‑based). */
seq_t a(idx_t n)
{
    if (n < 1) return 0;  /* undefined for n=0, behavior irrelevant */
    ensure_primes((size_t)n);
    return (seq_t)rep_primes[n - 1];
}
