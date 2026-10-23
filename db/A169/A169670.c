/*
 * A169670  Decimal expansion of 9099999999923/27500000000000.
 * https://oeis.org/A169670
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy, nice
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 none, u64 n <= 1000, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A169670: decimal expansion of 9099999999923/27500000000000.
   a(n) gives the (n+1)-th decimal digit after the point. */

/* NOTE:  For n ≥ 0, a(n) is the digit at position n in
   0.3309090909062909090...  (digits are 0‑based). */

seq_t a(idx_t n)
{
    /* Denominator and numerator fit in 64‑bit unsigned. */
    static const seq_t denom = (seq_t)27500000000000ULL;  /* 27,500,000,000,000 */
    static const seq_t numer = (seq_t)9099999999923ULL;   /* 9,099,999,999,923 */

    seq_t r = numer % denom;         /* initial remainder */
    for (idx_t i = 0; i <= n; ++i) {
        r = r * (seq_t)10;             /* shift left by one decimal place */
        seq_t d = r / denom;           /* current digit */
        r = r % denom;                 /* new remainder */
        if (i == n) return d;          /* return requested digit */
    }
    /* Unreachable, but satisfies compiler */
    return (seq_t)0;
}
