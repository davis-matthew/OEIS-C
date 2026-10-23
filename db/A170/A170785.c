/*
 * A170785  a(n) = n^9*(n^3 + 1)/2.
 * https://oeis.org/A170785
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^9*(n^3 + 1)/2
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 6, u64 n <= 42, u128 n <= 1722,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sn = n;
    seq_t n2 = sn * sn;
    seq_t n3 = n2 * sn;
    seq_t n9 = n3 * n3 * n3;   /* n^9 */

    /* Divide the even factor first to keep intermediate values small */
    if ((sn & 1) == 0) {             /* n even → n^9 even */
        return (n9 / 2) * (n3 + 1);
    } else {                          /* n odd  → n^3+1 even */
        return n9 * ((n3 + 1) / 2);
    }
}
