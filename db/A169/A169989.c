/*
 * A169989  Expansion of Product_{i=0..m-1} 1 + x^(2*i+1) for m=6.
 * https://oeis.org/A169989
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = coefficient of x^n in ∏_{i=0..5} (1 + x^(2*i+1))
 * Range:     u8 n <= 36, u16 n <= 36, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Validated: full, pass, 37 terms through n = 36 (2026-11-02)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

/* Lazy initialization of the coefficient table for 0 ≤ n ≤ 36. */
static seq_t *init_table(void)
{
    static bool done = false;
    static seq_t table[37];

    if (!done) {
        /* Generate all 2^6 = 64 subsets of {1,3,5,7,9,11}. */
        for (idx_t mask = 0; mask < 64; ++mask) {
            idx_t s = 0;
            for (idx_t bit = 0; bit < 6; ++bit) {
                if (mask & (1ULL << bit)) {
                    s += 2 * bit + 1; /* add the corresponding odd power */
                }
            }
            table[s] += 1;
        }
        done = true;
    }
    return table;
}

seq_t a(idx_t n)
{
    seq_t *const tbl = init_table();
    if (n < 37) {
        return tbl[n];
    } else {
        return 0;
    }
}
