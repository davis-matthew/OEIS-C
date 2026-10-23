/*
 * A107505  Theta series of quadratic form with Gram matrix [ 2, 1, 0, 1; 1,
 *          4, 1, 0; 0, 1, 4, -2; 1, 0, -2, 8].
 * https://oeis.org/A107505
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2 * ∏_{p^e‖n} (p^(e+1)-1)/(p-1) for p≠13, factors for
 *            p=13 equal 1; a(0)=1
 * Range:     u8 n <= 59, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>

/* ---------- prime generation helpers (dynamic sieve) ---------- */

static uint64_t *prime_list = NULL;   /* array of primes */
static size_t     prime_cnt  = 0;
static size_t     prime_cap  = 0;
static uint64_t   last_checked = 1;

/* add a prime to the list, enlarging the array if necessary */
static void add_prime(uint64_t p)
{
    if (prime_cnt == prime_cap) {
        prime_cap = prime_cap ? prime_cap * 2 : 16;
        prime_list = (uint64_t*)realloc(prime_list, prime_cap * sizeof(uint64_t));
    }
    prime_list[prime_cnt++] = p;
}

/* integer square‑root (floor) using binary search */
static uint64_t isqrt_uint64(uint64_t x)
{
    uint64_t low = 0, high = x;
    while (low < high) {
        uint64_t mid = (low + high + 1) >> 1;
        if (mid <= x / mid) low = mid;
        else high = mid - 1;
    }
    return low;
}

/* ensure prime_list contains all primes ≤ limit */
static void ensure_primes_up_to(uint64_t limit)
{
    if (limit <= last_checked) return;

    for (uint64_t n = last_checked + 1; n <= limit; ++n) {
        int is_prime = 1;
        uint64_t r = isqrt_uint64(n);
        for (size_t i = 0; i < prime_cnt && prime_list[i] <= r; ++i) {
            if (n % prime_list[i] == 0) { is_prime = 0; break; }
        }
        if (is_prime) add_prime(n);
    }
    last_checked = limit;
}

/* ---------- sequence calculation ---------- */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;          /* a(0) = 1 */

    uint64_t temp = (uint64_t)n;
    seq_t result = (seq_t)1;

    /* handle prime factor 13 (factor = 1) */
    while (temp % 13ULL == 0) temp /= 13ULL;

    /* generate primes up to sqrt(temp) for factorisation */
    uint64_t limit = isqrt_uint64(temp);
    ensure_primes_up_to(limit);

    /* factorise remaining part using primes */
    for (size_t i = 0; i < prime_cnt && prime_list[i] * prime_list[i] <= temp; ++i) {
        uint64_t p = prime_list[i];
        if (temp % p == 0) {
            uint64_t exp = 0;
            while (temp % p == 0) { temp /= p; ++exp; }

            /* compute factor = 1 + p + p^2 + ... + p^exp   */
            seq_t factor = (seq_t)1;
            seq_t pow = (seq_t)1;
            for (uint64_t j = 0; j < exp; ++j) {
                pow *= (seq_t)p;
                factor += pow;
            }
            result *= factor;
        }
    }

    /* if a prime factor > sqrt(original n) remains */
    if (temp > 1) {
        /* temp is prime */
        if (temp != 13ULL) {
            result *= ((seq_t)1 + (seq_t)temp);
        }
        /* else factor 1 – nothing to do */
    }

    return result * (seq_t)2;          /* final multiplier 2 for n>0 */
}
