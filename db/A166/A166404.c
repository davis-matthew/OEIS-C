/*
 * A166404  Self-inverse permutation of natural numbers obtained by exchanging
 *          the run lengths of adjacent runs of ones and zeros in the binary
 *          expansion of n, starting from the most significant run of 1's.
 *          (See the example lines).
 * https://oeis.org/A166404
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = integer obtained by swapping the run lengths of adjacent
 *            runs of 1's and 0's in the binary representation of n (starting
 *            with a run of 1's)
 * Range:     u8 n <= 255, u16 n <= 9272, u32 n <= 9272, u64 n <= 9272,
 *            u128 n <= 9272, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Compute A166404 for a given n. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    /* Even for 64‑bit n the number of runs never exceeds 64. */
    unsigned int run_len[65];
    int run_cnt = 0;

    /* Current bit value of the run being counted (start with 1). */
    unsigned int cur_bit = 1;

    /* Find index of the most significant 1 bit (0‑based). */
    int msb = 63 - __builtin_clzll(n);
    int pos = msb;

    /* Scan the bits from most significant to least and record run lengths. */
    while (pos >= 0) {
        unsigned int len = 0;
        while (pos >= 0 && ((n >> pos) & 1u) == cur_bit) {
            ++len;
            --pos;
        }
        run_len[run_cnt++] = len;
        cur_bit ^= 1u;          /* switch between 1 and 0 */
    }

    seq_t result = 0;
    int used_bits = 0;
    int bits_in_seq = sizeof(seq_t) * 8;  /* 8, 16, 32, 64 or 128 */

    /* Reconstruct the new number using the swapped run lengths. */
    for (int i = 0; i < run_cnt; ++i) {
        unsigned int len;
        if ((i & 1) == 0) {          /* even index – 1's run */
            if (i + 1 < run_cnt)
                len = run_len[i + 1];
            else
                len = run_len[i];
        } else {                     /* odd index – 0's run */
            len = run_len[i - 1];
        }

        if (len == 0)
            continue;

        if (used_bits > 0)
            result <<= len;          /* make room for the new run */
        used_bits += len;

        if ((i & 1) == 0) {          /* run of 1's – add ones */
            seq_t mask;
            if (len == bits_in_seq)        /* all ones, avoid shift by width */
                mask = (seq_t)-1;
            else
                mask = ((seq_t)1 << len) - 1;
            result |= mask;
        }
        /* zero runs contribute no bits beyond the shift */
    }

    return result;
}
