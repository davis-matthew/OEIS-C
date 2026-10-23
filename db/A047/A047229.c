/*
 * A047229  Numbers that are congruent to {0, 2, 3, 4} mod 6.
 * https://oeis.org/A047229
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*floor((n-1)/4) + {0,2,3,4} where the added value
 *            depends on (n-1) mod 4
 * Range:     u8 n <= 171, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A047229: numbers that are congruent to {0,2,3,4} mod 6. */
seq_t a(idx_t n)
{
    /* compute quotient and remainder of (n-1) divided by 4 */
    idx_t q = (n - 1) / 4;
    idx_t r = (n - 1) % 4;

    /* base value is 6 * q */
    seq_t base = (seq_t)6 * (seq_t)q;

    /* add the appropriate offset */
    switch (r) {
        case 0: return base;          /* +0 */
        case 1: return base + (seq_t)2;/* +2 */
        case 2: return base + (seq_t)3;/* +3 */
        case 3: return base + (seq_t)4;/* +4 */
        default: return base;          /* should never happen */
    }
}
