/*
 * A043125  Numbers k such that 2 and 3 occur juxtaposed in the base-6
 *          representation of k but not of k-1.
 * https://oeis.org/A043125
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   has_23
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Computes A043125: numbers k such that base‑6 digits of k contain a
 *   2 and a 3 adjacent in either order, but base‑6 digits of k‑1 do not.
 *   The function searches increasing k until the requested index is
 *   reached.  No formula is known, so a direct scan is used.
 */
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/has_23.h"

/* memoised sequence storage */
static seq_t *seq = NULL;    /* array of found values */
static size_t seq_cap = 0;   /* allocated capacity */
static size_t seq_len = 0;   /* number of values stored     */
static seq_t last_k = 0;     /* highest k examined so far   */

seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* offset starts at 1 */

    while (seq_len < (size_t)n)
    {
        ++last_k;
        if (has_23(last_k) && !has_23(last_k - 1))
        {
            if (seq_len >= seq_cap)
            {
                size_t new_cap = seq_cap ? seq_cap << 1 : 16;
                seq = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
                seq_cap = new_cap;
            }
            seq[seq_len++] = last_k;
        }
    }
    return seq[(size_t)n - 1];
}
