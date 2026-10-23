/*
 * A177145  Expansion of e.g.f. arcsin(x).
 * https://oeis.org/A177145
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 0 for n even; a(2k+1) = (∏_{i=1..k} (2i–1))², with
 *            (−1)!!=1
 * Range:     u8 n <= 8, u16 n <= 10, u32 n <= 14, u64 n <= 22, u128 n <= 30,
 *            big any n
 * Helpers:   seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A177145 – expansion of e.g.f. arcsin(x). */

#include <stddef.h>                /* for size_t, idx_t is defined elsewhere */
#include "../utils/seq_realloc.h"  /* seq_realloc(seq_t *old, size_t old_cap, size_t new_cap) */

/* Cached values for odd indices: store[k] holds a(2k+1). */
static seq_t *store = NULL;
static idx_t  store_len = 0;   /* number of computed entries, i.e. store_len == k+1 */
static idx_t  store_cap = 0;   /* current capacity of the store array */

/* Ensure that store[0..k] are computed; enlarges the array if needed. */
static void ensure_store(idx_t k)
{
    if (k < store_len) return;                    /* already computed */

    /* Grow the array if required. */
    if (k + 1 > store_cap) {
        idx_t new_cap = store_cap ? store_cap * 2 : 1;
        while (new_cap < k + 1) new_cap *= 2;
        store = seq_realloc(store, store_cap, new_cap);
        store_cap = new_cap;
    }

    /* Maintain the running product of odd numbers: 1, 1·3, 1·3·5, ... */
    static seq_t product = 1;                      /* product of odd numbers up to current k-1 */
    idx_t i = store_len;                           /* start from the next index to compute */

    for (; i <= k; ++i) {
        if (i == 0) {
            /* (−1)!! = 1, so a(1) = 1 */
            product = 1;
            store[0] = product * product;          /* 1*1 = 1 */
        } else {
            seq_t odd = (seq_t)(2 * i - 1);
            product = product * odd;               /* extend product by (2i-1) */
            store[i] = product * product;          /* square the product */
        }
    }
    store_len = k + 1;
}

/* Public sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* a(0) = 0 */
    if ((n & 1) == 0) return 0;         /* even indices are zero */

    idx_t k = (n - 1) >> 1;             /* k = (n-1)/2 */
    ensure_store(k);
    return store[k];
}
