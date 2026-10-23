/*
 * A033067  Numbers whose base-16 representation Sum_{i=0..m} d(i)*16^i has
 *          odd d(m-i) for all odd i.
 * https://oeis.org/A033067
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 65, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   is_a033067
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A033067
 * Numbers whose base‑16 representation Sum_{i=0..m} d(i)*16^i
 * has odd d(m-i) for all odd i.
 *
 * The function returns the n‑th term (n≥1) of the sequence.
 * It is a straightforward O(n·average‑digits) generator;
 * no memoisation is required for n≤~10000 (runtime <<2 s).
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_a033067.h"

/* Public sequence function */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;  /* undefined for n<1, but safe to return 0 */

    idx_t found = 0;
    seq_t x = (seq_t)1;

    while (1) {
        if (is_a033067(x)) {
            ++found;
            if (found == n)
                return x;
        }
        ++x;
    }
}
