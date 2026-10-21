/*
 * A207021  Number of nX5 0..1 arrays avoiding 0 0 0 and 0 1 1 horizontally
 *          and 0 0 1 and 1 0 1 vertically.
 * https://oeis.org/A207021
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (13*n^4 + 78*n^3 + 104*n^2 - 117*n)/6
 * Range:     u8 n <= 1, u16 n <= 7, u32 n <= 134, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h> /* for uint64_t, though seq_t and idx_t are already defined */

seq_t a(idx_t n) {
    if (n == 0)
        return (seq_t)0;               /* undefined offset, but harmless */
    seq_t n0 = (seq_t)n;               /* n */
    seq_t n1 = n0 * n0;                /* n^2 */
    seq_t n2 = n1 * n0;                /* n^3 */
    seq_t n3 = n2 * n0;                /* n^4 */
    seq_t term1 = (seq_t)13 * n3;      /* 13*n^4 */
    seq_t term2 = (seq_t)78 * n2;      /* 78*n^3 */
    seq_t term3 = (seq_t)104 * n1;     /* 104*n^2 */
    seq_t term4 = (seq_t)117 * n0;     /* 117*n */
    seq_t num = term1 + term2 + term3 - term4;
    return num / (seq_t)6;
}
