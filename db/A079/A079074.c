/*
 * A079074  Sum of numbers < n having in binary representation the same number
 *          of 0's and 1's as n.
 * https://oeis.org/A079074
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{0 ≤ m < n, bitlen(m)=bitlen(n) ∧
 *            popcount(m)=popcount(n)} m
 * Range:     u8 n <= 49, u16 n <= 848, u32 n <= 9318, u64 n <= 9318,
 *            u128 n <= 9318, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function returns the sum of all numbers less than n whose binary
 * representation has the same number of 1's and the same length as n. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    unsigned len_n   = 64 - __builtin_clzll((uint64_t)n);
    unsigned ones_n  = __builtin_popcountll((uint64_t)n);

    seq_t sum = 0;
    if (len_n > 0) {
        /* smallest m with the same bit‑length as n */
        idx_t start = (idx_t)1ULL << (len_n - 1);
        for (idx_t m = start; m < n; ++m) {
            if (__builtin_popcountll((uint64_t)m) == ones_n)
                sum += (seq_t)m;
        }
    }
    return sum;
}
