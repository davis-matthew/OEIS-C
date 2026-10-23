/*
 * A016772  a(n) = (3*n)^8.
 * https://oeis.org/A016772
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (3*n)^8
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 5, u64 n <= 85, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert the index to seq_t to avoid unintended overflow */
    seq_t x = (seq_t)3 * (seq_t)n;      /* 3*n */
    seq_t res = (seq_t)1;              /* identity for multiplication */

    /* Exponent 8: multiply x by itself 8 times */
    for (int i = 0; i < 8; ++i) {
        res *= x;
    }

    return res;
}
