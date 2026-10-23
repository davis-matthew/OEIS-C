/*
 * A133020  Divisors of 10000.
 * https://oeis.org/A133020
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, full, nonn
 * Formula:   a(n) = n-th smallest divisor of 10000 (sorted ascending)
 * Range:     u8 n <= 7, u16 n <= 25, u32 n <= 25, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>

static seq_t divisors[25];
static bool initialized = false;

static int cmp_seq(const void *p1, const void *p2) {
    seq_t a = *(const seq_t *)p1;
    seq_t b = *(const seq_t *)p2;
    if (a < b) return -1;
    if (a > b) return 1;
    return 0;
}

seq_t a(idx_t n)
{
    if (!initialized) {
        seq_t pow2[5] = {(seq_t)1, (seq_t)2, (seq_t)4, (seq_t)8, (seq_t)16};
        seq_t pow5[5] = {(seq_t)1, (seq_t)5, (seq_t)25, (seq_t)125, (seq_t)625};
        idx_t cnt = 0;
        for (idx_t i = 0; i < 5; ++i) {
            for (idx_t j = 0; j < 5; ++j) {
                divisors[cnt++] = pow2[i] * pow5[j];
            }
        }
        qsort(divisors, 25, sizeof(seq_t), cmp_seq);
        initialized = true;
    }
    if (n == 0 || n > 25) {
        return (seq_t)0;          /* undefined beyond the 25 divisors of 10000 */
    }
    return divisors[n - 1];
}
