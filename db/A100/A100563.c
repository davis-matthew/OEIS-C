/*
 * A100563  Number of bases less than sqrt(n) in which n is a palindrome.
 * https://oeis.org/A100563
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, base, nonn
 * Formula:   a(n) = number of bases b, 2 ≤ b < √n, for which n is a
 *            palindrome in base b
 * Range:     u8 n <= 292, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_pal_base
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation: brute‑force over all bases 2..⌊√n⌋−(if n is a perfect square) */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_pal_base.h"

seq_t a(idx_t n) {
    if (n < 3) return 0;   /* n = 1 or 2 → no bases < √n */

    /* Compute floor(√n). */
    idx_t low = 0, high = n, lim = 0;
    while (low <= high) {
        idx_t mid = low + (high - low) / 2;
        unsigned __int128 sq = (unsigned __int128)mid * mid;
        if (sq <= n) {
            lim = mid;
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }

    /* Exclude the exact square root if n is a perfect square. */
    if ((unsigned __int128)lim * lim == n && lim > 0) {
        lim--;
    }

    if (lim < 2) return 0;

    seq_t count = 0;
    for (idx_t base = 2; base <= lim; ++base) {
        if (is_pal_base(n, base)) {
            count++;
        }
    }
    return count;
}
