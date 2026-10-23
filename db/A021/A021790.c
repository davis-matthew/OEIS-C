/*
 * A021790  Decimal expansion of 1/786.
 * https://oeis.org/A021790
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = floor(10^(n+1)/786) - floor(10^n/786)
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*   (= the n-th decimal digit of 1/786, 0‑based) */
seq_t a(idx_t n)
{
    /* Simulate long division of 1 by 786. */
    const uint64_t denom = 786;            /* fixed denominator */
    uint64_t rem = 1;                      /* remainder initially 1 (numerator) */
    uint64_t digit = 0;
    for (idx_t i = 0; i <= n; ++i) {
        uint64_t prod = rem * 10ULL;        /* 10 * remainder */
        digit = prod / denom;               /* next decimal digit */
        rem   = prod % denom;               /* new remainder for next step */
    }
    return (seq_t)digit;                   /* digit is 0‑9, fits seq_t */
}
