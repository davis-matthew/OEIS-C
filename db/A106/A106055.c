/*
 * A106055  Fixed point of the morphism 1 -> 21212, 2 -> 3, 3 -> 43434, 4 ->
 *          1, applied 4*k times to 1.
 * https://oeis.org/A106055
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, less
 * Formula:   a(n) = nth symbol of the fixed point of the morphism 1 → 21212,
 *            2 → 3, 3 → 43434, 4 → 1, applied 4*k times to the starting
 *            symbol 1
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function generates the word by repeatedly applying the
   morphism until its length exceeds n, then returns the n‑th
   symbol.  It uses a byte buffer to store the symbols (1–4) and
   finally casts the result to seq_t. */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* buffer holding symbols 1..4 */
    uint8_t *curr = NULL, *next = NULL;
    idx_t len = 1, cap = 1;

    curr = (uint8_t *)malloc(sizeof(uint8_t) * cap);
    if (!curr) return (seq_t)0;          /* allocation failure */
    curr[0] = 1;                         /* seed symbol */

    /* keep expanding until the required index is inside the word */
    while (len <= n) {
        /* apply the morphism four times per batch */
        for (int step = 0; step < 4; ++step) {
            /* rough upper bound: each symbol produces at most 5 */
            idx_t new_cap = len * 5 + 5;
            next = (uint8_t *)malloc(sizeof(uint8_t) * new_cap);
            if (!next) { 
                free(curr);
                return (seq_t)0; 
            }

            idx_t pos = 0;
            for (idx_t i = 0; i < len; ++i) {
                uint8_t sym = curr[i];
                switch (sym) {
                    case 1: /* 21212 */
                        next[pos++] = 2; next[pos++] = 1;
                        next[pos++] = 2; next[pos++] = 1;
                        next[pos++] = 2;
                        break;
                    case 2: /* 3 */
                        next[pos++] = 3;
                        break;
                    case 3: /* 43434 */
                        next[pos++] = 4; next[pos++] = 3;
                        next[pos++] = 4; next[pos++] = 3;
                        next[pos++] = 4;
                        break;
                    case 4: /* 1 */
                        next[pos++] = 1;
                        break;
                    default:
                        /* should never occur */
                        break;
                }
            }

            free(curr);
            curr = next;
            len  = pos;
        }
    }

    /* retrieve the requested symbol and clean up */
    seq_t result = (seq_t)curr[n];
    free(curr);
    return result;
}
