/*
 * A237667  Number of partitions of n such that no part is a sum of two or
 *          more other parts.
 * https://oeis.org/A237667
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 24, u16 n <= 84, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A237667
 *  Count of partitions of n in which no part equals the sum of at least
 *  two other parts.
 *
 *  The algorithm enumerates partitions in non‑decreasing order and keeps
 *  two bitsets:
 *
 *      B – all achievable subset sums with at least one part;
 *      S – all achievable subset sums with at least two parts.
 *
 *  A new part x is allowed iff the bit for x is not set in S.
 *  Adding x updates B and S by shifting B by x and ORing.
 *  The changes are stored on an undo stack so the recursion can backtrack.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct {
    uint16_t idx;       /* word index */
    uint8_t   isB;      /* 1 → stored old value of B, 0 → old value of S */
    uint8_t   pad;      /* padding to align to 8 bytes */
    uint64_t  old;      /* old word value before modification */
} Change;

static uint64_t *B_global, *S_global;
static int WORDS_global;
static Change *changes_global;
static size_t change_top;
static seq_t count_global;

/* Is bit 'pos' set in 'arr'? */
static inline bool bit_is_set(const uint64_t *arr, int pos)
{
    return (arr[pos >> 6] >> (pos & 63)) & 1U;
}

/* Depth‑first search over partitions */
static void dfs(int remain, int min_part)
{
    if (remain == 0) {
        ++count_global;
        return;
    }
    for (int x = min_part; x <= remain; ++x) {
        /* Reject part if it equals the sum of at least two earlier parts */
        if (bit_is_set(S_global, x))
            continue;

        size_t pre_top = change_top;          /* remember stack depth */
        int shift_words = x >> 6;
        int shift_bits  = x & 63;

        /* Update B and S with the new part */
        for (int i = WORDS_global - 1; i >= 0; --i) {
            uint64_t shifted = 0;
            int idx1 = i - shift_words;
            if (idx1 >= 0) {
                shifted = B_global[idx1] << shift_bits;
                if (shift_bits && idx1 - 1 >= 0)
                    shifted |= B_global[idx1 - 1] >> (64 - shift_bits);
            }

            uint64_t oldS = S_global[i];
            uint64_t newS = oldS | shifted;
            if (newS != oldS) {
                changes_global[change_top++] = (Change){(uint16_t)i, 0, 0, oldS};
                S_global[i] = newS;
            }

            uint64_t oldB = B_global[i];
            uint64_t newB = oldB | shifted;
            if (newB != oldB) {
                changes_global[change_top++] = (Change){(uint16_t)i, 1, 0, oldB};
                B_global[i] = newB;
            }
        }

        /* Set the bit for the part itself in B */
        int idx = x >> 6;
        uint64_t mask = 1ULL << (x & 63);
        uint64_t oldB = B_global[idx];
        if ((oldB & mask) == 0) {
            changes_global[change_top++] = (Change){(uint16_t)idx, 1, 0, oldB};
            B_global[idx] = oldB | mask;
        }

        dfs(remain - x, x);

        /* Undo all changes made while processing part x */
        while (change_top > pre_top) {
            Change ch = changes_global[--change_top];
            if (ch.isB)
                B_global[ch.idx] = ch.old;
            else
                S_global[ch.idx] = ch.old;
        }
    }
}

/* public function required by the framework */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;          /* empty partition */

    int N = (int)n;
    int WORDS = (N + 64) >> 6;     /* words needed for bits 0 … n */

    /* allocate working bit‑sets (C++ requires explicit cast) */
    B_global = (uint64_t *)calloc((size_t)WORDS, sizeof(uint64_t));
    S_global = (uint64_t *)calloc((size_t)WORDS, sizeof(uint64_t));
    if (!B_global || !S_global) {
        free(B_global);
        free(S_global);
        return (seq_t)0;          /* allocation failed */
    }

    WORDS_global = WORDS;
    count_global = 0;
    change_top   = 0;

    /* size of undo stack: each part may touch at most 2*WORDS words */
    size_t max_changes = ((size_t)2 * WORDS + 1) * (size_t)(N + 1) + 10;
    changes_global = (Change *)malloc(max_changes * sizeof(Change));
    if (!changes_global) {
        free(B_global);
        free(S_global);
        return (seq_t)0;          /* allocation failed */
    }

    dfs(N, 1);

    /* cleanup */
    free(B_global);
    free(S_global);
    free(changes_global);

    return count_global;
}
