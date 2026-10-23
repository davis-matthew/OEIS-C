/*
 * A186195  Expansion of (1+5x+sqrt(1+2x+9x^2))/(2(1+2x)).
 * https://oeis.org/A186195
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   n*a(n)+(4*n-3)*a(n-1)+(13*n-33)*a(n-2)+18*(n-3)*a(n-3)=0 Initial
 *            values: a(0)=1, a(1)=1, a(2)=0 This implementation uses the
 *            recurrence directly, computing each term in O(n) time without
 *            dynamic allocation or memoisation
 * Range:     i8 n <= 7, i16 n <= 12, i32 n <= 22, i64 n <= 30, i128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == (idx_t)0) return (seq_t)1;
    if (n == (idx_t)1) return (seq_t)1;
    if (n == (idx_t)2) return (seq_t)0;

    seq_t a_nm3 = (seq_t)1;   /* a(k-3), starting with a(0) */
    seq_t a_nm2 = (seq_t)1;   /* a(k-2), starting with a(1) */
    seq_t a_nm1 = (seq_t)0;   /* a(k-1), starting with a(2) */
    seq_t a_n   = (seq_t)0;   /* placeholder for a(k) */

    for (idx_t k = (idx_t)3; k <= n; ++k)
    {
        seq_t term1 = (seq_t)(4*k-3) * a_nm1;
        seq_t term2 = (seq_t)(13*k-33) * a_nm2;
        seq_t term3 = (seq_t)18 * (k-3) * a_nm3;
        seq_t num   = -(term1 + term2 + term3);
        a_n = num / (seq_t)k;

        /* shift indices for next iteration */
        a_nm3 = a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = a_n;
    }

    return a_n;
}
