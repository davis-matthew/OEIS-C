/*
 * A071236  a(n) = (n^10 + n^5)/2.
 * https://oeis.org/A071236
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^10 + n^5)/2 = n^5 * (n^5 + 1) / 2
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 9, u64 n <= 90, u128 n <= 2000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* promote the index to seq_t */
    seq_t nn = (seq_t)n;

    /* compute n^5 = nn^5 */
    seq_t n5 = nn;
    for (int i = 0; i < 4; ++i) {       /* multiply 4 more times */
        n5 *= nn;
    }

    /* compute n5*(n5+1)/2 with a small overflow guard:
       divide one factor by 2 first, depending on parity */
    if ((n5 & 1) == 0) {                /* n5 even */
        return (n5 / 2) * (n5 + 1);
    } else {                            /* n5 odd */
        return ((n5 + 1) / 2) * n5;
    }
}
