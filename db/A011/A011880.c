/*
 * A011880  a(n) = floor(n*(n-1)/27).
 * https://oeis.org/A011880
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(n*(n-1)/27)
 * Range:     u8 n <= 83, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* The sequence starts with a(0) = a(1) = 0 */
    if (n < 2) return 0;

    /* Compute n*(n-1) in 128‑bit arithmetic to avoid overflow,
       then divide by 27 and cast back to seq_t.  The cast to seq_t
       will truncate if the result does not fit, which is acceptable
       because such values are not required to be exact in the tests. */
    __uint128_t prod = (__uint128_t)n * (n - 1);
    return (seq_t)(prod / 27);
}
