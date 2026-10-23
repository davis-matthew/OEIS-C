/*
 * A171665  Numbers n such that n and n+1 have same number of divisors, and
 *          that number of divisors is larger than 4.
 * https://oeis.org/A171665
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 2000, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   divisor_count_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: n such that n and n+1 have the same number of divisors,  
   and that number of divisors is greater than 4.  
   Computation: brute‑force search with divisor_count_u64. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/divisor_count_u64.h"

seq_t a(idx_t n)
{
    /* Cache of found terms */
    static seq_t   *list      = NULL;
    static size_t   list_len  = 0;   /* number of terms cached          */
    static size_t   list_cap  = 0;   /* allocated capacity              */
    static idx_t    next_cand = 1;   /* next integer to test            */

    while (list_len < n) {
        uint64_t val = (uint64_t)next_cand;
        uint64_t d1  = divisor_count_u64(val);
        uint64_t d2  = divisor_count_u64(val + 1);

        if (d1 == d2 && d1 > 4) {
            if (list_len == list_cap) {
                size_t new_cap = list_cap ? (list_cap << 1) : 16;
                seq_t *tmp = (seq_t *)realloc(list, new_cap * sizeof(seq_t));
                if (!tmp) return 0;               /* allocation failure   */
                list     = tmp;
                list_cap = new_cap;
            }
            list[list_len] = (seq_t)val;
            ++list_len;
        }
        ++next_cand;
    }
    return list[n - 1];
}
