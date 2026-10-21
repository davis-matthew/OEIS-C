/*
 * A221588  Equals one maps: number of n X 3 binary arrays indicating the
 *          locations of corresponding elements equal to exactly one of their
 *          king-move neighbors in a random 0..3 n X 3 array.
 * https://oeis.org/A221588
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 8^n - 169 × 2^(2n-7) for n > 4; a(1)=4, a(2)=40,
 *            a(3)=418, a(4)=3760 (offset 1)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 21, u128 n <= 42,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Handle the small indices explicitly. */
    if (n == 0)                 /* offset is 1 – not used, but safe */
        return (seq_t)0;
    if (n <= 4) {
        switch (n) {
            case 1: return (seq_t)4;
            case 2: return (seq_t)40;
            case 3: return (seq_t)418;
            case 4: return (seq_t)3760;
        }
    }

    /* For n ≥ 5 use the closed‑form expression */
    seq_t term1 = pow2(3 * n);            /* 2^(3n) = 8^n */
    seq_t term2 = pow2(2 * n - 7);        /* 2^(2n-7)        */
    term2 = term2 * (seq_t)169;           /* 169 × 2^(2n-7)   */

    return term1 - term2;
}
