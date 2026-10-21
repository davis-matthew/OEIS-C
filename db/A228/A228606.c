/*
 * A228606  The Merrifield-Simmons index of the para-polyphenylene chain of
 *          length n.
 * https://oeis.org/A228606
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 17 * a(n-1) - 7 * a(n-2) with a(0) = 1, a(1) = 18
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;
    if (n == 1)
        return (seq_t)18;

    seq_t a0 = (seq_t)1;
    seq_t a1 = (seq_t)18;

    for (idx_t i = 2; i <= n; ++i) {
        seq_t an = (seq_t)17 * a1 - (seq_t)7 * a0;
        a0 = a1;
        a1 = an;
    }

    return a1;
}
