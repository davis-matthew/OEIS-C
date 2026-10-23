/*
 * A031940  Length of longest legal domino snake using full set of dominoes up
 *          to [n:n].
 * https://oeis.org/A031940
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = C(n,2)+n if n is odd; a(n) = C(n,2)+n/2+1 if n is even
 * Range:     u8 n <= 22, u16 n <= 362, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Offset is 1; for n == 0 we return 0 as a safe default. */
    if (n == 0)
        return 0;

    if (n & (idx_t)1) {            /* n is odd */
        seq_t nn = (seq_t)n;
        return nn * (nn + (seq_t)1) / (seq_t)2;
    } else {                        /* n is even */
        seq_t nn = (seq_t)n;
        return nn * nn / (seq_t)2 + (seq_t)1;
    }
}
