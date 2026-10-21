/*
 * A224915  a(n) = Sum_{k=0..n} n XOR k where XOR is the bitwise logical
 *          exclusive-or operator.
 * https://oeis.org/A224915
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..n} (n XOR k)
 * Range:     u8 n <= 15, u16 n <= 255, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t np1 = (seq_t)n + 1;          /* count of numbers 0..n inclusive */
    seq_t result = 0;

    for (unsigned i = 0; i < 64; ++i) {
        seq_t bit = (seq_t)1 << i;     /* 2^i */
        if (bit > (seq_t)n) break;     /* no further bits can be set in k or n */

        seq_t ones;
        if (i + 1 < 64) {
            /* Full periods of size 2^(i+1) */
            seq_t full = np1 >> (i + 1);
            seq_t mask = ((seq_t)1 << (i + 1)) - 1;
            seq_t rem = np1 & mask;    /* remaining numbers after full periods */

            ones = full * bit + (rem > bit ? rem - bit : 0);
        } else { /* i == 63: period would be 2^64, which exceeds np1 for all n */
            seq_t rem = np1;
            ones = (rem > bit) ? rem - bit : 0;
        }

        seq_t n_bit = ((seq_t)n >> i) & 1;     /* 1 if bit i of n is set */
        seq_t count_diff = n_bit ? (np1 - ones) : ones;
        result += count_diff * bit;
    }

    return result;
}
