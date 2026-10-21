/*
 * A052212  Numbers k whose square contains the digits of k in order (but not
 *          necessarily consecutively).
 * https://oeis.org/A052212
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 18, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   seq_to_str
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth integer k whose square (in base 10) contains the digits of k in order */
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "../utils/seq_to_str.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    idx_t count = 0;
    for (idx_t k = 1; ; ++k) {
        seq_t k_seq = (seq_t)k;
        seq_t square = k_seq * k_seq;

        /* Convert k and its square to decimal strings */
        /* Buffers sized generously for arbitrary‑precision seq_t */
        char buf_k[256];
        char buf_sq[512];
        size_t len_k, len_sq;
        seq_to_str(k_seq, buf_k, &len_k);
        seq_to_str(square, buf_sq, &len_sq);

        /* Check whether buf_k is a subsequence of buf_sq */
        size_t i = 0;
        for (size_t j = 0; j < len_sq && i < len_k; ++j) {
            if (buf_sq[j] == buf_k[i]) ++i;
        }
        if (i == len_k) {
            ++count;
            if (count == n) return k_seq;
        }
    }
}
