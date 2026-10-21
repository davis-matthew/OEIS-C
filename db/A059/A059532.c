/*
 * A059532  Beatty sequence for 1 + Pi.
 * https://oeis.org/A059532
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n + ⌊n⋅π⌋ , using the rational approximation π ≈ 3.141592
 *            (which is strictly less than π). For n ≤ 10000 this guarantees
 *            that ⌊n⋅π⌋ computed with 3.141592 equals ⌊n⋅π⌋ exactly
 * Range:     u8 n <= 61, u16 n <= 2000, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* if called with n = 0 (outside the offset) return 0 */
    if (n == 0) return 0;

    /* use a 64‑bit intermediate to avoid overflow on 32‑bit seq_t */
    uint64_t tmp = (uint64_t)n * 3141592ULL;      /* numerator * n  */
    seq_t floor_pi = tmp / 1000000ULL;           /* divide by 10⁶  */

    return (seq_t)n + floor_pi;
}
