/*
 * A026930  Number of partitions of n into an even number of parts, the
 *          greatest being 6; also, a(n+11) = number of partitions of n+5 into
 *          an odd number of parts, each <=6.
 * https://oeis.org/A026930
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of n into an even number of parts,
 *            each part ≤ 6, that contain at least one part equal to 6.
 *            Implemented by dynamic programming
 * Range:     u8 n <= 29, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Validated: partial, pass, 52 terms through n = 52 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    /* static buffers reused between calls */
    static seq_t *even_f = NULL, *odd_f = NULL;
    static seq_t *even_g = NULL, *odd_g = NULL;
    static idx_t cap = 0;               /* capacity of the buffers      */

    /* ensure capacity at least n+1 */
    idx_t needed = n + 1;
    if (cap < needed) {
        idx_t newcap = needed;
        even_f = mem_resize(seq_t, even_f, newcap);
        odd_f  = mem_resize(seq_t, odd_f, newcap);
        even_g = mem_resize(seq_t, even_g, newcap);
        odd_g  = mem_resize(seq_t, odd_g, newcap);
        cap = newcap;
    }

    /* initialize arrays to zero up to n */
    for (idx_t i = 0; i < needed; ++i) {
        even_f[i] = 0;
        odd_f[i]  = 0;
        even_g[i] = 0;
        odd_g[i]  = 0;
    }

    /* base case: empty partition has zero parts (even) */
    even_f[0] = 1;
    even_g[0] = 1;

    /* DP for all parts 1..6 (full partitions) */
    for (idx_t part = 1; part <= 6; ++part) {
        for (idx_t i = part; i <= n; ++i) {
            even_f[i] += odd_f[i - part];
            odd_f[i]  += even_f[i - part];
        }
    }

    /* DP for parts 1..5 (partitions that do NOT use 6) */
    for (idx_t part = 1; part <= 5; ++part) {
        for (idx_t i = part; i <= n; ++i) {
            even_g[i] += odd_g[i - part];
            odd_g[i]  += even_g[i - part];
        }
    }

    /* a(n) = even partitions that contain a 6 minus those that don't */
    return even_f[n] - even_g[n];
}
