/*
 * A109896  Group the natural numbers so that every 2n-th group product is
 *          divisible by the single number in the next group. (1), (2,3,4,5),
 *          (6), (7,8,9,10,11), (12), (13,14,15,16,17,18,19),(20),
 *          (21,22,23,24,25,26,27),(28),... Sequence contains the number of
 *          terms in the 2n-th group.
 * https://oeis.org/A109896
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 26, u32 n <= 75, u64 n <= 75, u128 n <= 75,
 *            big any n
 * Helpers:   mul128
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: generate the groups sequentially.  Each odd group
 * (1,3,5,… ) consists of a single integer.  Each even group (2,4,6,…)
 * contains a minimal number of consecutive integers starting from
 * the next unused number such that the product of the group is
 * divisible by the following single integer (which is just the next
 * integer).  a(n) is the size of the 2n‑th group. */
#include <stdint.h>
#include <stdbool.h>

/* Helper from lib */
#include "../utils/mul128.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* undefined for offset <1, but safe */
    idx_t target = 2 * n;           /* index of the group to return */


    idx_t group_idx = 1;            /* current group number (1‑based) */
    idx_t current   = 1;            /* next unused natural number */

    seq_t len = 0;                  /* answer will be stored here */

    while (group_idx <= target) {
        if (group_idx & 1) {        /* odd group – single element */
            current++;              /* consume the single integer */
            group_idx++;
        } else {                    /* even group – find minimal length */
            idx_t start = current;   /* first number in this group */
            idx_t l;                /* candidate length */

            /* Search for the smallest L such that the product of
             * start … start+L-1 is divisible by (start+L). */
            for (l = 1; ; ++l) {
                seq_t mod = (seq_t)(start + l);  /* divisor (next odd group) */
                seq_t prod = 1;

                /* Compute product modulo mod; stop early if it becomes zero. */
                for (idx_t j = 0; j < l; ++j) {
                    prod = mul128(prod, (seq_t)(start + j)) % mod;
                    if (prod == 0) break;
                }

                if (prod == 0) break;           /* found minimal L */
            }

            /* If we are at the target group, return its length */
            if (group_idx == target) return (seq_t)l;

            /* Move past this even group and increment counter */
            current += l;
            group_idx++;
        }
    }
    return len;  /* should never reach here */
}
