/*
 * A038461  Sums of 10 distinct powers of 2.
 * https://oeis.org/A038461
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) is the nth smallest integer that has exactly 10 ones in its
 *            binary representation (i.e., the nth sum of 10 distinct powers
 *            of 2). This is obtained by unranking the combinatorial number
 *            system (combinadic) for k = 10
 * Range:     u8 none, u16 n <= 8008, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   binom, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binom.h"   /* provides seq_t binom(idx_t, idx_t); */
#include "../utils/pow2.h"    /* provides seq_t pow2(idx_t); */

seq_t a(idx_t n) {
    const int k = 10;                     /* number of set bits */
    seq_t rank = (seq_t)n - 1;            /* zero‑based rank */
    int pos[k];                           /* positions of the set bits (0 = LSB) */

    /* Unrank: find the positions of the k bits */
    for (int i = k; i >= 1; --i) {        /* i = number of bits left to place */
        int p = i - 1;                   /* minimal possible position for this bit */
        while (1) {
            seq_t c = binom((idx_t)p, (idx_t)i);
            if (c <= rank) {
                ++p;
            } else {
                break;
            }
        }
        --p;                              /* last valid position */
        pos[i - 1] = p;                   /* store position */
        rank -= binom((idx_t)p, (idx_t)i);/* subtract contribution */
    }

    /* Build the result by adding 2^pos for each set bit */
    seq_t result = 0;
    for (int i = 0; i < k; ++i) {
        result += pow2((idx_t)pos[i]);
    }
    return result;
}
