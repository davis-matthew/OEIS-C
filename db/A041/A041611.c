/*
 * A041611  Denominators of continued fraction convergents to sqrt(323).
 * https://oeis.org/A041611
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(0)=1, a(1)=1, a(2)=35, a(3)=36, a(n)=36*a(n-2)-a(n-4) for n>3
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 13, u64 n <= 25, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Fast return for the first four values */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)35;
    if (n == 3) return (seq_t)36;

    /* Sliding window keeping the last four terms.
       The indices used are always non‑negative because n>=4. */
    seq_t v[4];
    v[0] = (seq_t)1;   /* a(0) */
    v[1] = (seq_t)1;   /* a(1) */
    v[2] = (seq_t)35;  /* a(2) */
    v[3] = (seq_t)36;  /* a(3) */

    for (idx_t i = 4; i <= n; ++i) {
        /* a(i) = 36*a(i-2) - a(i-4) */
        seq_t ai = (seq_t)36 * v[(i - 2) & 3] - v[(i - 4) & 3];
        v[i & 3] = ai;
    }
    return v[n & 3];
}
