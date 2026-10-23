/*
 * A107333  1-limiting word of the morphism 1->1331, 2->3, 3->12.
 * https://oeis.org/A107333
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 1-limiting word of the morphism 1->1331, 2->3, 3->12
   The infinite word is obtained by iterating this morphism
   starting with the single symbol “1”.
   a(n) is the n-th (0‑based) symbol of this limit word. */

#include <stdint.h>
/* seq_t and idx_t are provided by the build environment. */

seq_t a(idx_t n)
{
    /* Lengths of the block produced by a single symbol after d further
       morphism applications.  lenX[d] is the length of the block
       obtained from symbol X after d additional morphisms. */
    static uint64_t len1[65], len2[65], len3[65];
    static int initialized = 0;

    if (!initialized) {
        /* For 0 further morphisms the block is the symbol itself. */
        len1[0] = len2[0] = len3[0] = 1;
        initialized = 1;
    }

    /* Find the smallest depth such that the block from the root symbol 1
       is longer than n.  This is the depth we will descend. */
    int depth = 0;
    while (len1[depth] <= n && depth < 64) {
        depth++;
        __uint128_t v1 = (__uint128_t)2 * len1[depth - 1]
                       + (__uint128_t)2 * len3[depth - 1];
        len1[depth] = (v1 > UINT64_MAX) ? UINT64_MAX : (uint64_t)v1;

        /* 2 maps to a single 3 */
        len2[depth] = len3[depth - 1];

        __uint128_t v3 = (__uint128_t)len1[depth - 1] + (__uint128_t)len2[depth - 1];
        len3[depth] = (v3 > UINT64_MAX) ? UINT64_MAX : (uint64_t)v3;
    }

    /* Descend from the root symbol 1 to the leaf that contains position n.
       At each step we know the current symbol and the depth remaining. */
    uint64_t idx = n;
    int symbol = 1;          /* the root of the limit word is '1' */

    for (int d = depth; d > 0; d--) {
        switch (symbol) {
        case 1: {                  /* 1 -> 1 3 3 1 */
            uint64_t l1 = len1[d - 1];
            uint64_t l3 = len3[d - 1];

            if (idx < l1) {
                /* first child: 1 */
                symbol = 1;
            } else {
                idx -= l1;
                if (idx < l3) {   /* second child: 3 */
                    symbol = 3;
                } else {
                    idx -= l3;
                    if (idx < l3) {   /* third child: 3 */
                        symbol = 3;
                    } else {          /* fourth child: 1 */
                        idx -= l3;
                        symbol = 1;
                    }
                }
            }
            break;
        }
        case 2:                   /* 2 -> 3 */
            symbol = 3;
            /* idx stays unchanged because len3[d-1] is guaranteed > idx. */
            break;
        case 3: {                  /* 3 -> 1 2 */
            uint64_t l1 = len1[d - 1];
            uint64_t l2 = len2[d - 1];

            if (idx < l1) {
                symbol = 1;
            } else {
                idx -= l1;
                symbol = 2;
            }
            break;
        }
        default:
            /* Should never happen; fall back to '1'. */
            symbol = 1;
            break;
        }
    }

    return (seq_t)symbol;
}
