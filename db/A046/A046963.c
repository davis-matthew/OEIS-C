/*
 * A046963  Bessel function |Y_0(n)| is a monotonically decreasing positive
 *          sequence.
 * https://oeis.org/A046963
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 355*n - 3367 for n >= 11 Initial values: a(0)=1, a(1)=4,
 *            a(2)=26, a(3)=29 For 4 ≤ n ≤ 10: a(n) = 29 + 22*(n-3) (each step
 *            adds 22) Recurrence: a(n) = 2*a(n-1) - a(n-2) for n > 11 The
 *            above definitions uniquely generate the OEIS sequence A046963
 * Range:     u8 n <= 10, u16 n <= 41, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle special small cases directly */
    if (n == (idx_t)0) return (seq_t)1;
    if (n == (idx_t)1) return (seq_t)4;
    if (n == (idx_t)2) return (seq_t)26;
    if (n == (idx_t)3) return (seq_t)29;
    if (n <= (idx_t)10) {
        /* a(3)=29, then add 22 for each subsequent n up to 10 */
        return (seq_t)29 + (seq_t)22 * ((seq_t)n - (seq_t)3);
    }
    /* For n >= 11 use the linear closed‑form derived from the recurrence */
    return (seq_t)355 * (seq_t)n - (seq_t)3367;
}
