/*
 * A118864  Start with 1 and repeatedly place the first digit at the end of
 *          the number and add 17.
 * https://oeis.org/A118864
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(1) = 1; For n>1, let k = floor(log10(a(n-1))); // number of
 *            digits minus 1 Let msd = a(n-1) / 10^k (most significant digit)
 *            Let rem = a(n-1) % 10^k (remaining digits) Then a(n) = rem * 10
 *            + msd + 17 This implements the operation: move the first digit
 *            to the end and add 17
 * Range:     u8 n <= 61, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* The sequence is defined for n >= 1.  For n < 1, return 0 just in case. */
    if (n < 1) return (seq_t)0;

    seq_t cur = (seq_t)1;          /* a(1) */
    for (idx_t i = 1; i < n; ++i)
    {
        /* Find the power of ten 10^k such that 10^k <= cur < 10^(k+1). */
        seq_t power = (seq_t)1;
        while (power * (seq_t)10 <= cur)
            power *= (seq_t)10;

        seq_t msd   = cur / power;        /* most significant digit */
        seq_t rem   = cur % power;        /* remaining digits */

        /* Move msd to the end and add 17. */
        cur = rem * (seq_t)10 + msd + (seq_t)17;
    }

    return cur;
}
