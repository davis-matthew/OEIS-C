/*
 * A092186  a(n) = 2(m!)^2 for n = 2m and m!(m+1)! for n = 2m+1.
 * https://oeis.org/A092186
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*(m!)^2 for n = 2m; a(n) = m!*(m+1)! for n = 2m+1
 * Range:     u8 n <= 7, u16 n <= 10, u32 n <= 16, u64 n <= 25, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* pre‑allocate factorials up to m = max(n/2) for n up to ~20000 */
    static seq_t fact[10001];
    static idx_t fact_computed = 0;               /* highest index computed so far */

    /* initialise factorial(0) once */
    if (fact_computed == 0) {
        fact[0] = (seq_t)1;
    }

    /* compute the needed factorials on demand */
    idx_t m   = n >> 1;                            /* floor(n/2) */
    idx_t need = (n & 1) ? m + 1 : m;             /* m for even, m+1 for odd  */

    if (need > fact_computed) {
        for (idx_t i = fact_computed + 1; i <= need; ++i) {
            fact[i] = fact[i-1] * (seq_t)i;
        }
        fact_computed = need;
    }

    /* compute the term */
    if ((n & 1) == 0) {                           /* even n */
        seq_t f = fact[m];
        seq_t f2 = f * f;                        /* (m!)^2 */
        return f2 * (seq_t)2;                   /* 2*(m!)^2 */
    } else {                                       /* odd n */
        return fact[m] * fact[m+1];             /* m! * (m+1)! */
    }
}
