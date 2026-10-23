/*
 * A021805  Decimal expansion of 1/801.
 * https://oeis.org/A021805
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons
 * Formula:   a(n) = nth digit after the decimal point of 1/801 (0‑indexed;
 *            a(0) is the first digit after the decimal point)
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* The remainder before the first digit. */
    idx_t r = 1;          /* 1 < 801, always fits in uint64_t */

    seq_t d = 0;          /* the digit at the current position */

    /* Perform the long division for n+1 steps: the loop leaves d as
       the digit at position n. */
    for (idx_t i = 0; i <= n; ++i) {
        r *= 10ULL;                 /* bring down a zero */
        d = (seq_t)(r / 801ULL);    /* next digit */
        r %= 801ULL;                /* new remainder */
    }

    return d;
}
