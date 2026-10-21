/*
 * A082908  Largest value of gcd(2^n, binomial(n,j)) with j=0..n-1; maximum
 *          value of largest power of 2 dividing binomial(n,j) in the n-th row
 *          of Pascal's triangle.
 * https://oeis.org/A082908
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^⌊log₂( odd_part(n+1) )⌋ where odd_part(k) is k with all
 *            factors of 2 removed. This follows from the OEIS recurrence
 *            a(n-1)=2^⌊log₂(A000265(n))⌋
 * Range:     u8 n <= 255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function works for any idx_t and seq_t defined by the build. */
seq_t a(idx_t n) {
    /* 1. Compute odd part of n+1. */
    idx_t odd = n + 1;
    odd >>= __builtin_ctzll((unsigned long long)odd);      /* remove trailing zeros */

    /* 2. Find the position of the most significant 1‑bit in odd. */
    idx_t msb = 63 - __builtin_clzll((unsigned long long)odd); /* guaranteed odd>0 */

    /* 3. Return 2^msb as appropriate seq_t. */
    return ((seq_t)1 << msb);
}
