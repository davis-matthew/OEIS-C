/*
 * A220370  Number of nonzero squares that appear as a subsequences in the
 *          binary digits of n.
 * https://oeis.org/A220370
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, look
 * Formula:   a(n) = number of nonzero squares that appear as subsequences in
 *            the binary digits of n. For each non‑empty subsequence of the
 *            binary representation of n, interpret the digits as a binary
 *            integer (leading zeros allowed). Count the subsequences whose
 *            value is a perfect square > 0; each occurrence i...
 * Range:     u8 n <= 63, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n) {
    /* compute length L of the binary representation of n */
    idx_t tmp = n;
    idx_t L = 0;
    while (tmp) {
        ++L;
        tmp >>= 1;
    }
    if (L == 0) return 0;          /* n = 0 (should not happen in the domain) */

    /* number of non‑empty masks = 2^L - 1;  L is small (e.g. <= 20 for n <= 10^6) */
    uint64_t subCount = 1ULL << L; /* L < 64 for the intended test range */
    seq_t count = 0;

    /* iterate over all non‑empty subsequence masks */
    for (uint64_t mask = 1; mask < subCount; ++mask) {
        seq_t val = 0;
        /* reconstruct the binary value of this subsequence */
        for (idx_t i = 0; i < L; ++i) {
            if (mask >> i & 1) {
                idx_t bit = (n >> (L - 1 - i)) & 1;
                val = (val << 1) | (seq_t)bit;
            }
        }
        if (val == 0) continue;           /* ignore zero */
        seq_t r = floor_sqrt(val);
        if (r * r == val) count++;       /* non‑zero perfect square */
    }
    return count;
}
