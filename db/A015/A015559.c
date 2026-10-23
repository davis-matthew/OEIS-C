/*
 * A015559  Expansion of x/(1 - 7*x - 3*x^2).
 * https://oeis.org/A015559
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 23, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: a(0)=0, a(1)=1, a(n)=7*a(n-1)+3*a(n-2) for n>=2 */

seq_t a(idx_t n)
{
    /* Handle the base cases directly. */
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;

    /* Use two running variables to generate the sequence iteratively. */
    seq_t a0 = 0;          /* a(k-2)   */
    seq_t a1 = 1;          /* a(k-1)   */
    seq_t a2;              /* a(k)     */

    for (idx_t k = 2; k <= n; ++k) {
        a2 = 7 * a1 + 3 * a0;
        a0 = a1;
        a1 = a2;
    }
    return a1;
}
