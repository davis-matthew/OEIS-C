/*
 * A073718  Powers of 2 with composite exponents.
 * https://oeis.org/A073718
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2^c(n) (c(n) = the nth composite integer, n ≥ 1)
 * Range:     u8 n <= 2, u16 n <= 8, u32 n <= 19, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Validated: partial, pass, 24 terms through n = 24 (2026-11-01)
 * Helpers:   ipow_idx, is_prime_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/ipow_idx.h"
#include "../utils/is_prime_seq.h"

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;                       /* undefined for n < 1 */

    /* Cached list of composite integers */
    static uint64_t *comps = NULL;
    static size_t   cap   = 0;
    static size_t   cnt   = 0;
    static uint64_t last  = 1;                 /* last integer examined */

    if (!comps) {                              /* first call: allocate */
        cap   = 128;
        comps = (uint64_t *)malloc(cap * sizeof(uint64_t));
    }

    /* Extend the cache until we have at least n composites */
    while (cnt < n) {
        last++;
        if (last > 1 && !is_prime_seq((seq_t)last)) {
            if (cnt >= cap) {
                cap <<= 1;
                comps = (uint64_t *)realloc(comps, cap * sizeof(uint64_t));
            }
            comps[cnt++] = last;
        }
    }

    idx_t exp = (idx_t)comps[n - 1];
    return ipow_idx((seq_t)2, exp);            /* compute 2^exp exactly */
}
