/*
 * A010449  Squares mod 88.
 * https://oeis.org/A010449
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = 88*floor((n-1)/18) + residues[(n-1) mod 18] where
 *            residues are the quadratic residues modulo 88 in ascending order
 * Range:     u8 n <= 18, u16 n <= 18, u32 n <= 18, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    static seq_t residues[18];
    static bool initialized = false;
    if (!initialized) {
        bool seen[88];
        for (int i = 0; i < 88; ++i) seen[i] = false;
        for (idx_t x = 0; x < 88; ++x) {
            uint64_t sq = (uint64_t)x * (uint64_t)x;
            seen[(int)(sq % 88ULL)] = true;
        }
        int pos = 0;
        for (int r = 0; r < 88; ++r) {
            if (seen[r]) {
                residues[pos++] = (seq_t)r;
            }
        }
        initialized = true;
    }

    idx_t k = (n - 1) / 18;
    idx_t r = (n - 1) % 18;
    return (seq_t)k * 88 + residues[(size_t)r];
}
