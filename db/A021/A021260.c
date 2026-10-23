/*
 * A021260  Decimal expansion of 1/256.
 * https://oeis.org/A021260
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   decimal digits of 1/256 (0.00390625) with trailing zeros
 * Range:     u8 n <= 94, u16 n <= 94, u32 n <= 94, u64 n <= 94, u128 n <= 94,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Returns the n-th decimal digit of 1/256; a(0)=0, a(1)=0, a(2)=3, a(3)=9,
   a(4)=0, a(5)=6, a(6)=2, a(7)=5, and a(n)=0 for all n≥8. */
seq_t a(idx_t n)
{
    /* Only the first 8 digits are non‑zero; compute them by long division. */
    if (n < 8) {
        unsigned long long r = 1;       /* remainder of 1 / 256 initially 1 */
        unsigned long long digit = 0;   /* current digit */

        for (idx_t i = 0; i <= n; ++i) {
            r *= 10ULL;                 /* bring down a zero */
            digit = r / 256ULL;         /* next decimal digit */
            r = r % 256ULL;             /* new remainder */
        }
        return (seq_t)digit;
    }
    /* The decimal expansion terminates after 8 digits; trailing digits are zero. */
    return 0;
}
