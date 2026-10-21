/*
 * A072747  Counting factor 2 in the first n squarefree numbers.
 * https://oeis.org/A072747
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of even squarefree numbers among the first n
 *            squarefree numbers
 * Range:     u8 n <= 767, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_squarefree
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_squarefree.h"

seq_t a(idx_t n) {
    /* Static state for memoisation */
    static seq_t *cache = NULL;
    static size_t cap = 0;
    static size_t len = 0;          /* number of computed terms */
    static uint64_t cur = 1;        /* next integer to test */
    static idx_t processed = 0;     /* how many squarefree numbers processed */
    static seq_t even_cnt = 0;      /* count of even squarefree numbers so far */

    if (len < (size_t)n) {
        /* Extend computation up to the required n */
        while (processed < n) {
            if (is_squarefree((seq_t)cur)) {
                processed++;
                if ((cur & 1ULL) == 0)  /* cur is even */
                    even_cnt++;

                /* Grow cache if necessary */
                if (len == cap) {
                    size_t new_cap = cap ? (cap << 1) : 1;
                    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                    if (!tmp) abort();
                    cache = tmp;
                    cap = new_cap;
                }
                cache[len++] = even_cnt;
            }
            cur++;
        }
    }

    return cache[n - 1];
}
