/*
 * A240135  a(n) = composite(n)*2^(n - 3).
 * https://oeis.org/A240135
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, less
 * Formula:   a(n) = composite(n) * 2^(n - 3)
 * Range:     u8 n <= 7, u16 n <= 14, u32 n <= 29, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* composite(n) is the nth composite number (starting with 4,6,8,9,10,…) */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

/* Compute the nth term of OEIS A240135.  */
/* This function is thread‑safe because all data are static local.  */
seq_t a(idx_t n)
{
    /* static cache of composites as 64‑bit integers.
       We use idx_t (uint64_t) for storage; conversion to seq_t is safe.  */
    static idx_t      *comp = NULL;   /* array of composite numbers */
    static size_t      comp_cap = 0;   /* allocated capacity */
    static size_t      comp_cnt = 0;   /* number of composites stored */
    static idx_t       next = 4;       /* next integer to test */

    /* Ensure we have at least n composites cached.  */
    if (comp_cnt < n) {
        /* Grow storage if needed.  */
        size_t need = n;
        if (need > comp_cap) {
            size_t newcap = comp_cap ? comp_cap * 2 : 128;
            while (newcap < need) newcap <<= 1;
            idx_t *tmp = (idx_t *)realloc(comp, newcap * sizeof(idx_t));
            if (!tmp) exit(EXIT_FAILURE);
            comp = tmp;
            comp_cap = newcap;
        }

        /* Generate composites until we reach the nth one.  */
        while (comp_cnt < n) {
            /* If next is not prime, it is a composite (>=4).  */
            if (!is_prime(next)) {
                comp[comp_cnt++] = next;
            }
            ++next;
        }
    }

    /* Fetch the desired composite number.  */
    seq_t comp_val = (seq_t)comp[n - 1];   /* zero‑based index */

    /* Multiply or divide by the appropriate power of two.  */
    if (n < 3) {
        /* Shift right: divide by 2^(3-n).  */
        comp_val >>= (3 - n);
    } else {
        /* Shift left: multiply by 2^(n-3).  */
        comp_val <<= (n - 3);
    }

    return comp_val;
}
