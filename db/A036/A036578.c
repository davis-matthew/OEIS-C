/*
 * A036578  Ternary Thue-Morse sequence: closed under a->abc, b->ac, c->b.
 * https://oeis.org/A036578
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: iterate the morphism
   0 → 12,  1 → 102,  2 → 0
   starting with the symbol 1.  The nth element of the infinite fixed
   point is returned. */
#include <stdlib.h>

/*------ mapping of the morphism ------------------------------------*/
static const seq_t map0[2] = { 1, 2 };      /* 0 → 12 */
static const seq_t map1[3] = { 1, 0, 2 };   /* 1 → 102 */
static const seq_t map2[1] = { 0 };         /* 2 → 0 */

static const seq_t * const rep_map[3] = { map0, map1, map2 };
static const size_t rep_len[3]  = { 2, 3, 1 };

/*------ sequence generator ----------------------------------------*/
seq_t a(idx_t n)
{
    /* static state to rebuild only when necessary */
    static seq_t *current = NULL;      /* buffer containing the word   */
    static size_t cur_len = 0;         /* current length of the word   */

    /* initialise the first word (just the starting symbol 1) */
    if (current == NULL) {
        current = (seq_t *)malloc(sizeof(seq_t));
        current[0] = 1;
        cur_len = 1;
    }

    /* expand the word until it contains at least n+1 symbols */
    while (cur_len <= (size_t)n) {
        size_t new_len = 0;
        size_t i;

        /* compute length of the expanded word */
        for (i = 0; i < cur_len; ++i)
            new_len += rep_len[(size_t)current[i]];

        /* allocate buffer for the expansion */
        seq_t *next = (seq_t *)malloc(new_len * sizeof(seq_t));

        /* fill the next buffer with the expansion */
        size_t pos = 0;
        for (i = 0; i < cur_len; ++i) {
            seq_t sym  = current[i];
            const seq_t *rep = rep_map[(size_t)sym];
            size_t rlen  = rep_len[(size_t)sym];
            size_t j;
            for (j = 0; j < rlen; ++j)
                next[pos++] = rep[j];
        }

        /* swap buffers */
        free(current);
        current = next;
        cur_len = new_len;
    }

    return current[(size_t)n];
}
