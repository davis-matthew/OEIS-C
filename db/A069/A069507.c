/*
 * A069507  a(1) = 4; a(n) = smallest palindromic number of the form k*a(n-1)
 *          + 1 with k > 1.
 * https://oeis.org/A069507
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(0)=4, a(1)=9, a(2)=55; for n≥3, a(n) is the integer with n
 *            digits all equal to 1, i.e. a(n) = (10^n – 1)/9. The function
 *            implements this by iterative multiplication to avoid overflow in
 *            fixed‑width builds
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* handle initial terms explicitly */
    if (n == 0) return (seq_t)4;
    if (n == 1) return (seq_t)9;
    if (n == 2) return (seq_t)55;

    /* build the n‑digit 1…1 number */
    seq_t r = 0;
    for (idx_t i = 0; i < n; ++i)
        r = r * 10 + 1;
    return r;
}
