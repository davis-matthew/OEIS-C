/*
 * A100878  Smallest number of pentagonal numbers which sum to n.
 * https://oeis.org/A100878
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = minimal number of pentagonal numbers whose sum equals n
 *            (by the polygonal number theorem, a(n) ≤ 5 for all n > 0)
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <limits.h>

/* Helper macro to get maximum of two values (needed only for sanity) */
#define MAX(a,b) ((a) > (b) ? (a) : (b))

seq_t a(idx_t n)
{
    /* a(0) = 0 by definition */
    if (n == 0) {
        return (seq_t)0;
    }

    /* --------------------------------------------------------------------
       Step 1: generate the list of all pentagonal numbers ≤ n.
       Pentagonal numbers are given by P(k) = k*(3*k - 1)/2,  k ≥ 1.
    -------------------------------------------------------------------- */
    /* We store them as idx_t because they are ≤ n. */
    idx_t pent_size = 0;   /* number of pentagonal numbers found */
    idx_t pent_capacity = 16; /* initial capacity */
    idx_t *pent = (idx_t *)malloc(pent_capacity * sizeof(idx_t));
    if (!pent) abort();     /* allocation failure */

    for (idx_t k = 1; ; ++k) {
        seq_t pk = (seq_t)k * ((seq_t)3 * k - (seq_t)1) / (seq_t)2;
        if (pk > n) break;
        /* grow array if needed */
        if (pent_size >= pent_capacity) {
            pent_capacity *= 2;
            idx_t *new_pent = (idx_t *)realloc(pent, pent_capacity * sizeof(idx_t));
            if (!new_pent) { free(pent); abort(); }
            pent = new_pent;
        }
        pent[pent_size++] = (idx_t)pk;
    }

    /* --------------------------------------------------------------------
       Step 2: dynamic programming for minimal number of pentagonal parts.
       We keep an array of uint8_t distances (0..255), using 255 as "infinite".
    -------------------------------------------------------------------- */
    uint8_t *dist = (uint8_t *)malloc((size_t)(n + 1) * sizeof(uint8_t));
    if (!dist) { free(pent); abort(); }

    for (size_t i = 0; i <= n; ++i) dist[i] = 255; /* INF initial value */
    dist[0] = 0;

    for (idx_t i = 0; i <= n; ++i) {
        uint8_t cur = dist[i];
        if (cur == 255) continue;          /* unreachable so far */
        if (cur >= 5) continue;            /* cannot exceed 5 by theorem */
        for (idx_t j = 0; j < pent_size; ++j) {
            idx_t p = pent[j];
            idx_t nxt = i + p;
            if (nxt > n) break;           /* p's are sorted ascending */
            if (dist[nxt] > cur + 1) {
                dist[nxt] = cur + 1;
            }
        }
    }

    /* Result is guaranteed to be ≤ 5 for all n > 0 */
    seq_t result = (seq_t)dist[n];

    /* --------------------------------------------------------------------
       Clean up temporary allocations and return
    -------------------------------------------------------------------- */
    free(pent);
    free(dist);

    return result;
}
