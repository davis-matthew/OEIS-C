/*
 * A162829  Positive numbers n such that 2*120*n/(120+n) are integers.
 * https://oeis.org/A162829
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = n such that 240*n/(120 + n) is integer (equivalent to 120
 *            + n divides 28800)
 * Range:     u8 n <= 1, u16 n <= 40, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   cmp_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/cmp_seq.h"

/* Static storage for the pre‑computed values.  Number of
   needed terms is 53 (divisors of 28800 greater than 120). */
static seq_t seq_values[72];        /* room for all divisors of 28800   */
static size_t seq_len = 0;          /* number of valid terms */
static bool seq_init = false;       /* initialization flag */

/* Comparator for qsort on seq_t integers. */

/* One‑time initialization: generate all divisors of 28800,
   keep those >120, compute n = d-120, sort. */
static void init_seq(void)
{
    /* Powers of the prime factors of 28800. */
    seq_t pow2[8], pow3[3], pow5[3];
    pow2[0] = 1;
    for (int i = 1; i < 8; ++i) pow2[i] = pow2[i-1] * 2;
    pow3[0] = 1;
    for (int i = 1; i < 3; ++i) pow3[i] = pow3[i-1] * 3;
    pow5[0] = 1;
    for (int i = 1; i < 3; ++i) pow5[i] = pow5[i-1] * 5;

    seq_t divisors[72];
    int dcnt = 0;
    for (int i = 0; i < 8; ++i)
        for (int j = 0; j < 3; ++j)
            for (int k = 0; k < 3; ++k)
                divisors[dcnt++] = pow2[i] * pow3[j] * pow5[k];

    /* Keep only divisors greater than 120. */
    size_t ncnt = 0;
    for (int i = 0; i < dcnt; ++i) {
        if (divisors[i] > (seq_t)120) {
            seq_values[ncnt++] = divisors[i] - (seq_t)120;
        }
    }

    /* Sort the resulting sequence. */
    qsort(seq_values, ncnt, sizeof(seq_t), cmp_seq);
    seq_len = ncnt;
    seq_init = true;
}

/* Main sequence accessor. */
seq_t a(idx_t n)
{
    if (!seq_init) init_seq();
    if (n < 1 || (size_t)n > seq_len) return 0;
    return seq_values[n - 1];
}
