/*
 * A133009  One defining property of the sequences {A, B} = {A000069, A001969}
 *          is that they are the unique pair of sets complementary with
 *          respect to the nonnegative integers such that q(n) = |{x : x, y in
 *          A, x < y, x + y = n}| = |{x : x, y in B, x < y, x + y = n}| for
 *          all n >= 0. The present sequence gives...
 * https://oeis.org/A133009
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of unordered pairs (x < y) of integers with even
 *            binary * weight (i.e., parity of the number of 1‑bits) such that
 *            x + y = n. * This equals the common pair count for the
 *            complementary sets A and B, * which are the even‑parity and
 *            odd‑parity integers respectively
 * Range:     u8 n <= 1022, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   even_parity_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* required external types */
#include <stdint.h>
#include "../utils/even_parity_idx.h"

seq_t a(idx_t n)
{
    seq_t total = 0;

    for (idx_t k = 0; k <= n; ++k) {
        if (even_parity_idx(k) && even_parity_idx(n - k))
            total += 1;
    }

    /* subtract the diagonal term when n is even and n/2 has even parity */
    if ((n & 1ULL) == 0) {
        idx_t h = n >> 1;
        if (even_parity_idx(h))
            total -= 1;
    }

    /* each unordered pair was counted twice (k, n-k) and (n-k, k) */
    return total >> 1;
}
