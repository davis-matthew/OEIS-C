/*
 * A064814  Greatest common divisor of n and the n-th composite number.
 * https://oeis.org/A064814
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nice, nonn
 * Formula:   a(n) = gcd(n , A002808(n)) (A002808 = n‑th composite)
 * Range:     u8 n <= 1921, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 114 terms through n = 9915 (2026-11-03)
 * Helpers:   gcd, is_prime, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(1) is the first composite; for n<1 the function returns 0. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/gcd.h"
#include "../utils/is_prime.h"
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;           /* undefined for n<1, harmless */

    /* Static cache of the composite numbers found so far. */
    static seq_t *comps   = NULL;          /* array of composites   */
    static size_t   len   = 0;             /* number stored         */
    static size_t   cap   = 0;             /* allocated capacity    */
    static idx_t    next  = 4;             /* last integer examined +1 */

    /* Initialise cache on first call. */
    if (comps == NULL) {
        cap   = 256;                      /* start with some room  */
        comps = mem_new(seq_t, cap);
        len   = 0;
    }

    /* Produce composites until we have at least n of them. */
    while (len < n) {
        if (!is_prime((uint64_t)next)) {          /* composite detected */
            if (len >= cap) {                 /* grow array       */
                cap *= 2;
                comps = mem_resize(seq_t, comps, cap);
            }
            comps[len++] = (seq_t)next;
        }
        ++next;
    }

    /* Compute a(n) = gcd(n , A002808(n))  (A002808(n) is comps[n‑1]) */
    return gcd((seq_t)n, comps[n - 1]);
}
