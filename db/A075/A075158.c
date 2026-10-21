/*
 * A075158  Prime factorization of n+1 encoded with the run lengths of binary
 *          expansion.
 * https://oeis.org/A075158
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number whose binary expansion encodes the run lengths of
 *            the prime exponents of (n+1)
 * Range:     u8 n <= 21, u16 n <= 57, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <string.h>

/* Static prime table up to a limit sufficient for n <= 10000 */
#define PRIME_LIMIT 20000
static unsigned int primes[PRIME_LIMIT];
static size_t prime_count = 0;
static bool primes_ready = false;

static void init_primes(void)
{
    if (primes_ready) return;
    bool is_composite[PRIME_LIMIT + 1];
    memset(is_composite, 0, sizeof(is_composite));
    for (int p = 2; p <= PRIME_LIMIT; ++p) {
        if (!is_composite[p]) {
            primes[prime_count++] = (unsigned int)p;
            if ((long long)p * p <= PRIME_LIMIT)
                for (int j = p * p; j <= PRIME_LIMIT; j += p)
                    is_composite[j] = true;
        }
    }
    primes_ready = true;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;         /* a(0) = 0 */

    init_primes();

    /* Factor n+1 and record exponents */
    idx_t m = (idx_t)n + 1;
    unsigned char exp_arr[PRIME_LIMIT];
    memset(exp_arr, 0, sizeof(exp_arr));

    size_t p_max_idx = 0;      /* index of largest prime factor found */
    idx_t tmp = m;
    for (size_t i = 0; i < prime_count; ++i) {
        unsigned int p = primes[i];
        if ((idx_t)p * (idx_t)p > tmp) break;
        int e = 0;
        while (tmp % p == 0) {
            tmp /= p;
            ++e;
        }
        if (e) {
            exp_arr[i] = (unsigned char)e;
            if (i > p_max_idx) p_max_idx = i;
        }
    }
    if (tmp > 1) {   /* remaining prime factor > sqrt(original) */
        unsigned int p = (unsigned int)tmp;
        /* binary search for p in primes[] */
        size_t lo = 0, hi = prime_count;
        while (lo < hi) {
            size_t mid = (lo + hi) / 2;
            if (primes[mid] < p) lo = mid + 1;
            else hi = mid;
        }
        if (lo < prime_count && primes[lo] == p) {
            exp_arr[lo] = 1;
            if (lo > p_max_idx) p_max_idx = lo;
        }
    }
    /* If m == 1 (n+1 == 1), result is 0 */
    if (p_max_idx == 0 && exp_arr[p_max_idx] == 0) return (seq_t)0;

    seq_t result = 0;

    /* Initial run of ones from the largest prime exponent */
    int e0 = exp_arr[p_max_idx];
    for (int i = 0; i < e0; ++i) {
        result = (result << 1) | 1;
    }

    if (p_max_idx == 0) return result;   /* only prime 2 or n+1==1 */

    /* Generate alternating zeros and ones from the remaining primes,
       starting with zeros for the prime immediately below the largest one. */
    int flag_zero = 1;   /* 1 -> zeros next, 0 -> ones next */
    for (int idx = (int)p_max_idx - 1; idx >= 0; --idx) {
        if (flag_zero) {
            int zeros = (int)exp_arr[idx] + 1;
            for (int j = 0; j < zeros; ++j)
                result <<= 1;          /* appending a 0 */
        } else {
            int ones = (int)exp_arr[idx] + 1;
            for (int j = 0; j < ones; ++j)
                result = (result << 1) | 1;  /* appending a 1 */
        }
        flag_zero ^= 1;  /* toggle between zeros and ones */
    }

    return result;
}
