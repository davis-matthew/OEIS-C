/*
 * A216651  Lengths of decreasing blocks of A006530, the greatest prime factor
 *          of n, starting from the second term.
 * https://oeis.org/A216651
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gpf
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A216651: lengths of decreasing blocks of A006530 (greatest prime factor) */
/* The sequence is obtained by scanning consecutive integers beginning at 2
   and recording the lengths of strictly decreasing runs of their greatest
   prime factors. */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/gpf.h"

static seq_t *blocks = NULL;         /* array of block lengths (1‑based)   */
static size_t blocks_count = 0;      /* number of blocks already stored   */
static size_t blocks_cap = 0;        /* allocated capacity of 'blocks'    */

static uint64_t current_m = 1;       /* last integer processed (>=1)      */
static uint64_t last_gpf = 0;        /* gpf of the last processed integer*/
static seq_t cur_block_len = 0;      /* length of the block currently being built */

/* append a finished block length to the cache                               */
static void push_block(seq_t len)
{
    if (blocks_count == blocks_cap) {
        size_t new_cap = blocks_cap == 0 ? 64 : blocks_cap * 2;
        seq_t *new_blocks = (seq_t *)realloc(blocks, new_cap * sizeof(seq_t));
        if (!new_blocks) {
            /* allocation failed – abort the program; for competition
               environments a signal or exit would be appropriate */
            abort();
        }
        blocks = new_blocks;
        blocks_cap = new_cap;
    }
    blocks[blocks_count++] = len;
}

/* compute a(n) for the OEIS sequence A216651                                 */
seq_t a(idx_t n)
{
    if (n == 0)        /* outside offset – value does not matter            */
        return 0;

    /* generate blocks until we have at least n of them                      */
    while (blocks_count < (size_t)n) {
        ++current_m;                       /* move to next integer             */
        uint64_t gp = gpf(current_m);       /* greatest prime factor of m        */

        if (cur_block_len == 0) {           /* starting a new block (first time)*/
            cur_block_len = 1;
            last_gpf = gp;
            continue;
        }

        if (last_gpf > gp) {                /* still decreasing               */
            cur_block_len++;
        } else {                            /* block ends – record length      */
            push_block(cur_block_len);
            cur_block_len = 1;              /* start new block with current m  */
        }
        last_gpf = gp;
    }

    /* n is 1‑based, array index is (n-1)                                   */
    return blocks[(size_t)(n - 1)];
}
