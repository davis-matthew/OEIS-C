/*
 * A117767  a(n) is the difference between the smallest square greater than
 *          prime(n) and the largest square less than prime(n), where prime(n)
 *          = A000040(n) is the n-th prime number.
 * https://oeis.org/A117767
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2*floor(sqrt(prime(n))) + 1
 * Range:     u8 n <= 1900, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence a(n) is defined for n ≥ 1.  The function below
   generates the primes on demand and computes the term using
   an integer square root routine that works for any value that
   fits in idx_t.  It requires only standard C11 headers and
   returns seq_t as requested. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* -------------- Integer square root for idx_t -------------- */
static idx_t isqrt_u64(idx_t n)
{
    idx_t lo = 0, hi = n;
    while (lo <= hi) {
        idx_t mid = lo + (hi - lo) / 2;
        __uint128_t sq = (__uint128_t)mid * (__uint128_t)mid;
        if (sq <= n)
            lo = mid + 1;
        else
            hi = mid - 1;
    }
    return hi;
}

/* -------------- Prime generator (trial division) -------------- */
static idx_t *primes    = NULL;   /* dynamic array of primes */
static idx_t primes_cap = 0;     /* capacity of array */
static idx_t primes_len = 0;     /* number of primes stored */
static idx_t next_candidate = 2; /* next odd number to test */

/* Append a prime to the static array, expanding if necessary. */
static void append_prime(idx_t p)
{
    if (primes_len == primes_cap) {
        idx_t new_cap = primes_cap ? primes_cap << 1 : 32;
        idx_t *tmp = (idx_t *)realloc(primes, new_cap * sizeof(idx_t));
        if (!tmp) abort();            /* allocation failed */
        primes = tmp;
        primes_cap = new_cap;
    }
    primes[primes_len++] = p;
}

/* Ensure that at least n primes have been generated. */
static void ensure_n_primes(idx_t n)
{
    if (n <= primes_len) return;

    while (primes_len < n) {
        bool is_prime = true;
        idx_t limit = isqrt_u64(next_candidate);
        for (idx_t i = 0; i < primes_len && primes[i] <= limit; ++i) {
            if (next_candidate % primes[i] == 0) {
                is_prime = false;
                break;
            }
        }
        if (is_prime) append_prime(next_candidate);
        ++next_candidate;
    }
}

/* -------------- Sequence function -------------- */
seq_t a(idx_t n)
{
    if (n < 1) return 0;               /* undefined for n < 1 */
    ensure_n_primes(n);
    idx_t p = primes[n - 1];           /* nth prime (0‑based) */
    idx_t sq = isqrt_u64(p);           /* floor(sqrt(p)) */
    return (seq_t)(2 * sq + 1);
}
