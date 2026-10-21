/*
 * A095250  a(n) = 11111111... (n times) = (10^n-1)/9 reduced mod n.
 * https://oeis.org/A095250
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (10^n - 1)/9 mod n = 111…1 (n ones) mod n
 * Range:     u8 n <= 278, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Computes A095250:  a(n) = 111…1 (n ones) reduced modulo n.
 * Implementation uses a linear scan that repeatedly builds the repunit
 * digit by digit, taking the remainder modulo n at each step.
 * This is O(n) per call and requires no precomputation or memoisation.
 *
 * The function works for any n ≥ 1. It uses only integer arithmetic
 * and is safe for all supported seq_t sizes.
 */
seq_t a(idx_t n)
{
    if (n == 0)      /* safeguard – offset starts at 1, but handle gracefully */
        return 0;
    seq_t rem = 0;
    for (idx_t i = 0; i < n; ++i)
        rem = (rem * 10 + 1) % (seq_t)n;
    return rem;
}
