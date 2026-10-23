/*
 * A032864  Numbers whose base-9 representation Sum_{i=0..m} d(i)*9^i has d(m)
 *          > d(m-1) < d(m-2) > ...
 * https://oeis.org/A032864
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the n-th integer (offset 1) whose base‑9 digits d_k
 *            d_{k‑1}…d_0 satisfy the alternating pattern d_k > d_{k‑1} <
 *            d_{k‑2} > d_{k‑3} < … . The function generates numbers in
 *            increasing order and memoizes already computed terms
 * Range:     u8 n <= 62, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   alt9
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/alt9.h"

/* Main sequence function. */
seq_t a(idx_t n)
{
    /* Static cache of already generated terms. */
    static seq_t *cache = NULL;
    static size_t size = 0;   /* number of terms stored */
    static size_t cap  = 0;   /* capacity of cache */
    static seq_t cur   = 0;   /* next integer to test */

    if (n == 0) return 0;   /* safety, offset starts at 1 */

    /* Grow cache to store at least n terms. */
    while (size < n) {
        if (cache == NULL || size >= cap) {
            size_t new_cap = (cap == 0) ? 64 : cap * 2;
            seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) exit(EXIT_FAILURE);   /* allocation failure */
            cache = tmp;
            cap   = new_cap;
        }
        if (alt9(cur)) {
            cache[size++] = cur;
        }
        ++cur;
    }

    return cache[n - 1];
}
