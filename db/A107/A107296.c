/*
 * A107296  1-limiting word of the morphism 1->1311, 2->13, 3->2.
 * https://oeis.org/A107296
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

/* Sequence A107296: 1-limiting word of the morphism
 *   1 -> 1311
 *   2 -> 13
 *   3 -> 2
 *
 * a(n) is the nth letter of the infinite word obtained by starting from the
 * single symbol "1" and repeatedly applying the substitution.  The function
 * computes a(n) by first determining the minimal number of substitution
 * levels needed to contain position n and then descending the substitution
 * tree to locate the letter corresponding to that position.  The method
 * runs in O(log n) time (the substitution growth factor is ≈3–4).
 */

#include <stdint.h>
#include <stdbool.h>

/* Maximum depth needed for a 64‑bit index is < 200, which is ample. */
#define MAX_DEPTH 200

/* 128‑bit integers are used for the length calculations. */
static unsigned __int128 len1[MAX_DEPTH];
static unsigned __int128 len2[MAX_DEPTH];
static unsigned __int128 len3[MAX_DEPTH];
static bool lengths_prepared = false;

/* Prepare the length tables once. */
static void prepare_lengths(void)
{
    if (lengths_prepared)
        return;

    /* Base case: a single symbol has length 1. */
    len1[0] = 1;
    len2[0] = 1;
    len3[0] = 1;

    for (int d = 1; d < MAX_DEPTH; ++d) {
        len1[d] = 3 * len1[d - 1] + len3[d - 1];
        len2[d] = len1[d - 1] + len3[d - 1];
        len3[d] = len2[d - 1];
    }
    lengths_prepared = true;
}

/* Recursive descent to locate the symbol at position pos in the word
 * obtained after 'depth' further expansions of the symbol 'sym'.
 *
 * 'sym' is 1, 2, or 3.  'depth' counts the remaining substitution levels.
 * If depth==0 we simply return sym. */
static seq_t descend(idx_t pos, int depth, int sym)
{
    if (depth == 0) {
        return (seq_t)sym;
    }

    /* Helper lambda to test a child segment. */
    const idx_t len1d = (idx_t)len1[depth - 1];
    const idx_t len2d = (idx_t)len2[depth - 1];
    const idx_t len3d = (idx_t)len3[depth - 1];

    if (sym == 1) {
        /* 1 -> 1 3 1 1 */
        if (pos < len1d) {
            return descend(pos, depth - 1, 1);
        }
        pos -= len1d;
        if (pos < len3d) {
            return descend(pos, depth - 1, 3);
        }
        pos -= len3d;
        if (pos < len1d) {
            return descend(pos, depth - 1, 1);
        }
        pos -= len1d; /* third '1' block */
        return descend(pos, depth - 1, 1);
    } else if (sym == 2) {
        /* 2 -> 1 3 */
        if (pos < len1d) {
            return descend(pos, depth - 1, 1);
        }
        pos -= len1d;
        return descend(pos, depth - 1, 3);
    } else /* sym == 3 */ {
        /* 3 -> 2 */
        return descend(pos, depth - 1, 2);
    }
}

/* Public function implementing the OEIS sequence. */
seq_t a(idx_t n)
{
    prepare_lengths();

    /* Find minimal depth with length > n. */
    int depth = 0;
    while ((unsigned __int128)len1[depth] <= (unsigned __int128)n)
        ++depth;

    /* Descend from the initial symbol '1'. */
    return descend(n, depth, 1);
}
