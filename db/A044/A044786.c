/*
 * A044786  Numbers n such that string 7,3 occurs in the base 10
 *          representation of n but not of n+1.
 * https://oeis.org/A044786
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Validated: partial, pass, 39 terms through n = 39 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Sequence A044786 : numbers n such that "73" appears in the decimal
 * representation of n but not in n+1.
 *
 * The function generates terms by brute force enumeration.  It
 * maintains a static cache of already-found terms so that calls with
 * arbitrary indices remain fast.
 */
#include <stdlib.h>
#include "../utils/mem.h"

static inline int has73(seq_t x)
{
    /* Return 1 if x contains the substring "73" in decimal */
    while (x >= (seq_t)10) {
        seq_t last   = x % (seq_t)10;
        seq_t second = (x / (seq_t)10) % (seq_t)10;
        if (second == (seq_t)7 && last == (seq_t)3)
            return 1;
        x /= (seq_t)10;
    }
    return 0;
}

seq_t a(idx_t n)
{
    /* Handle the offset of the sequence (a(1) is the first term) */
    if (n == 0)
        return (seq_t)0;   /* undefined for non-positive indices */

    /* static cache of computed terms */
    static seq_t *terms = NULL;
    static size_t terms_len = 0;
    static size_t terms_cap = 0;

    /* next integer to test */
    static seq_t cur = (seq_t)1;

    while (terms_len < (size_t)n) {
        if (has73(cur) && !has73(cur + (seq_t)1)) {
            /* store cur in the cache */
            if (terms_len == terms_cap) {
                size_t new_cap = terms_cap ? terms_cap * 2 : 16;
                seq_t *tmp = mem_resize(seq_t, terms, new_cap);
                if (!tmp) /* allocation failure */
                    return (seq_t)0;
                terms = tmp;
                terms_cap = new_cap;
            }
            terms[terms_len++] = cur;
        }
        cur++;   /* move to the next integer */
    }

    return terms[n - 1];
}
