/*
 * A044249  Numbers k such that string 7,6 occurs in the base 8 representation
 *          of k but not of k-1.
 * https://oeis.org/A044249
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   has_substring_76
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Sequence A044249: numbers k such that the base‑8 representation of k contains
 * the substring “76” but the base‑8 representation of k−1 does not.
 * This implementation simply scans integers sequentially until the desired
 * term is found.  It works for any seq_t and idx_t defined by the build.
 */
#include <stdbool.h>
#include "../utils/has_substring_76.h"

/* Return the n‑th term of A044249 (1‑based index). */
seq_t a(idx_t n)
{
    if (n == (idx_t)0) return (seq_t)0;  /* offset not required */
    idx_t found = (idx_t)0;
    seq_t k = (seq_t)0;
    for (;;) {
        ++k;
        /* k-1 is safe because k ≥ 1 after the increment. */
        if (has_substring_76(k) && !has_substring_76(k - (seq_t)1)) {
            ++found;
            if (found == n) return k;
        }
    }
}
