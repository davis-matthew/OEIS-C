/*
 * A044075  Numbers k such that the string 3,2 occurs in the base-4
 *          representation of k but not of k-1.
 * https://oeis.org/A044075
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 20, u16 n <= 3976, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   has32_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS A044075: numbers k such that the base‑4 representation
   contains the substring 3,2 and k−1 does not.  The function uses a
   simple brute‑force scan over successive integers. */
#include <stdbool.h>
#include <stdint.h>
#include "../utils/has32_uint64.h"

seq_t a(idx_t n) {
    idx_t count = 0;
    uint64_t k = 1;
    while (1) {
        if (has32_uint64(k) && !has32_uint64(k - 1)) {
            ++count;
            if (count == n)
                return (seq_t)k;
        }
        ++k;
    }
}
