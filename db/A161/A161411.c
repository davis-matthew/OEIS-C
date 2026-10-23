/*
 * A161411  First differences of A160410.
 * https://oeis.org/A161411
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) = 4 * 3^popcount(n-1)
 * Range:     u8 n <= 15, u16 n <= 511, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = 4 * 3^popcount(n-1),  n >= 1   */

seq_t a(idx_t n)
{
    if (n == 0) {
        /* Behavior below the offset is irrelevant, return 0. */
        return (seq_t)0;
    }

    idx_t x = n - 1;            /* Index used for popcount.   */
    unsigned pop = 0;           /* Number of set bits.         */

    /* Count bits in x (no built‑in popcount function). */
    while (x != 0) {
        pop += (unsigned)(x & 1U);
        x >>= 1;
    }

    seq_t result = 1;
    /* Multiply by 3, 'pop' times. */
    for (unsigned i = 0; i < pop; ++i)
        result *= (seq_t)3;

    /* Multiply by 4 to obtain the sequence value. */
    result *= (seq_t)4;

    return result;
}
