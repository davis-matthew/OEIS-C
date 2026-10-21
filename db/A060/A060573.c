/*
 * A060573  Tower of Hanoi: using the optimal way to move an even number of
 *          disks from peg 0 to peg 2 or an odd number from peg 0 to peg 1,
 *          a(n) is the smallest disk on peg 0 after n moves.
 * https://oeis.org/A060573
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) is obtained by simulating the optimal solution to the Tower
 *            of Hanoi for an even number of disks (target peg 2). The
 *            smallest disk on peg 0 after each move is recorded
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* std headers needed for types and memory allocation */
#include <stdint.h>
#include <stdlib.h>
#include <limits.h>

static seq_t *cache = NULL;        /* memoized sequence values */
static size_t cache_size = 0;      /* number of terms currently cached */
static size_t cache_cap   = 0;      /* allocated cache capacity */

/* Compute a(n), expanding the cached sequence as necessary. */
seq_t a(idx_t n) {
    size_t idx = (size_t)n;

    /* expand cache to hold at least idx+1 terms */
    if (idx >= cache_size) {
        size_t needed = idx + 1;

        /* increase allocation size if required */
        if (needed > cache_cap) {
            size_t new_cap = cache_cap ? cache_cap * 2U : 16U;
            if (new_cap < needed) new_cap = needed;
            seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
            cache = tmp;
            cache_cap = new_cap;
        }

        /* Simulate the moves up to 'needed - 1' */
        /* Use a fixed number of disks large enough for n ≤ 10000 */
        enum { MAX_D = 64 };                       /* enough for any n up to ~10k */
        int stack[3][MAX_D];
        int top[3];
        int posS = 0;               /* current peg of the smallest disk (disk 1) */

        /* initialise pegs */
        top[0] = MAX_D - 1;
        for (int d = MAX_D; d >= 1; --d)
            stack[0][MAX_D - d] = d;   /* bottom to top: MAX_D … 1 */
        top[1] = top[2] = -1;

        cache[0] = (seq_t)1;          /* before any move the smallest disk is 1 */

        for (size_t move = 1; move < needed; ++move) {
            if (move & 1U) {           /* odd move: move the smallest disk */
                int to = (posS + 1) % 3;          /* cycle 0 → 1 → 2 → 0 */
                int disk = stack[posS][top[posS]];
                top[posS]--;                      /* pop from source */
                top[to]++; stack[to][top[to]] = disk; /* push to destination */
                posS = to;
            } else {                    /* even move: the only legal move
                                            that does not involve disk 1 */
                int a = (posS + 1) % 3;
                int b = (posS + 2) % 3;
                int top_a = (top[a] >= 0) ? stack[a][top[a]] : INT_MAX;
                int top_b = (top[b] >= 0) ? stack[b][top[b]] : INT_MAX;
                int from, to;
                if (top_a <= top_b) { from = a; to = b; }
                else {              from = b; to = a; }
                int disk = stack[from][top[from]];
                top[from]--;
                top[to]++; stack[to][top[to]] = disk;
            }

            /* record the smallest disk currently on peg 0 */
            if (top[0] >= 0)
                cache[move] = (seq_t)stack[0][top[0]];
            else
                cache[move] = (seq_t)0;           /* should not occur for n ≤ 1e4 */
        }

        cache_size = needed;
    }

    return cache[idx];
}
