/*
 * A050493  a(n) = sum of binary digits of n-th triangular number.
 * https://oeis.org/A050493
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, nice, nonn
 * Formula:   a(n) = A000120(n*(n+1)/2) (population count of the n-th
 *            triangular number)
 * Range:     u8 n <= 22, u16 n <= 402, u32 n <= 1024, u64 n <= 1024,
 *            u128 n <= 1024, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t popcount_seq_t(seq_t v)
{
    seq_t c = 0;
    while (v)
    {
        c += (v & 1);
        v >>= 1;
    }
    return c;
}

seq_t a(idx_t n)
{
    seq_t t = (seq_t)n * ((seq_t)n + 1) / 2;   /* n-th triangular number */
    return popcount_seq_t(t);
}
