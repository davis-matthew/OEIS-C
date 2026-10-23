/*
 * A021187  Decimal expansion of 1/183.
 * https://oeis.org/A021187
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = the (n+1)-st digit in the decimal expansion of 1/183
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation via long division, O(n) time, no floating point */

/* The types seq_t and idx_t are defined by the build system. */
seq_t a(idx_t n)
{
    /* Remainder of the division after producing the previous digit */
    seq_t r = 1;              /* initial remainder: 1 / 183 */
    seq_t digit = 0;          /* the digit produced at the current step */

    for (idx_t i = 0; i <= n; ++i) {
        /* Perform the next division step using a wider intermediate type
           to avoid overflow when seq_t is 8 or 16 bits. */
        uint64_t t = (uint64_t)r * 10ULL;   /* r * 10 */
        digit = (seq_t)(t / 183ULL);        /* next digit (0‑9) */
        r     = (seq_t)(t % 183ULL);        /* new remainder (<183) */
    }

    return digit;   /* a(n) is the digit at position n+1 */
}
