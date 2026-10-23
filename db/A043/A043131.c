/*
 * A043131  Numbers k such that 0 and 1 occur juxtaposed in the base-7
 *          representation of k but not of k-1.
 * https://oeis.org/A043131
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 44, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A043131: Numbers k such that 0 and 1 occur juxtaposed in the base-7
   representation of k but not of k-1.  The function searches for such
   integers incrementally and caches the results. */

#include <stdlib.h>
#include <stdbool.h>

/* Return true if the base‑7 representation of x contains an adjacent
   pair of digits 0 and 1 (in either order). */
static inline bool has_adj_zero_one(seq_t x)
{
    if (x == 0)
        return false;                /* a single digit 0 has no pair */
    seq_t prev = x % 7;
    x /= 7;
    while (x > 0) {
        seq_t cur = x % 7;
        if ((prev == 0 && cur == 1) || (prev == 1 && cur == 0))
            return true;
        prev = cur;
        x /= 7;
    }
    return false;
}

seq_t a(idx_t n)
{
    static seq_t *table = NULL;   /* cache: table[0] is a(1) */
    static size_t table_cap = 0;
    static size_t table_len = 0;
    static seq_t next_k = 0;      /* next candidate to test */

    if (n == 0)                 /* undefined for n < 1, harmless */
        return (seq_t)0;

    /* Grow the table until we have at least n entries. */
    while (table_len < (size_t)n) {
        seq_t k = ++next_k;          /* candidate number */

        bool cur_has  = has_adj_zero_one(k);
        bool prev_has = has_adj_zero_one(k - 1);   /* safe: k ≥ 1 */

        if (cur_has && !prev_has) {
            /* Allocate more space if needed. */
            if (table_len >= table_cap) {
                table_cap = table_cap ? table_cap * 2 : 16;
                table = (seq_t *)realloc(table,
                                         table_cap * sizeof(seq_t));
            }
            table[table_len++] = k;
        }
    }

    return table[n - 1];           /* 1‑based index */
}
