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
 * Validated: partial, pass, 7 terms through n = 7 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static bool is_self_contained(uint64_t k) {
    uint64_t c = k;
    while (c != 1) {
        if (c & 1) {
            if (c > (UINT64_MAX - 1) / 3) return false;
            c = 3 * c + 1;
        } else {
            c >>= 1;
        }
        if (c > k) {
            if (c <= 0xFFFFFFFFULL) {
                if ((uint32_t)c % (uint32_t)k == 0) return true;
            } else if (c % k == 0) {
                return true;
            }
        }
    }
    return false;
}

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
