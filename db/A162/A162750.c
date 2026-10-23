/*
 * A162750  The a(n)-th (odd) binary palindrome is A162749(n).
 * https://oeis.org/A162750
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   Let k = ⌊log₂(n+1)⌋ (k ≥ 1 for n ≥ 1). * Let d = 1 << (k-1). *
 *            Then a(n) = n + d if (n – (2^k – 1)) < d, else a(n) = n – d. *
 *            This produces the self‑inverse permutation A162750
 * Range:     u8 n <= 68, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   floor_log2_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/floor_log2_uint64.h"

seq_t a(idx_t n)
{
    /* n ≥ 1 by definition of the sequence */
    idx_t k = floor_log2_uint64(n + 1);          /* k ≥ 1 */
    idx_t d = ((idx_t)1) << (k - 1);             /* half block size */
    idx_t start = ((idx_t)1 << k) - 1;           /* start of the block */
    idx_t offset = n - start;                    /* position in block */

    if (offset < d) {
        return (seq_t)(n + d);
    } else {
        return (seq_t)(n - d);
    }
}
