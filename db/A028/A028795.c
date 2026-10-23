/*
 * A028795  Nonsquares mod 82.
 * https://oeis.org/A028795
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = 82 * floor((n-1)/40) + u[ (n-1) mod 40 ], where u is the
 *            increasing list of residues 0 ≤ r < 82 that are not quadratic
 *            residues modulo 82
 * Range:     u8 n <= 40, u16 n <= 40, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>      /* for size_t */

seq_t a(idx_t n)
{
    /* elements that are not quadratic residues modulo 82; 40 of them */
    static seq_t unsquares[40];
    static bool init = false;

    if (!init) {
        /* square residues modulo 82 */
        bool is_res[82] = { false };
        for (idx_t x = 0; x < 82; ++x) {
            idx_t r = (idx_t)(((seq_t)x * (seq_t)x) % (seq_t)82);
            is_res[r] = true;
        }
        /* collect non‑residues in ascending order */
        size_t pos = 0;
        for (idx_t r = 0; r < 82; ++r) {
            if (!is_res[r]) {
                unsquares[pos++] = (seq_t)r;
            }
        }
        /* pos should be 40 */
        (void)pos;  /* silence unused warning if static build has no asserts */
        init = true;
    }

    idx_t idx = n - 1;                    /* zero‑based index */
    idx_t cycle = idx / 40;               /* full blocks of 40 non‑residues */
    idx_t offset = idx % 40;              /* position inside one block    */

    return (seq_t)82 * (seq_t)cycle + unsquares[offset];
}
