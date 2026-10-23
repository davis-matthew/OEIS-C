/*
 * A115811  Integers i such that 9*i = 25 X i, but 17*i is not 49 X i.
 * https://oeis.org/A115811
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the nth integer i (with 1‑based indexing) satisfying 9 *
 *            i = 25 X i and 17 * i ≠ 49 X i, where X denotes carryless
 *            (GF(2)[X]) multiplication
 * Range:     u8 n <= 1, u16 n <= 10, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Carryless multiplication in GF(2)[X] */
static inline seq_t carryless_mul(seq_t a, seq_t b)
{
    seq_t res = 0;
    while (b != 0) {
        if (b & 1) res ^= a;
        a <<= 1;
        b >>= 1;
    }
    return res;
}

seq_t a(idx_t n)
{
    /* Handle trivial or invalid request */
    if (n == 0) return 0;

    const seq_t nine      = 9;
    const seq_t seventeen = 17;
    const seq_t twentyfive= 25;
    const seq_t fortynine = 49;

    idx_t count = 0;
    seq_t i = 1;

    while (1) {
        seq_t prod9      = nine * i;
        seq_t carry25    = carryless_mul(twentyfive, i);

        if (prod9 == carry25) {
            seq_t prod17   = seventeen * i;
            seq_t carry49   = carryless_mul(fortynine, i);

            if (prod17 != carry49) {
                count++;
                if (count == n) return i;
            }
        }
        i++;
    }
}
