/*
 * A055554  An arithmetic progression each term of which is followed by at
 *          least 4 nonsquarefree consecutive integers.
 * https://oeis.org/A055554
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n-1)*1757711340 + 242
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 3, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* The sequence is defined only for n >= 1.  For n == 0 the behaviour
       is immaterial, so we return 0 in that case. */
    if (n == 0)
        return (seq_t)0;

    /* (n-1) is converted to the sequence type before multiplication
       to avoid overflow in intermediate 64‑bit arithmetic. */
    return ((seq_t)(n - 1)) * ((seq_t)1757711340) + ((seq_t)242);
}
