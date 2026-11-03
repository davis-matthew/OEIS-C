/*
 * A117991  Number of distinct rational numbers that can be written as
 *          fractions of the first n composite numbers.
 * https://oeis.org/A117991
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of distinct reduced fractions a/b where a and b
 *            are the first n composite numbers
 * Range:     u8 n <= 21, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Validated: partial, pass, 53 terms through n = 53 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Simple sieve to generate composites up to a bound. */
static void generate_composites(idx_t n, uint64_t *out)
{
    /* Rough upper bound for the n‑th composite.  For n<=10000 this is safe. */
    idx_t limit = n * 20ULL + 100ULL;
    /* Ensure limit is at least 10. */
    if (limit < 10ULL) limit = 10ULL;

    /* Increase limit until we have enough composites. */
    while (true) {
        /* Sieve of Eratosthenes. */
        bool *is_prime = (bool *)malloc((limit + 1) * sizeof(bool));
        if (!is_prime) abort();
        for (idx_t i = 0; i <= limit; ++i) is_prime[i] = true;
        is_prime[0] = is_prime[1] = false;
        for (idx_t i = 2; i * i <= limit; ++i) {
            if (is_prime[i]) {
                for (idx_t j = i * i; j <= limit; j += i)
                    is_prime[j] = false;
            }
        }
        idx_t cnt = 0;
        for (idx_t i = 4; i <= limit && cnt < n; ++i) {
            if (!is_prime[i]) out[cnt++] = (uint64_t)i;
        }
        free(is_prime);
        if (cnt >= n) return;
        /* Not enough composites, increase bound. */
        limit *= 2;
    }
}

static inline uint64_t gcd_uint64(uint64_t a, uint64_t b)
{
    while (b != 0) {
        uint64_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* Main required function. */
seq_t a(idx_t n)
{
    if (n == (idx_t)0) return (seq_t)0;   /* offset is 1 */

    /* Allocate array for composites. */
    uint64_t *comps = (uint64_t *)malloc(n * sizeof(uint64_t));
    if (!comps) abort();
    generate_composites(n, comps);
    uint64_t M = comps[n - 1];   /* largest composite among first n */

    /* Max value for reduced numerator/denominator does not exceed M. */
    idx_t size = M + 1ULL;  /* range [0..M] */

    /* Bitset for visited (p,q) pairs. */
    size_t total_bits = (size_t)size * (size_t)size;
    size_t words = (total_bits + 63ULL) >> 6;
    uint64_t *vis = (uint64_t *)calloc(words, sizeof(uint64_t));
    if (!vis) abort();

    seq_t count = 0;
    for (idx_t ii = 0; ii < n; ++ii) {
        uint64_t a_val = comps[ii];
        for (idx_t jj = 0; jj < n; ++jj) {
            uint64_t b_val = comps[jj];
            uint64_t g = gcd_uint64(a_val, b_val);
            uint64_t p = a_val / g;
            uint64_t q = b_val / g;
            size_t idx = (size_t)p * size + (size_t)q;
            size_t word = idx >> 6;
            uint64_t mask = 1ULL << (idx & 63ULL);
            if ((vis[word] & mask) == 0) {
                vis[word] |= mask;
                count++;
            }
        }
    }

    free(comps);
    free(vis);
    return count;
}
