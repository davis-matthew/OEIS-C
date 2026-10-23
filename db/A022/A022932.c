/*
 * A022932  a(n) is the number of powers Pi^m between e^n and e^(n+1).
 * https://oeis.org/A022932
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor((n+1)/ln(pi)) - floor(n/ln(pi)) (a(n) is 0 if no
 *            integer m satisfies e^n ≤ π^m < e^(n+1), 1 otherwise. The
 *            implementation uses only integer arithmetic. ln(pi) ≈
 *            1144729885849400174 / 10^18, an error < 10^-18
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* special case n = 0 (e^0=1, e^1≈2.718, no π^m with exponent ≥1) */
    if (n == 0)
        return (seq_t)0;

    /* high‑precision rational approximation of ln(pi) */
    const unsigned __int128 DENOM = (unsigned __int128)1000000000000000000ULL;   /* 10^18 */
    const unsigned __int128 LNPI_LOW = (unsigned __int128)1144729885849400174ULL;/* floor(ln(pi)*DENOM) */
    const unsigned __int128 LNPI_HIGH = LNPI_LOW + (unsigned __int128)1;         /* upper bound */

    unsigned __int128 n128 = (unsigned __int128)n;

    /* m_candidate = floor((n+1)/ln(pi)) using the upper bound LNPI_HIGH
       ensures m_candidate ≤ the true floor value.                 */
    unsigned __int128 m128 = ((n128 + (unsigned __int128)1) * DENOM) / LNPI_HIGH;
    if (m128 == 0)          /* would correspond to π^0, which is not counted */
        return (seq_t)0;

    /* Test the integer inequalities using bracketed bounds */
    unsigned __int128 left  = m128 * LNPI_LOW;                     /* m * ln(pi) ≥ n  */
    unsigned __int128 rmin  = n128 * DENOM;
    unsigned __int128 rmax  = (n128 + (unsigned __int128)1) * DENOM;

    if (left >= rmin && m128 * LNPI_HIGH < rmax)
        return (seq_t)1;          /* exists at least one integer m with π^m in the interval */
    else
        return (seq_t)0;
}
