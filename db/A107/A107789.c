/*
 * A107789  Trajectory of 2 under evenly many applications of the morphism 1
 *          -> 2, 2 -> 114, 3 -> 4, 4 -> 233.
 * https://oeis.org/A107789
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=2, a(n) = nth symbol of the fixed point of the morphism τ:
 *            2 → 22233, 3 → 233 (the 2‑step substitution of the original
 *            mapping 1→2, 2→114, 3→4, 4→233)
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Precompute lengths of τ^d applied to symbols 2 and 3 */
    /* Depth needed so that len2[depth] > n */
    const int MAX_DEPTH = 128;                /* plenty for n ≤ 10000 */
    seq_t len2[MAX_DEPTH];
    seq_t len3[MAX_DEPTH];

    len2[0] = (seq_t)1;
    len3[0] = (seq_t)1;
    int depth = 0;

    /* Find minimal depth with length of symbol 2 exceeding n */
    while ((idx_t)len2[depth] <= n) {
        depth++;
        /* Safety guard: should never hit MAX_DEPTH for all tests */
        if (depth >= MAX_DEPTH) break;
        seq_t l2 = (seq_t)3 * len2[depth - 1] + (seq_t)2 * len3[depth - 1];
        seq_t l3 = (seq_t)1 * len2[depth - 1] + (seq_t)2 * len3[depth - 1];
        len2[depth] = l2;
        len3[depth] = l3;
    }

    /* Iteratively determine the symbol at position n */
    seq_t symbol = 2;          /* start from the initial symbol 2 */
    idx_t pos = n;
    int d = depth;

    while (d > 0) {
        if (symbol == 2) {
            /* children: 2,2,2,3,3 (in this order) */
            if (pos < len2[d - 1]) {
                symbol = 2;
            } else {
                pos -= (idx_t)len2[d - 1];
                if (pos < len2[d - 1]) {
                    symbol = 2;
                } else {
                    pos -= (idx_t)len2[d - 1];
                    if (pos < len2[d - 1]) {
                        symbol = 2;
                    } else {
                        pos -= (idx_t)len2[d - 1];
                        if (pos < len3[d - 1]) {
                            symbol = 3;
                        } else {
                            pos -= (idx_t)len3[d - 1];
                            symbol = 3; /* last child */
                        }
                    }
                }
            }
        } else { /* symbol == 3 */
            /* children: 2,3,3 */
            if (pos < len2[d - 1]) {
                symbol = 2;
            } else {
                pos -= (idx_t)len2[d - 1];
                if (pos < len3[d - 1]) {
                    symbol = 3;
                } else {
                    pos -= (idx_t)len3[d - 1];
                    symbol = 3; /* last child */
                }
            }
        }
        d--;
    }

    return symbol;
}
