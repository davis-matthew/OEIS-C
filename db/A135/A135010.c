/*
 * A135010  Triangle read by rows in which row n lists A000041(n-1) 1's
 *          followed by the list of juxtaposed lexicographically ordered
 *          partitions of n that do not contain 1 as a part.
 * https://oeis.org/A135010
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 2456, u16 n <= 9907, u32 n <= 9907, u64 n <= 9907,
 *            u128 n <= 9907, big any n
 * Helpers:   find_partitions
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A135010 – triangle of 1's and lexicographically ordered partitions with parts ≥ 2
    The n-th term a(n) is the n-th entry of the flattened triangle:
      row r = p(r‑1) copies of 1 followed by the parts of all partitions of r
      that contain no part equal to 1, listed in lexicographic order
      (parts are written in non‑decreasing order).
*/

#include <stdbool.h>
#include <stddef.h>
#include "../utils/find_partitions.h"

/* forward declarations of helper */
static bool find_partitions(idx_t *rem, seq_t *out,
                            int sum, int min_part,
                            int *parts, int depth);

/* pre‑computed partition numbers p[0] … p[200]   */
static seq_t *p_parts = NULL;
static idx_t p_size = 0;

/* ensure partition numbers up to limit are available */
static void ensure_partitions(idx_t limit)
{
    if (limit < 200) limit = 200;
    if (p_parts && p_size > limit) return;

    seq_t *new_table = (seq_t*)realloc(p_parts, (limit + 1) * sizeof(seq_t));
    if (!new_table) abort();          /* allocation failure */
    p_parts = new_table;
    p_size = limit + 1;

    /* compute partition numbers using the classic DP */
    p_parts[0] = 1;
    for (idx_t i = 1; i <= limit; ++i) p_parts[i] = 0;
    for (idx_t k = 1; k <= limit; ++k)
        for (idx_t i = k; i <= limit; ++i)
            p_parts[i] += p_parts[i - k];
}

/* main sequence function */
seq_t a(idx_t n)
{
    /* pre‑compute partition numbers up to a reasonable limit */
    ensure_partitions(200);

    idx_t remaining = n;          /* position we are looking for */
    for (idx_t r = 1;; ++r) {
        /* number of 1's at the start of row r */
        idx_t ones = (r - 1 <= 200) ? (idx_t)p_parts[r - 1] : 0;
        if (remaining <= ones)
            return (seq_t)1;     /* falls inside the block of 1's */
        remaining -= ones;

        /* generate partitions of r (parts ≥ 2) */
        int parts_buf[128];
        seq_t value;
        if (find_partitions(&remaining, &value, (int)r, 2, parts_buf, 0))
            return value;
        /* otherwise the target is further down, proceed to next row */
    }
}
