/*
 * A093373  Numbers that are not the product of factorials.
 * https://oeis.org/A093373
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = the n-th positive integer that is not a product of factorials */

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/seq_realloc.h"

/* upper bound for precomputation – large enough for n up to ~10000 */
#define PRECOMP_LIMIT 2000000ULL

seq_t a(idx_t n)
{
    /* static state for lazy initialization */
    static bool initialized = false;
    static unsigned char *reachable = NULL;   /* reachable[0..LIMIT] */
    static seq_t   *complement   = NULL;      /* array of complement numbers */
    static size_t  comp_size    = 0;           /* current number of stored values */
    static size_t  comp_cap     = 0;           /* allocated capacity of complement */
    static idx_t   current_val  = 1;           /* last integer examined */

    if (!initialized) {
        /* --- 1)  Allocate array of flags for all numbers up to LIMIT  --- */
        reachable = (unsigned char *)malloc((size_t)(PRECOMP_LIMIT + 1));
        memset(reachable, 0, (size_t)(PRECOMP_LIMIT + 1));

        /* --- 2)  Compute all factorials that fit in the limit --- */
        seq_t fac = 1;
        seq_t facts[64];           /* plenty of room for all factorials <= LIMIT */
        size_t fact_cnt = 0;
        for (idx_t i = 2; ; ++i) {
            if (fac > (seq_t)(PRECOMP_LIMIT) / (seq_t)i) break;   /* would overflow */
            fac *= (seq_t)i;
            if (fac > (seq_t)(PRECOMP_LIMIT)) break;
            facts[fact_cnt++] = fac;
        }

        /* --- 3)  BFS to generate every product of the above factorials --- */
        idx_t *queue = (idx_t *)malloc((size_t)(PRECOMP_LIMIT + 1) * sizeof(idx_t));
        size_t q_head = 0, q_tail = 0;

        reachable[1] = 1;          /* 1 is product of zero factorials */
        queue[q_tail++] = 1;       /* start BFS from 1 */

        while (q_head < q_tail) {
            idx_t val = queue[q_head++];
            for (size_t k = 0; k < fact_cnt; ++k) {
                seq_t f = facts[k];
                if (val <= PRECOMP_LIMIT / f) {
                    idx_t next = val * (idx_t)f;
                    if (!reachable[next]) {
                        reachable[next] = 1;
                        queue[q_tail++] = next;
                    }
                }
            }
        }
        free(queue);
        initialized = true;
    }

    /* --- 4)  Grow complement array until we have at least n terms --- */
    while (comp_size < n) {
        ++current_val;                     /* examine next integer > 1 */
        if (current_val <= PRECOMP_LIMIT) {
            if (!reachable[current_val]) { /* not a product of factorials */
                if (comp_size == comp_cap) {              /* need more space  */
                    size_t new_cap = comp_cap ? comp_cap * 2U : 128U;
                    complement = seq_realloc(complement, comp_cap, new_cap);
                    comp_cap = new_cap;
                }
                complement[comp_size++] = (seq_t)current_val;
            }
        } else {
            /* If the precomputation limit were ever exceeded, the algorithm
               would need to be extended. For the intended n ≤ 10000 this
               never occurs with PRECOMP_LIMIT = 2,000,000. */
            break;
        }
    }

    return complement[n - 1];
}
