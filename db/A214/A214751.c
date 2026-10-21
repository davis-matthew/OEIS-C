/*
 * A214751  Least m>0 such that n-m divides ceiling(n/2)+ceiling(m/2).
 * https://oeis.org/A214751
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = least m>0 such that (n-m) | (ceil(n/2)+ceil(m/2))
 * Range:     u8 n <= 259, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Offset: 2 (a(2) is the first term) */
seq_t a(idx_t n)
{
    /* If n < 2 the sequence is not defined; return 0 as a safe default. */
    if (n < 2)
        return (seq_t)0;

    /* ceil(n/2) computed once */
    seq_t ceil_n = ((seq_t)n + 1) / 2;

    /* Search for the smallest m (1 <= m < n) satisfying the divisibility. */
    for (idx_t m = 1; m < n; ++m) {
        seq_t ceil_m = ((seq_t)m + 1) / 2;
        seq_t d      = (seq_t)(n - m);          /* always > 0 */
        seq_t sum    = ceil_n + ceil_m;
        if (sum % d == 0)
            return (seq_t)m;
    }

    /* According to the definition, there is always a solution for n>=2.
       The following return is only to satisfy the compiler. */
    return (seq_t)n;
}
