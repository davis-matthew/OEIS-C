/*
 * A018689  Divisors of 861.
 * https://oeis.org/A018689
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   seq_cmp
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Divisors of 861 in ascending order */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/seq_cmp.h"

/* Helper to use the provided seq_cmp for qsort */
static int cmp_seq_t(const void *p1, const void *p2)
{
    return seq_cmp(p1, p2);
}

/* Cached sorted divisor list */
static seq_t divisors[8];
static bool divisors_initialized = false;

/* One–time initialization of the divisor table */
static void init_divisors(void)
{
    /* 861 = 3 · 7 · 41, all primes of exponent 1 */
    seq_t tmp;
    int idx = 0;
    for (int i = 0; i <= 1; ++i) {
        for (int j = 0; j <= 1; ++j) {
            for (int k = 0; k <= 1; ++k) {
                tmp = (seq_t)1;
                if (i) tmp *= (seq_t)3;
                if (j) tmp *= (seq_t)7;
                if (k) tmp *= (seq_t)41;
                divisors[idx++] = tmp;
            }
        }
    }
    /* Sort ascending using the supplied comparator */
    qsort(divisors, 8, sizeof(seq_t), cmp_seq_t);
    divisors_initialized = true;
}

seq_t a(idx_t n)
{
    if (!divisors_initialized) {
        init_divisors();
    }
    if (n < 1 || n > 8) {
        return 0;   /* out‑of‑range request */
    }
    return divisors[n - 1];
}
