/*
 * A069959  Define C(n) by the recursion C(0) = 2*i where i^2 = -1, C(n+1) =
 *          1/(1 + C(n)), then a(n) = 2*(-1)^n/Im(C(n)) where Im(z) denotes
 *          the imaginary part of the complex number z.
 * https://oeis.org/A069959
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2*a(n-1) + 2*a(n-2) - a(n-3) (a(0)=1, a(1)=5, a(2)=8)
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 22, u64 n <= 45, u128 n <= 91,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the first three terms directly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)5;
    if (n == 2) return (seq_t)8;

    /* Use the three‑term linear recurrence */
    seq_t a_nm3 = (seq_t)1; /* a(n-3) */
    seq_t a_nm2 = (seq_t)5; /* a(n-2) */
    seq_t a_nm1 = (seq_t)8; /* a(n-1) */
    seq_t a_n   = 0;

    for (idx_t i = 3; i <= n; ++i)
    {
        a_n = 2 * a_nm1 + 2 * a_nm2 - a_nm3; /* a(n) = 2*a(n-1)+2*a(n-2)-a(n-3) */
        /* shift indices for next iteration */
        a_nm3 = a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = a_n;
    }

    return a_n;
}
