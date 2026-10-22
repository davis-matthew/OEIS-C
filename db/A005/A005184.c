/*
 * A005184  Self-contained numbers: odd numbers k whose Collatz sequence
 *          contains a higher multiple of k.
 * https://oeis.org/A005184
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   search over odd k = 1, 3, 5, ...; k is a term if its Collatz
 *            trajectory (stopping at 1) contains a value v > k with k | v
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 7, u64 n <= 7, u128 n <= 7,
 *            big any n
 * Helpers:   is_self_contained
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_self_contained.h"

static uint64_t found[64];
static idx_t nfound = 0;
static uint64_t next_k = 3;

seq_t a(idx_t n) {
    while (nfound < n && nfound < 64) {
        uint64_t k = next_k;
        next_k += 2;
        if (is_self_contained(k)) found[nfound++] = k;
    }
    if (n > nfound) return (seq_t)0;
    return (seq_t)found[n - 1];
}
