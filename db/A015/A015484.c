/*
 * A015484  q-Fibonacci numbers for q=11, scaling a(n-1).
 * https://oeis.org/A015484
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 11^(n-1) * a(n-1) + a(n-2) a(0) = 0, a(1) = 1 Recurrence
 *            is computed iteratively
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 6, u128 n <= 9,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle initial conditions */
    if (n == 0) return 0;
    if (n == 1) return 1;

    seq_t a_nm2 = 0;      /* a(n-2) */
    seq_t a_nm1 = 1;      /* a(n-1) */
    seq_t power = 1;      /* Will become 11^(i-1) after the first multiplication */

    for (idx_t i = 2; i <= n; ++i) {
        power *= 11;                        /* power = 11^(i-1) */
        seq_t cur = power * a_nm1 + a_nm2;   /* a(i) = 11^(i-1)*a(i-1) + a(i-2) */
        a_nm2 = a_nm1;
        a_nm1 = cur;
    }

    return a_nm1;   /* a(n) */
}
