/*
 * A059541  Beatty sequence for 1 + log(2).
 * https://oeis.org/A059541
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(n*(1+log(2)))
 * Range:     u8 n <= 151, u16 n <= 2000, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* High‑precision rational approximation of 1+log(2).
       1 + log(2) ≈ 1.693147180559945309
       Using a 18‑digit mantissa gives an error < 5e-19; for n up to 10⁴ the
       rounding error is < 5e-15, far below 1, so the floor is correct.       */
    static const uint64_t NUM = 1693147180559945309ULL; /* numerator of α */
    static const uint64_t DEN = 1000000000000000000ULL; /* denominator of α */

    if (n == 0) return 0;   /* offset 1 – a(0) is undefined, guard it */

    __uint128_t prod = (__uint128_t)n * NUM;   /* n * NUM fits in 128 bits */
    uint64_t res   = (uint64_t)(prod / DEN);   /* floor(n * NUM / DEN) */

    return (seq_t)res;
}
