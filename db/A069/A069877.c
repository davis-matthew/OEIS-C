/*
 * A069877  Smallest number with a prime signature whose indices are the
 *          decimal digits of n.
 * https://oeis.org/A069877
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 58, u32 n <= 498, u64 n <= 8898,
 *            u128 n <= 10000, big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = smallest positive integer whose multiset of prime exponents is exactly the set
 * of non‑zero decimal digits of n.  For example, a(231) = 2³·3²·5¹ = 360.  a(0) = 1. */
#include <stddef.h>
#include "../utils/big_pow.h"

static const uint64_t first_primes[20] = {
    2ULL, 3ULL, 5ULL, 7ULL, 11ULL,
    13ULL, 17ULL, 19ULL, 23ULL, 29ULL,
    31ULL, 37ULL, 41ULL, 43ULL, 47ULL,
    53ULL, 59ULL, 61ULL, 67ULL, 71ULL
};

static int cmp_desc(const void *a, const void *b)
{
    int ia = *(const int *)a;
    int ib = *(const int *)b;
    return ib - ia;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;     /* a(0) = 1 */

    int digits[20];
    size_t dcnt = 0;
    idx_t tmp = n;
    while (tmp > 0 && dcnt < sizeof digits / sizeof digits[0]) {
        digits[dcnt++] = tmp % 10;
        tmp /= 10;
    }

    /* filter out zeros and sort remaining exponents descending */
    int exps[20];
    size_t ecnt = 0;
    for (size_t i = 0; i < dcnt; ++i) {
        if (digits[i] != 0) {
            exps[ecnt++] = digits[i];
        }
    }
    if (ecnt == 0) return (seq_t)1;   /* all zeros, e.g., n = 0 */

    qsort(exps, ecnt, sizeof exps[0], cmp_desc);

    seq_t result = (seq_t)1;
    for (size_t i = 0; i < ecnt; ++i) {
        seq_t p = (seq_t)first_primes[i];
        seq_t pw = big_pow(p, (seq_t)exps[i]);
        result *= pw;
    }
    return result;
}
