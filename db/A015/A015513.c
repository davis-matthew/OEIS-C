/*
 * A015513  a(1) = 1, a(n) = Sum_{k=1..n-1} ((11^k - 1)/10)*a(k).
 * https://oeis.org/A015513
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1) = 1, a(2) = 1 for n = 1,2 a(n) = ((11^(n-1) + 9)/10) *
 *            a(n-1) for n >= 3 This follows from the recurrence a(n) =
 *            Σ_{k=1}^{n-1} ((11^k - 1)/10)*a(k)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 5, u64 n <= 7, u128 n <= 10,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 2)
        return (seq_t)1;

    seq_t a_val = (seq_t)1;        /* a(2) */
    seq_t pow11 = (seq_t)11;       /* 11^1, will be advanced to 11^(i-1) before use */

    for (idx_t i = 3; i <= n; ++i) {
        pow11 *= (seq_t)11;                       /* now 11^(i-1) */
        seq_t factor = (pow11 + (seq_t)9) / (seq_t)10;
        a_val *= factor;
    }

    return a_val;
}
