/*
 * A223994  Number of nX3 0..2 arrays with antidiagonals unimodal and rows and
 *          diagonals nondecreasing.
 * https://oeis.org/A223994
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 10*a(n-1) -24*a(n-2) -2*a(n-3) +30*a(n-4) +96*a(n-5)
 *            -150*a(n-6) -108*a(n-7) +48*a(n-8) +270*a(n-9) Initial values
 *            (offset 1): a(1)=10, a(2)=76, a(3)=476, a(4)=2980, a(5)=18672,
 *            a(6)=117386, a(7)=739672, a(8)=4664776, a(9)=29428242
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 23, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* Base values for n = 1 ... 9 */
    static const seq_t base[9] = {
        (seq_t)10,  (seq_t)76,     (seq_t)476,
        (seq_t)2980, (seq_t)18672, (seq_t)117386,
        (seq_t)739672, (seq_t)4664776, (seq_t)29428242
    };

    if (n <= 9)
        return base[n-1];

    /* Circular buffer holding the last 9 terms: buf[0] = a(k-9), ..., buf[8] = a(k-1) */
    seq_t buf[9];
    for (int i = 0; i < 9; ++i)
        buf[i] = base[i];

    for (idx_t k = 10; k <= n; ++k) {
        seq_t new_val =
            ((seq_t)10  * buf[8])                     /* a(k-1)  */
          - ((seq_t)24  * buf[7])                     /* a(k-2)  */
          - ((seq_t)2   * buf[6])                     /* a(k-3)  */
          + ((seq_t)30  * buf[5])                     /* a(k-4)  */
          + ((seq_t)96  * buf[4])                     /* a(k-5)  */
          - ((seq_t)150 * buf[3])                     /* a(k-6)  */
          - ((seq_t)108 * buf[2])                     /* a(k-7)  */
          + ((seq_t)48  * buf[1])                     /* a(k-8)  */
          + ((seq_t)270 * buf[0]);                    /* a(k-9)  */

        /* Shift buffer left by one position */
        for (int i = 0; i < 8; ++i)
            buf[i] = buf[i + 1];
        buf[8] = new_val;
    }

    return buf[8];
}
