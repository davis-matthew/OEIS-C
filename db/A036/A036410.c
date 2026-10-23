/*
 * A036410  G.f.: (1+x^6)/((1-x)*(1-x^3)*(1-x^4)).
 * https://oeis.org/A036410
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ceil((n+1)^2/12)
 * Range:     u8 n <= 14, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Equivalent recurrence: a(n) = a(n-1) + a(n-2) - a(n-4) - a(n-5) + a(n-6) */
/* Initial values: a(0)=1, a(1)=1, a(2)=1, a(3)=2, a(4)=3, a(5)=3 */

seq_t a(idx_t n)
{
    /* compute m = (n+1)^2 as seq_t */
    seq_t m = (seq_t)n + 1;
    m = m * m;

    /* compute ceil(m / 12) without overflow */
    seq_t q = m / 12;
    if (m % 12 != 0) { q = q + 1; }

    return q;
}
