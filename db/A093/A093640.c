/*
 * A093640  Number of divisors of n whose binary representation is contained
 *          in that of n.
 * https://oeis.org/A093640
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = number of divisors d of n such that the binary *
 *            representation of d occurs as a contiguous substring of the
 *            binary * representation of n
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   contains_bin, to_bin
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/contains_bin.h"
#include "../utils/to_bin.h"

/* Determine a(n) for n >= 1 */
seq_t a(idx_t n)
{
    uint64_t N = (uint64_t)n;          /* work in native 64‑bit */
    char n_bin[65];
    int n_len;
    to_bin(N, n_bin, &n_len);

    seq_t count = 0;
    for (uint64_t i = 1; i * i <= N; ++i) {
        if (N % i == 0) {
            uint64_t d1 = i;
            uint64_t d2 = N / i;
            if (contains_bin(d1, n_bin, n_len))
                count++;
            if (d1 != d2 && contains_bin(d2, n_bin, n_len))
                count++;
        }
    }
    return count;
}
