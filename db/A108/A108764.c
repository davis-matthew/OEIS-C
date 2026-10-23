/*
 * A108764  Products of exactly two supersingular primes (A002267).
 * https://oeis.org/A108764
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, fini, full, nonn
 * Formula:   Products of exactly two supersingular primes (A108764)
 * Range:     u8 n <= 1, u16 n <= 120, u32 n <= 120, u64 n <= 120,
 *            u128 n <= 120, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* List of supersingular primes (A002267) */
static const uint64_t supers[15] = {
    2ULL, 3ULL, 5ULL, 7ULL, 11ULL, 13ULL,
    17ULL, 19ULL, 23ULL, 29ULL, 31ULL,
    41ULL, 47ULL, 59ULL, 71ULL
};

static seq_t products[120];
static bool ready = false;

/* Comparison routine for qsort */
static int cmp_seq(const void *a, const void *b)
{
    seq_t v1 = *(const seq_t *)a;
    seq_t v2 = *(const seq_t *)b;
    return (v1 < v2) ? -1 : (v1 > v2) ? 1 : 0;
}

/* Return the n-th term of A108764 (1‑based).  For n>120 0 is returned. */
seq_t a(idx_t n)
{
    if (!ready) {
        size_t count = 0;
        for (size_t i = 0; i < 15; ++i) {
            for (size_t j = i; j < 15; ++j) {
                products[count++] = (seq_t)supers[i] *
                                    (seq_t)supers[j];
            }
        }
        qsort(products, 120, sizeof(seq_t), cmp_seq);
        ready = true;
    }
    if (n == 0 || n > 120) {
        return 0;
    }
    return products[n - 1];
}
