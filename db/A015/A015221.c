/*
 * A015221  Odd square pyramidal numbers.
 * https://oeis.org/A015221
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (1 + 4*n + (-1)^n) * (2 + 4*n + (-1)^n) * (3 + 4*n +
 *            (-1)^n) / 24
 * Range:     u8 n <= 1, u16 n <= 9, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t f1, f2, f3;
    seq_t four_n = (seq_t)4 * n;            /* 4*n */

    if ((n & 1) == 0) {                    /* n is even */
        f1 = four_n + 2;   /* 1 + 4*n + 1  */
        f2 = four_n + 3;   /* 2 + 4*n + 1  */
        f3 = four_n + 4;   /* 3 + 4*n + 1  */
    } else {                              /* n is odd */
        f1 = four_n;        /* 1 + 4*n - 1  */
        f2 = four_n + 1;    /* 2 + 4*n - 1  */
        f3 = four_n + 2;    /* 3 + 4*n - 1  */
    }

    seq_t prod = f1 * f2 * f3;
    prod /= 24;
    return prod;
}
