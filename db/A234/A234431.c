/*
 * A234431  Numbers that are the sum of 2 successive evil numbers (A001969).
 * https://oeis.org/A234431
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = A001969(n) + A001969(n + 1) (offset 1)
 * Range:     u8 n <= 64, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   _evil_parity_even
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/_evil_parity_even.h"

seq_t a(idx_t n)
{
    /* Find the (n‑1)‑th and n‑th evil numbers (1‑based offset). */
    idx_t target_prev = n - 1;   /* index of the previous evil number */
    idx_t target_next = n;       /* index of the next evil number (current) */

    idx_t cnt = 0;               /* number of evil numbers seen so far */
    seq_t prev_ev = 0;           /* (n‑1)‑th evil number */
    seq_t next_ev = 0;           /* n‑th evil number */

    seq_t val = 0;               /* candidate integer to test */

    while (1) {
        if (_evil_parity_even(val)) {
            if (cnt == target_prev) prev_ev = val;
            if (cnt == target_next) {
                next_ev = val;
                break;
            }
            cnt++;
        }
        val++;
    }
    return prev_ev + next_ev;
}
