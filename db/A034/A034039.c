/*
 * A034039  Numbers that are primitively but not imprimitively represented by
 *          (x^2+xy+2y^2, x>=0, y>=0).
 * https://oeis.org/A034039
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   floor_sqrt, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS A034039:
   Numbers that are primitively but not imprimitively represented
   by the quadratic form x^2 + x*y + 2*y^2 with x ≥ 0, y ≥ 0.
   The algorithm tests each integer sequentially, recording those
   that admit a primitive representation and admit no representation
   with gcd(x,y) > 1. */
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/floor_sqrt.h"
#include "../utils/gcd.h"

static bool qualifies(seq_t n)
{
    /* Search for all representations n = x^2 + x*y + 2*y^2.
       For each representation we check if it is primitive
       (gcd(x,y)==1) or imprimitive (gcd(x,y)>1).  The candidate
       is accepted iff it has at least one primitive representation
       and none that are imprimitive. */
    unsigned __int128 N128 = (unsigned __int128)n;
    unsigned __int128 limit = (N128 * 4) / 7; /* y^2 <= limit */

    bool havePrim = false, haveImp = false;

    for (idx_t y = 0ULL; ; ++y) {
        unsigned __int128 y2 = (unsigned __int128)y * y;
        if (y2 > limit)
            break;

        unsigned __int128 t128 = (N128 * 4) - (y2 * 7);
        seq_t t = (seq_t)t128;
        seq_t s = floor_sqrt(t);

        /* Check perfect square */
        if ((unsigned __int128)s * s != t128)
            continue;

        /* Require s and y to have same parity and s >= y so that x >= 0 */
        if (((s ^ (seq_t)y) & 1U) != 0U || s < y)
            continue;

        seq_t x = (s - (seq_t)y) / 2U;
        if (x == 0 && y == 0)
            continue;   /* ignore trivial zero representation */

        seq_t g = gcd(x, (seq_t)y);
        if (g == 1U)
            havePrim = true;
        else
            haveImp = true;

        if (havePrim && haveImp)
            break;       /* both found – no need to search further */
    }
    return havePrim && !haveImp;
}

seq_t a(idx_t n)
{
    static seq_t *terms    = NULL;
    static size_t terms_len  = 0;
    static size_t terms_cap  = 0;
    static idx_t  next_candidate = 1;   /* next integer to test */

    if (n == 0U)
        return 0;

    /* Generate terms until we reach the requested index */
    while (terms_len < n) {
        if (qualifies((seq_t)next_candidate)) {
            if (terms_cap == 0) {
                terms_cap = 128;
                terms = (seq_t *)malloc(terms_cap * sizeof(seq_t));
            } else if (terms_len >= terms_cap) {
                terms_cap <<= 1;
                terms = (seq_t *)realloc(terms, terms_cap * sizeof(seq_t));
            }
            terms[terms_len++] = (seq_t)next_candidate;
        }
        ++next_candidate;
    }
    return terms[n - 1];
}
