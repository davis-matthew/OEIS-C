/*
 * A171529  Numerator of (n-th nonprime/n).
 * https://oeis.org/A171529
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = numerator of (A018252(n)/n)
 * Range:     u8 n <= 75, u16 n <= 75, u32 n <= 75, u64 n <= 75, u128 n <= 75,
 *            big any n
 * Helpers:   gcd, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A018252(n) is the n-th nonprime number (1,4,6,8,9,10,12,...) */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/gcd.h"
#include "../utils/is_prime.h"

#ifdef __cplusplus
#include <vector>
static std::vector<seq_t> nonprimes;
static idx_t checked_prime = 1;
#else
static seq_t *nonprimes = NULL;
static size_t nonprimes_len = 0;
static size_t nonprimes_cap = 0;
static idx_t checked_prime = 1;
#endif

seq_t a(idx_t n) {
    if (n == 0) return 0; /* offset starts at 1 */

    /* Generate nonprimes up to index n */
#ifdef __cplusplus
    while (nonprimes.size() < n) {
        if (!is_prime(checked_prime))
            nonprimes.push_back((seq_t)checked_prime);
        ++checked_prime;
    }
    seq_t m = nonprimes[n - 1];
#else
    while (nonprimes_len < (size_t)n) {
        if (!is_prime(checked_prime)) {
            if (nonprimes_len == nonprimes_cap) {
                size_t new_cap = nonprimes_cap ? nonprimes_cap * 2 : 16;
                seq_t *tmp = (seq_t *)realloc(nonprimes, new_cap * sizeof(seq_t));
                if (!tmp) { /* allocation failed */
                    exit(EXIT_FAILURE);
                }
                nonprimes = tmp;
                nonprimes_cap = new_cap;
            }
            nonprimes[nonprimes_len++] = (seq_t)checked_prime;
        }
        ++checked_prime;
    }
    seq_t m = nonprimes[n - 1];
#endif

    /* Reduce the fraction m/n and return the numerator */
    seq_t den = (seq_t)n;
    seq_t g = gcd(m, den);
    return m / g;
}
