/*
 * A015262  Gaussian binomial coefficient [ n,2 ] for q = -11.
 * https://oeis.org/A015262
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 111*a(n-1) + 1221*a(n-2) - 1331*a(n-3); a(2) = 1, a(3) =
 *            111, a(4) = 13542
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 6, u64 n <= 11, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 2) { /* undefined below offset – behaviour irrelevant */
        return 0;
    }
    if (n == 2) return (seq_t)1;
    if (n == 3) return (seq_t)111;
    if (n == 4) return (seq_t)13542;

    /* Initialise with known values a(4), a(3), a(2) */
    seq_t a1 = (seq_t)13542; /* a(k-1) */
    seq_t a2 = (seq_t)111;   /* a(k-2) */
    seq_t a3 = (seq_t)1;     /* a(k-3) */

    for (idx_t i = 5; i <= n; ++i) {
        seq_t next = (seq_t)111 * a1 + (seq_t)1221 * a2 - (seq_t)1331 * a3;
        /* shift for next iteration */
        a3 = a2;
        a2 = a1;
        a1 = next;
    }
    return a1;   /* a(n) */
}
