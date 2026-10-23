/*
 * A132140  Numbers containing no zeros in ternary representation and with an
 *          initial 1.
 * https://oeis.org/A132140
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 31, u16 n <= 1023, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A132140 –  Numbers with ternary digits only 1 and 2, starting with 1.
   Each term is obtained by taking n‑1 in binary, treating each binary bit as a
   ternary digit (0→1, 1→2) and prefixing a leading 1.  Thus a(n) is the
   number whose ternary representation has a leading 1 followed by the
   digits derived from the bits of n‑1. */

#include <stdint.h>
#include <stdbool.h>

/* floor log2 for idx_t (uint64_t) */
static inline int floor_log2_idx(idx_t v) {
    int r = 0;
    while (v >>= 1) r++;
    return r;
}

seq_t a(idx_t n) {
    /* n >= 1 */
    int k = floor_log2_idx(n) + 1;              /* length of ternary string */
    idx_t cumulative_before = ((idx_t)1 << (k - 1)) - 1; /* terms before this block */
    idx_t m = n - cumulative_before - 1;       /* 0‑based offset in block */

    seq_t result = 1;                           /* leading digit is 1 */
    for (int i = k - 2; i >= 0; --i) {
        seq_t digit = ((m >> i) & 1) ? 2 : 1;
        result = result * 3 + digit;
    }
    return result;
}
