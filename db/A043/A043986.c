/*
 * A043986  Numbers k such that 4 and 5 occur juxtaposed in the base-9
 *          representation of k but not of k+1.
 * https://oeis.org/A043986
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 42, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   seq_alloc, seq_free
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Definition: a(n) is the nth integer k such that the base‑9 representation
   of k contains the digits 4 and 5 adjacent in either order (45 or 54)
   while the base‑9 representation of k+1 does not. */

#include <stdbool.h>
#include <stddef.h>
#include "../utils/seq_alloc.h"
#include "../utils/seq_free.h"

/* Return true if base‑9 representation of x contains adjacent digits 4
   and 5 in either order. */
static bool has_adjacent45(seq_t x)
{
    if (x == 0)
        return false;

    seq_t prev = 9;           /* sentinel that cannot be a digit */
    while (x > 0) {
        seq_t d = x % 9;      /* current least significant digit */
        if ((prev == 5 && d == 4) || (prev == 4 && d == 5))
            return true;     /* found "45" or "54" */
        x /= 9;
        prev = d;
    }
    return false;
}

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;            /* undefined below the offset */

    /* Cached terms to support out‑of‑order calls. */
    static seq_t *cache = NULL;
    static size_t cache_capacity = 0;
    static size_t cache_size = 0;
    static seq_t last_k = 0;      /* largest k that has been scanned */

    /* Grow cache if needed when adding a new element. */
    if (cache == NULL) {
        cache_capacity = 64;
        cache = seq_alloc(cache_capacity);
    }

    /* Scan forward until we have computed the nth term. */
    while (cache_size < n) {
        seq_t k = last_k + 1;           /* start just after the last scanned k */

        /* Find the next qualifying number. */
        for (;;) {
            seq_t kplus = k + 1;
            if (has_adjacent45(k) && !has_adjacent45(kplus)) {
                if (cache_size == cache_capacity) {
                    size_t newcap = cache_capacity * 2;
                    seq_t *tmp = seq_alloc(newcap);
                    size_t i;
                    for (i = 0; i < cache_size; ++i)
                        tmp[i] = cache[i];
                    seq_free(cache);
                    cache = tmp;
                    cache_capacity = newcap;
                }
                cache[cache_size] = k;
                cache_size++;
                last_k = k;
                break;          /* move on to the next missing term */
            }
            k++;
        }
    }

    return cache[n - 1];
}
