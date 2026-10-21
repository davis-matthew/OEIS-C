/*
 * A069716  Smallest number such that the LCM of the digits equals n, or 0 if
 *          no such number exists.
 * https://oeis.org/A069716
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 2519, u32 n <= 2520, u64 n <= 2520,
 *            u128 n <= 2520, big any n
 * Helpers:   gcd_int, lcm_int
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: a(n) = smallest integer whose decimal digits' LCM equals n,
   or 0 if no such integer exists.  The function pre‑computes the best
   value for every LCM ≤ 2520 using a Dijkstra‑style search over the
   allowed digits 1–9.  This is sufficient because lcm{1,…,9} = 2520
   and any larger n has no solution. */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/gcd_int.h"
#include "../utils/lcm_int.h"

/* Euclidean GCD for small integers.  The LCM values never exceed 2520. */

/* Node used in the priority queue: the current integer value and the
   current LCM of its decimal digits. */
typedef struct {
    seq_t val;  /* current value */
    int   lc;   /* current LCM of its digits */
} node_t;

/* Push a node onto a binary min‑heap. */
static inline void heap_push(node_t **heap, size_t *cap, size_t *size, node_t key)
{
    if (*size >= *cap) {
        *cap = (*cap > 0) ? (*cap << 1) : 512;
        *heap = (node_t *)realloc(*heap, *cap * sizeof(node_t));
    }
    size_t i = *size;
    (*size)++;
    while (i > 0) {
        size_t p = (i - 1) >> 1;
        if ((*heap)[p].val <= key.val) break;
        (*heap)[i] = (*heap)[p];
        i = p;
    }
    (*heap)[i] = key;
}

/* Pop the smallest node from the heap. Return false if the heap was empty. */
static inline bool heap_pop(node_t **heap, size_t *size, node_t *out)
{
    if (*size == 0) return false;
    *out = (*heap)[0];
    (*size)--;
    size_t i = 0;
    while (1) {
        size_t l = (i << 1) + 1, r = l + 1, best = i;
        if (l < *size && (*heap)[l].val < (*heap)[best].val) best = l;
        if (r < *size && (*heap)[r].val < (*heap)[best].val) best = r;
        if (best == i) break;
        (*heap)[i] = (*heap)[best];
        i = best;
    }
    if (*size > 0) (*heap)[i] = (*heap)[*size];
    return true;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    /* No solution exists for n > 2520 (lcm{1..9} = 2520). */
    if (n > 2520) return (seq_t)0;

    /* Static tables are initialized once. */
    static bool initialized = false;
    static seq_t best[2521];
    static bool seen[2521];

    if (!initialized) {
        for (int i = 0; i <= 2520; ++i) {
            seen[i] = false;
            best[i] = (seq_t)0;
        }

        node_t *heap = NULL;
        size_t heap_cap = 0, heap_size = 0;

        /* Insert single‑digit starting numbers 1‑9. */
        for (int d = 1; d <= 9; ++d) {
            node_t nd = { .val = (seq_t)d, .lc = d };
            seen[d] = true;
            best[d] = nd.val;
            heap_push(&heap, &heap_cap, &heap_size, nd);
        }

        /* Dijkstra expansion over LCM states. */
        node_t cur;
        while (heap_pop(&heap, &heap_size, &cur)) {
            /* Skip entries that are not currently optimal. */
            if (!seen[cur.lc] || cur.val != best[cur.lc]) continue;

            for (int d = 1; d <= 9; ++d) {
                int nl = lcm_int(cur.lc, d);
                seq_t nv = (cur.val * (seq_t)10) + (seq_t)d;

                if (!seen[nl] || nv < best[nl]) {
                    seen[nl] = true;
                    best[nl] = nv;
                    node_t nxt = { .val = nv, .lc = nl };
                    heap_push(&heap, &heap_cap, &heap_size, nxt);
                }
            }
        }
        free(heap);
        initialized = true;
    }

    return seen[n] ? best[n] : (seq_t)0;
}
