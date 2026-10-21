/*
 * A056539  Self-inverse permutation: reverse the bits in binary expansion of
 *          n and also complement them (0->1, 1->0) if the run count (A005811)
 *          is even.
 * https://oeis.org/A056539
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn, look
 * Formula:   not stated
 * Range:     u8 n <= 255, u16 n <= 8191, u32 n <= 8191, u64 n <= 8191,
 *            u128 n <= 8191, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A056539 – self‑inverse permutation
   a(n) = reverse_bits(n)          if number_of_runs(n) is odd
   a(n) = complement(reverse_bits(n)) if number_of_runs(n) is even
   where number_of_runs(n) is the number of maximal runs of equal bits
   in the binary expansion of n.
   The implementation works for both fixed‑width and arbitrary‑precision
   seq_t types. */
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Special case: n == 0 */
    if (n == 0)
        return (seq_t)0;

    /* Compute the bit length of n */
    uint64_t m = (uint64_t)n;          /* n fits in 64 bits (idx_t is uint64_t) */
    int bitlen = 64 - __builtin_clzll(m);   /* number of bits in m */

    /* Count number of runs in the binary representation of n */
    int run_count = 1;
    int prev_bit = (m >> (bitlen - 1)) & 1;
    for (int i = bitlen - 2; i >= 0; --i) {
        int bit = (m >> i) & 1;
        if (bit != prev_bit) {
            ++run_count;
            prev_bit = bit;
        }
    }

    /* Reverse the bits of n */
    uint64_t rev = 0;
    for (int i = 0; i < bitlen; ++i)
        rev = (rev << 1) | ((m >> i) & 1);

    /* If run_count is even, complement the reversed bits */
    if ((run_count & 1) == 0) {
        uint64_t mask = (bitlen == 64) ? ~((uint64_t)0) : ((uint64_t)1 << bitlen) - 1;
        rev ^= mask;
    }

    return (seq_t)rev;
}
