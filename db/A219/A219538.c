/*
 * A219538  G.f. satisfies A(x) = 1 + x*A(x)^2*(1 + A(x))^2/2.
 * https://oeis.org/A219538
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 19, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: 
   a(0)=1, a(1)=2, a(2)=12.
   For n≥3:
   3*n*(3*n-1)*(3*n+1)*(11*n-14)*a(n) =
     3*(2*n-1)*(693*n^3-1575*n^2+1026*n-176)*a(n-1) +
     2*(n-2)*(2*n-3)*(2*n-1)*(11*n-3)*a(n-2).
   The function memoises previously computed terms for O(n) time
   per call. */

#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>

/* External linkage as required */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;   /* array of cached terms */
    static size_t cap   = 0;      /* current allocation size   */
    static idx_t up_to  = 0;      /* highest index computed */

    /* Initialise cache with the first three terms. */
    if (!cache) {
        cap   = 4;                           /* room for indices 0..3 */
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)2;
        cache[2] = (seq_t)12;
        up_to   = 2;
    }

    /* Return cached value if already known. */
    if (n <= up_to)
        return cache[n];

    /* Expand the allocation as necessary. */
    size_t new_cap = cap;
    while (new_cap <= (size_t)n)
        new_cap <<= 1;
    cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cap   = new_cap;

    /* Compute values up to n using the recurrence. */
    for (idx_t i = up_to + 1; i <= n; ++i) {
        seq_t prev1 = cache[i - 1];
        seq_t prev2 = cache[i - 2];
        idx_t iv    = i;                 /* alias for readability */

        if (sizeof(seq_t) <= 8) {        /* seq_t fits in 64 bits – use 128‑bit intermediates */
            unsigned __int128 N   = iv;
            unsigned __int128 N2  = N * N;
            unsigned __int128 N3  = N2 * N;
            unsigned __int128 poly = (unsigned __int128)693 * N3
                                    - (unsigned __int128)1575 * N2
                                    + (unsigned __int128)1026 * N
                                    - (unsigned __int128)176;
            unsigned __int128 R1  = (unsigned __int128)3 * (unsigned __int128)(2*iv-1) * poly
                                   * (unsigned __int128)prev1;
            unsigned __int128 R2  = (unsigned __int128)2 * (unsigned __int128)(iv-2)
                                   * (unsigned __int128)(2*iv-3) * (unsigned __int128)(2*iv-1)
                                   * (unsigned __int128)(11*iv-3) * (unsigned __int128)prev2;
            unsigned __int128 L   = (unsigned __int128)3 * N * (unsigned __int128)(3*iv-1)
                                   * (unsigned __int128)(3*iv+1) * (unsigned __int128)(11*iv-14);
            unsigned __int128 val = (R1 + R2) / L;
            cache[i] = (seq_t)val;
        } else {                         /* seq_t is large (arbitrary‑precision) – use seq_t itself */
            seq_t N   = (seq_t)iv;
            seq_t N2  = N * N;
            seq_t N3  = N2 * N;
            seq_t poly = (seq_t)693 * N3 - (seq_t)1575 * N2
                         + (seq_t)1026 * N - (seq_t)176;
            seq_t R1 = (seq_t)3 * (seq_t)(2*iv-1) * poly * prev1;
            seq_t R2 = (seq_t)2 * (seq_t)(iv-2) * (seq_t)(2*iv-3)
                       * (seq_t)(2*iv-1) * (seq_t)(11*iv-3) * prev2;
            seq_t L  = (seq_t)3 * N * (seq_t)(3*iv-1)
                       * (seq_t)(3*iv+1) * (seq_t)(11*iv-14);
            cache[i] = (R1 + R2) / L;
        }
    }

    up_to = n;
    return cache[n];
}
