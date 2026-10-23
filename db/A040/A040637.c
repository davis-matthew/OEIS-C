/*
 * A040637  Continued fraction for sqrt(663).
 * https://oeis.org/A040637
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy, mult, less
 * Formula:   a(0) = 25. For n > 0, let e = v₂(n) (the exponent of 2 in n): •
 *            if e == 0 (n odd) → a(n) = 1 • if e == 1 (n ≡ 2 (mod 4))→ a(n) =
 *            2 • if e ≥ 2 (n ≡ 0 (mod 4))→ a(n) = 50 This follows from the
 *            multiplicative rule: a(2) = 2, a(2ᵉ) = 50 for e ≥ 2, a(pᵉ) = 1
 *            for odd primes
 * Range:     u8 n <= 83, u16 n <= 83, u32 n <= 83, u64 n <= 83, u128 n <= 83,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A040637: 25, 1, 2, 1, 50, 1, 2, 1, 50, … */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)25;                 /* special value */
    if ((n & 1) != 0) return (seq_t)1;            /* odd numbers */
    if ((n & 3) == 2) return (seq_t)2;            /* exactly one factor 2 */
    return (seq_t)50;                              /* divisible by 4 */
}
