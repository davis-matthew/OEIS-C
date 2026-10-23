/*
 * A044518  Numbers k such that the string 5,5 occurs in the base-6
 *          representation of k but not of k+1.
 * https://oeis.org/A044518
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   grow_seq, has_consecutive_5
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Definition: a(n) is the nth positive integer k such that
 * the base‑6 representation of k contains the substring "55"
 * and the base‑6 representation of k+1 does not. */

#include <stdbool.h>
#include <stddef.h>

#include "../utils/grow_seq.h"
#include "../utils/has_consecutive_5.h"

/* Return true if k qualifies: k has "55" and k+1 does not. */
static bool qualifies(seq_t k)
{
    return has_consecutive_5(k) && !has_consecutive_5(k + 1);
}

seq_t a(idx_t n)
{
    /* Cache of previously found sequence values. */
    static seq_t *seq = NULL;
    static size_t cap = 0;   /* capacity of the array */
    static size_t sz  = 0;   /* current number of stored terms   */
    static seq_t next = 0;   /* next integer to test */

    while (sz < n) {
        /* Find the next qualifying k. */
        do {
            ++next;
        } while (!qualifies(next));

        /* Grow the array if necessary. */
        if (sz >= cap)
            grow_seq(&seq, &cap, sz + 1);

        seq[sz++] = next;
    }

    return seq[n - 1];
}
