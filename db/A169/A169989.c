/*
 * A169989  Expansion of Product_{i=0..m-1} 1 + x^(2*i+1) for m=6.
 * https://oeis.org/A169989
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = coefficient of x^n in ∏_{i=0..5} (1 + x^(2*i+1))
 * Range:     u8 n <= 36, u16 n <= 36, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   init_table
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/init_table.h"

seq_t a(idx_t n)
{
    seq_t *const tbl = init_table();
    if (n < 37) {
        return tbl[n];
    } else {
        return 0;
    }
}
