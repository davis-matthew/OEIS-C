/*
 * A087303  Smallest nontrivial multiple of n whose digit sum is the same as
 *          that of the digit sum of n. By nontrivial one means a(n) is not
 *          equal to n or (10^k)*n. 0 if no such number exists.
 * https://oeis.org/A087303
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 30, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   digit_sum, is_power_of_10_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A087303: a(n) = smallest non‑trivial multiple of n with the same digit sum,
   0 if n is a power of 10. */
#include <stdbool.h>
#include "../utils/digit_sum.h"
#include "../utils/is_power_of_10_idx.h"

seq_t a(idx_t n)
{
    /* Powers of 10 have no solution. */
    if (is_power_of_10_idx(n)) return 0;

    const seq_t target_sum = digit_sum((seq_t)n);

    /* Search for a non‑trivial multiple m·n with the required digit sum. */
    for (idx_t m = 2;; ++m) {
        /* Skip multiples that are powers of 10. */
        if (is_power_of_10_idx(m)) continue;

        unsigned __int128 prod128 = (unsigned __int128)n * m;
        seq_t prod = (seq_t)prod128;  /* safe: the answer is guaranteed to fit in seq_t. */

        if (digit_sum(prod) == target_sum) return prod;
    }
}
