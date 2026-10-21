/*
 * A087304  Smallest nontrivial multiple of n whose nonzero digit product is
 *          the same as that of the nonzero digit product of n. By nontrivial
 *          one means a(n) is not equal to n or (10^k)*n. 0 if no such number
 *          exists.
 * https://oeis.org/A087304
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, look, nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 20, u32 n <= 206, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digit_prod_idx, is_power_of_10_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A087304: smallest nontrivial multiple of n whose product of
   non‑zero digits equals that of n.
   The function returns 0 if no such multiple exists (the
   conjecture asserts this never happens). */

#include <stdbool.h>
#include "../utils/digit_prod_idx.h"
#include "../utils/is_power_of_10_idx.h"

/* Return true iff x = 10^k for some k >= 0. */

seq_t a(idx_t n)
{
    seq_t target = digit_prod_idx(n);          /* product of non‑zero digits of n */

    for (idx_t k = 2; ; ++k) {
        if (is_power_of_10_idx(k))               /* skip pure powers of 10 */
            continue;

        /* compute product of digits of m = n * k */
        seq_t prod_m = digit_prod_idx(n * k);
        if (prod_m == target) {
            return (seq_t)(n * k);
        }
    }
}
