/*
 * A117371  Number of primes between smallest prime divisor of n and largest
 *          prime divisor of n that are coprime to n (not factors of n).
 * https://oeis.org/A117371
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A001221(A137795(n))
 * Range:     u8 n <= 3253, u16 n <= 9692, u32 n <= 9692, u64 n <= 9692,
 *            u128 n <= 9692, big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A117371: Number of primes between the smallest and
   largest prime divisors of n that are coprime to n (i.e., not divisors of n). */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#include "../utils/build_primes_upto.h"

/* Prime list cache -------------------------------------------------------- */
static bool primes_inited = false;
static uint64_t *prime_list   = NULL;
static size_t   prime_cnt     = 0;
#define PRIME_LIMIT 200000              /* sufficient for n <= 10000 */

static inline size_t first_gt(const uint64_t *arr, size_t n, uint64_t x)
{
    size_t lo = 0, hi = n;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        if (arr[mid] <= x) lo = mid + 1; else hi = mid;
    }
    return lo;                     /* first element > x  (may equal n) */
}

static inline size_t first_ge(const uint64_t *arr, size_t n, uint64_t x)
{
    size_t lo = 0, hi = n;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        if (arr[mid] < x) lo = mid + 1; else hi = mid;
    }
    return lo;                     /* first element >= x  (may equal n) */
}

/* Main sequence function --------------------------------------------------- */
seq_t a(idx_t n)
{
    /* Initialise prime list once ------------------------------------------ */
    if (!primes_inited) {
        build_primes_upto((idx_t)PRIME_LIMIT, &prime_list, &prime_cnt);
        primes_inited = true;
    }

    if (n <= 1) return (seq_t)0;   /* by definition a(1)=0 */

    /* Factorise n – collect distinct prime factors in ascending order ---- */
    uint64_t temp = (uint64_t)n;
    uint64_t factors[64];
    size_t fcnt = 0;

    for (size_t i = 0; i < prime_cnt && prime_list[i] * prime_list[i] <= temp; ++i) {
        uint64_t p = prime_list[i];
        if (temp % p == 0) {
            factors[fcnt++] = p;
            while (temp % p == 0) temp /= p;
        }
    }
    if (temp > 1) {
        factors[fcnt++] = temp;      /* remaining prime factor > sqrt(n) */
    }

    uint64_t spf = factors[0];          /* smallest prime factor */
    uint64_t lpf = factors[fcnt - 1];    /* largest prime factor */

    if (spf == lpf) return (seq_t)0;    /* no primes strictly between */

    /* Count primes strictly between spf and lpf -------------------------- */
    size_t idx_spf = first_gt(prime_list, prime_cnt, spf);  /* first prime > spf */
    size_t idx_lpf = first_ge(prime_list, prime_cnt, lpf);  /* first prime >= lpf */
    uint64_t prime_between = 0;
    if (idx_lpf > idx_spf) prime_between = (uint64_t)(idx_lpf - idx_spf);

    /* Exclude prime factors that lie strictly between spf and lpf ------- */
    if (fcnt > 2) prime_between -= (uint64_t)(fcnt - 2);

    return (seq_t)prime_between;
}
