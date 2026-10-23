/*
 * A043946  Numbers k such that 2 and 4 occur juxtaposed in the base-8
 *          representation of k but not of k+1.
 * https://oeis.org/A043946
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 44, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers k such that 2 and 4 occur juxtaposed in the base‑8 representation
   of k but not of k+1.  Implements a(n) for OEIS A043946. */
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

/* Return true if the base‑8 digits of x contain the substring "24" or "42".  */
static bool has24(seq_t x)
{
    if (x == 0)
        return false;
    seq_t prev = x % (seq_t)8;
    x /= (seq_t)8;
    while (x > 0) {
        seq_t cur = x % (seq_t)8;
        if ((cur == (seq_t)2 && prev == (seq_t)4) ||
            (cur == (seq_t)4 && prev == (seq_t)2))
            return true;
        prev = cur;
        x /= (seq_t)8;
    }
    return false;
}

/* Return the n‑th term (1‑based) of the sequence.  */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;   /* array of found terms */
    static size_t cap   = 0;      /* allocated capacity */
    static size_t len   = 0;      /* number of terms stored */
    static seq_t next_k = 1;      /* next candidate to test */

    while (len < n) {
        if (has24(next_k) && !has24(next_k + (seq_t)1)) {
            if (len == cap) {
                size_t new_cap = (cap == 0) ? 16 : cap << 1;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(*cache));
                if (tmp == NULL)
                    return (seq_t)0;
                cache = tmp;
                cap = new_cap;
            }
            cache[len++] = next_k;
        }
        ++next_k;
    }
    return cache[n - 1];
}
