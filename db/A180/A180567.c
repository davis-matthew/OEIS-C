/*
 * A180567  The Wiener index of the Fibonacci tree of order n.
 * https://oeis.org/A180567
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 19, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   seq_alloc_array, seq_free
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Wiener index of the Fibonacci tree of order n
    Recurrence (for n >= 2):
        N(0) = 1, N(1) = 1
        N(n) = 1 + N(n‑1) + N(n‑2)                     // number of nodes
        D(0) = 0, D(1) = 0
        D(n) = N(n‑1) + D(n‑1) + N(n‑2) + D(n‑2)       // sum of distances from root
        W(0) = 0, W(1) = 0
        W(n) = W(n‑1) + W(n‑2) + D(n)
               + N(n‑2)*D(n‑1) + N(n‑1)*D(n‑2) + 2*N(n‑1)*N(n‑2)
*/

#include <stddef.h>                 /* for size_t */
#include "../utils/seq_alloc_array.h"
#include "../utils/seq_free.h"

static seq_t *nodes   = NULL;   /* N array: number of nodes                  */
static seq_t *dist    = NULL;   /* D array: sum of distances from root       */
static seq_t *wiener  = NULL;   /* W array: Wiener index                     */
static size_t cap     = 0;       /* current capacity of the arrays            */

/* Ensure the arrays have space for indices up to n (inclusive). */
static void ensure_capacity(idx_t n)
{
    if (cap > 0 && n < (idx_t)cap) return;

    /* Determine the new capacity (next power of two >= n+1). */
    size_t new_cap = cap < 2 ? 2 : cap;
    while (new_cap <= (size_t)n) new_cap <<= 1;

    /* Allocate new arrays. */
    seq_t *new_nodes   = seq_alloc_array(new_cap);
    seq_t *new_dist    = seq_alloc_array(new_cap);
    seq_t *new_wiener  = seq_alloc_array(new_cap);

    /* Copy existing values if any. */
    if (nodes)
    {
        for (size_t i = 0; i < cap; ++i) {
            new_nodes[i]   = nodes[i];
            new_dist[i]    = dist[i];
            new_wiener[i]  = wiener[i];
        }
        seq_free(nodes);
        seq_free(dist);
        seq_free(wiener);
    }

    nodes   = new_nodes;
    dist    = new_dist;
    wiener  = new_wiener;
    cap     = new_cap;

    /* Initialize base cases once on first allocation. */
    if (cap >= 2) {
        nodes[0]   = 1;   dist[0]   = 0;   wiener[0]   = 0;
        nodes[1]   = 1;   dist[1]   = 0;   wiener[1]   = 0;
    }
}

/* Compute all values up to index n inclusive. */
static void compute_upto(idx_t n)
{
    ensure_capacity(n);
    for (idx_t i = 2; i <= n; ++i)
    {
        /* N(i) = 1 + N(i‑1) + N(i‑2) */
        nodes[i] = nodes[i-1] + nodes[i-2] + 1;

        /* D(i) = N(i‑1) + D(i‑1) + N(i‑2) + D(i‑2) */
        dist[i] = nodes[i-1] + dist[i-1] + nodes[i-2] + dist[i-2];

        /* cross term: N(i‑2)*D(i‑1) + N(i‑1)*D(i‑2) + 2*N(i‑1)*N(i‑2) */
        seq_t cross = nodes[i-2] * dist[i-1];
        cross += nodes[i-1] * dist[i-2];
        cross += 2 * nodes[i-1] * nodes[i-2];

        /* W(i) = W(i‑1) + W(i‑2) + D(i) + cross */
        wiener[i] = wiener[i-1] + wiener[i-2] + dist[i] + cross;
    }
}

/* Public function required by the exercise. */
seq_t a(idx_t n)
{
    compute_upto(n);
    return wiener[n];
}
