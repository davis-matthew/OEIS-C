/*
 * A024189  a(n) = ((n+3)!/2)*Sum_{k=1..n} (-1)^(k+1)/(k+3).
 * https://oeis.org/A024189
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ((n+3)!/2)*Sum_{k=1..n} (-1)^(k+1)/(k+3)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 18, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* a(1) = 3 by definition */
    if (n == 0) return 0;          /* undefined for offset, return 0 */
    if (n == 1) return 3;

    seq_t result = 3;              /* a(1) */
    seq_t fact = 6;                /* (1+2)! = 3! */

    for (idx_t i = 2; i <= n; ++i) {
        /* update factorial to (i+2)!  */
        fact *= (seq_t)(i + 2);

        seq_t half_fact = fact / 2;           /* ((i+2)!)/2 */

        /* base = (i+3) * a(i-1) */
        seq_t base = result * (seq_t)(i + 3);

        if (i & 1)  /* i odd => sign +1 ( (-1)^(i+1) = +1 ) */
            result = base + half_fact;
        else        /* i even => sign -1 */
            result = base - half_fact;
    }

    return result;
}
