/*
 * A152672  a(n) is the number of distinct tuples of up to k bit locations in
 *          L-bit words, in which, if bits are perturbed, the resulting change
 *          in unsigned L-bit value is n, for L=8 and k=7.
 * https://oeis.org/A152672
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, nonn
 * Formula:   a(n) = number of subsets T ⊆ {0..7} with 1 ≤ |T| ≤ 7 such that
 *            there exists a choice of signs ε_j ∈ {±1} for j ∈ T where
 *            Σ_{j∈T} ε_j·2^j equals ±n. The sequence is indexed starting at
 *            a(1)
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Precomputed table: counts[n] holds a(n) for 1 ≤ n ≤ 254. */
    static seq_t counts[255];
    static bool ready = false;

    if (!ready) {
        /* Initialise counts to zero. */
        for (int i = 0; i < 255; ++i) counts[i] = 0;

        /* Iterate over all non‑empty 8‑bit subsets of size ≤ 7. */
        for (int mask = 1; mask < 255; ++mask) {      /* mask==255 (all 8 bits) is excluded */
            /* Determine the indices of bits set in mask. */
            int bits[8];
            int k = 0;
            for (int j = 0; j < 8; ++j)
                if (mask & (1 << j))
                    bits[k++] = j;

            int max_assign = 1 << k;                 /* number of sign assignments */
            bool seen[255] = {0};                    /* per‑subset marker for n */

            for (int assign = 0; assign < max_assign; ++assign) {
                int sum = 0;
                for (int idx = 0; idx < k; ++idx)
                    if (assign & (1 << idx))
                        sum += 1 << bits[idx];
                    else
                        sum -= 1 << bits[idx];

                int abs_val = sum < 0 ? -sum : sum;
                if (abs_val >= 1 && abs_val <= 254 && !seen[abs_val]) {
                    counts[abs_val] += 1;
                    seen[abs_val] = true;
                }
            }
        }
        ready = true;
    }

    if (n < 1 || n > 254) return 0;
    return counts[(int)n];
}
