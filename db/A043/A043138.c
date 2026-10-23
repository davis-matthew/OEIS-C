/*
 * A043138  Numbers k such that 1 and 3 occur juxtaposed in the base-7
 *          representation of k but not of k-1.
 * https://oeis.org/A043138
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   has13_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Search: compute terms of A043138 by testing successive integers
    for the presence of the adjacent digit pairs 13 or 31 in base‑7, and
    ensuring the pair does not occur in the preceding integer. */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/has13_seq.h"

seq_t a(idx_t n)
{
    static seq_t *list      = NULL;   /* cache of found terms             */
    static size_t list_len = 0;       /* number of cached terms           */
    static size_t list_cap = 0;       /* current capacity of the cache    */
    static seq_t current_k  = 1;      /* next candidate to evaluate       */

    if (n == 0) return 0;             /* offset starts at 1               */

    /* Produce terms until we have at least n of them. */
    while (list_len < (size_t)n) {
        seq_t k = current_k;
        for (;;) {
            if (has13_seq(k) && !has13_seq(k - 1)) {
                /* Grow the cache if necessary. */
                if (list_len == list_cap) {
                    size_t new_cap = list_cap ? list_cap * 2 : 16;
                    seq_t *tmp = (seq_t *)realloc(list, new_cap * sizeof(seq_t));
                    if (!tmp) {                 /* out of memory */
                        free(list);
                        list = NULL;
                        list_cap = list_len = 0;
                        return 0;
                    }
                    list = tmp;
                    list_cap = new_cap;
                }
                list[list_len++] = k;
                current_k = k + 1;
                break;                         /* found next term */
            }
            ++k;                                 /* test next integer */
        }
    }
    return list[n - 1];
}
