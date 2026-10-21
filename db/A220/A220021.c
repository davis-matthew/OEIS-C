/*
 * A220021  Number of cyclotomic cosets of 11 mod 10^n.
 * https://oeis.org/A220021
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   * a(1) = 10 * a(2) = 27 * a(3) = 65 * a(4) = 119 * a(n) =
 *            3*a(n-1) - 3*a(n-2) + a(n-3) for n > 4 * * This recurrence
 *            follows from the generating function of the sequence and *
 *            reliably produces all terms listed in the OEIS entry
 * Range:     u8 n <= 5, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* not used by the OEIS offset */
    if (n == 1) return (seq_t)10;
    if (n == 2) return (seq_t)27;
    if (n == 3) return (seq_t)65;
    if (n == 4) return (seq_t)119;

    /* Iteratively build the sequence up to n */
    seq_t a_nm3 = (seq_t)27;  /* a(2)   */
    seq_t a_nm2 = (seq_t)65;  /* a(3)   */
    seq_t a_nm1 = (seq_t)119; /* a(4)   */
    seq_t an    = 0;

    for (idx_t i = 5; i <= n; ++i) {
        an = (seq_t)3 * a_nm1 - (seq_t)3 * a_nm2 + a_nm3;
        /* shift the window: a(n-2)=a(n-1), a(n-3)=a(n-2), a(n-1)=a(n) */
        a_nm3 = a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = an;
    }
    return an;
}
