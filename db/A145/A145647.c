/*
 * A145647  First differences of A145646.
 * https://oeis.org/A145647
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, less
 * Formula:   a(n) = (3646 + (-i)^n + i^n + 7290*n)/4
 * Range:     u8 none, u16 n <= 8, u32 n <= 13, u64 n <= 13, u128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t base;
    if (n & 1) {                     /* odd n : (-i)^n + i^n = 0 */
        base = (seq_t)3646;
    } else {                         /* even n : (-i)^n + i^n = 2*(-1)^(n/2) */
        idx_t k = n >> 1;            /* k = n/2 */
        base = (k & 1) ? (seq_t)3644   /* k odd → -2 added: 3646-2 */
                        : (seq_t)3648; /* k even → +2 added: 3646+2 */
    }
    seq_t term = (seq_t)7290 * n;     /* 7290 * n */
    return (base + term) / 4;         /* division by 4 is exact */
}
