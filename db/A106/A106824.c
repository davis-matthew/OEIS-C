/*
 * A106824  Trajectory of 1 under the morphism 1->13, 2->13223, 3->1323.
 * https://oeis.org/A106824
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   This function implements the morphic sequence A106824.
   Definition: Start with the word "1". In each step replace
   every symbol according to the morphism:
       1 → 1 3
       2 → 1 3 2 2 3
       3 → 1 3 2 3
   The resulting fixed point is the infinite word 1 3 1 3 2 3 ...
   a(n) is the nth symbol of this word (0‑based indexing).
*/
#include <stdint.h>
#include <stdlib.h>

/* mapping tables */
static const uint8_t map1[2] = {1, 3};
static const uint8_t map2[5] = {1, 3, 2, 2, 3};
static const uint8_t map3[4] = {1, 3, 2, 3};

/* external linkage as required */
seq_t a(idx_t n)
{
    /* generate the word until length > n */
    size_t size = 1;
    uint8_t *cur = (uint8_t*)malloc(sizeof(uint8_t));
    if (!cur) return (seq_t)0;     /* allocation failure – unlikely */
    cur[0] = 1;

    while (size <= (size_t)n) {
        /* next size is at most 5 times the current size */
        size_t est = size * 5 + 1;
        uint8_t *next = (uint8_t*)realloc(cur, est * sizeof(uint8_t));
        if (!next) {
            free(cur);
            return (seq_t)0;     /* allocation failure */
        }
        cur = next;

        size_t next_len = 0;
        for (size_t i = 0; i < size; ++i) {
            const uint8_t *map;
            size_t map_len;
            switch (cur[i]) {
                case 1: map = map1; map_len = 2; break;
                case 2: map = map2; map_len = 5; break;
                case 3: map = map3; map_len = 4; break;
                default: map = map1; map_len = 2; break; /* shouldn't happen */
            }
            for (size_t j = 0; j < map_len; ++j) {
                cur[next_len++] = map[j];
            }
        }
        size = next_len;
    }

    uint8_t result = cur[(size_t)n];
    free(cur);
    return (seq_t)result;
}
