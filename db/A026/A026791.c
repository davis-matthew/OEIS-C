/*
 * A026791  Triangle in which n-th row lists juxtaposed lexicographically
 *          ordered partitions of n; e.g., the partitions of 3 (1+1+1,1+2,3)
 *          appear as 1,1,1,1,2,3 in row 3.
 * https://oeis.org/A026791
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 9881, u16 n <= 9881, u32 n <= 9881, u64 n <= 9881,
 *            u128 n <= 9881, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A026791: a(n) is the n-th part in the concatenation of all
    lexicographically ordered partitions of all positive integers.
    The function enumerates the partitions row by row until the
    desired position is reached. */

#include <stdbool.h>
#include <stdlib.h>

/* Recursive generation of partitions of "remaining" into parts
   that are at least "min_part".  The current partial partition
   is stored in "parts[0..pos-1]".  Each complete partition is
   emitted part by part, updating the global counter "cur".  When
   the target index is reached, the corresponding part is stored
   in *result and *found is set. */
static void gen_part(idx_t remaining, idx_t min_part, idx_t *parts,
                     idx_t pos, idx_t *cur, idx_t target,
                     seq_t *result, bool *found)
{
    if (*found) return;

    if (remaining == 0) {
        /* Emit the completed partition. */
        for (idx_t i = 0; i < pos; ++i) {
            ++(*cur);
            if (*cur == target) {
                *result = (seq_t)parts[i];
                *found = true;
                return;
            }
        }
        return;
    }

    for (idx_t x = min_part; x <= remaining; ++x) {
        parts[pos] = x;
        gen_part(remaining - x, x, parts, pos + 1,
                 cur, target, result, found);
        if (*found) return;
    }
}

/* External linkage as required. */
seq_t a(idx_t n)
{
    /* 1‑based target index. */
    idx_t target = n;
    idx_t cur = 0;          /* cumulative parts emitted so far */
    seq_t result = 0;       /* value to return */
    bool found = false;
    idx_t row = 1;          /* current partition length (row number) */

    while (!found) {
        /* Allocate temporary array for the current row. */
        idx_t *parts = (idx_t *)calloc(row, sizeof(idx_t));
        if (!parts) {
            /* Allocation failure: return 0 (undefined behaviour
               otherwise, but this safeguards against crashes). */
            return 0;
        }
        gen_part(row, 1, parts, 0, &cur, target, &result, &found);
        free(parts);
        ++row;
    }

    return result;
}
