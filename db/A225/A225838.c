/*
 * A225838  Numbers of form 2^i*3^j*(6k+5), i, j, k >= 0.
 * https://oeis.org/A225838
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 52, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A225838: numbers of the form 2^i * 3^j * (6k + 5).
   The function returns the n-th smallest such number (a(1) = 5).  The
   algorithm merges the arithmetic progressions
   (6k+5)*m for all 3‑smooth numbers m = 2^i * 3^j using a min‑heap. */

#include <stddef.h>
#include <stdlib.h>

/* Node used in the heap. */
typedef struct {
    seq_t val; /* current candidate value */
    seq_t m;   /* underlying 3‑smooth multiplier */
} Node;

/* Minimum‑heap sift‑down operation. */
static void sift_down(Node *h, size_t size, size_t idx)
{
    while (1)
    {
        size_t left  = (idx << 1) + 1;
        size_t right = left + 1;
        size_t smallest = idx;

        if (left < size && h[left].val < h[smallest].val)
            smallest = left;
        if (right < size && h[right].val < h[smallest].val)
            smallest = right;

        if (smallest == idx)
            break;

        Node temp = h[idx];
        h[idx] = h[smallest];
        h[smallest] = temp;
        idx = smallest;
    }
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0; /* undefined for n = 0 */

    /* Upper bound for the 3‑smooth multiplier m.
       Since the n‑th term is ≈ 2n, all useful m satisfy 5*m ≤ 2n,
       i.e. m ≤ 2n/5.  Use a modest safety factor. */
    idx_t m_max = (n * (idx_t)4) / (idx_t)5 + (idx_t)10;

    /* Generate all 3‑smooth numbers ≤ m_max. */
    size_t ms_cap = 16;
    seq_t *ms = (seq_t *)malloc(ms_cap * sizeof(seq_t));
    size_t ms_cnt = 0;

    for (idx_t p2 = 1; p2 <= m_max; p2 <<= 1)
    {
        for (idx_t p3 = 1; p3 <= m_max / p2; p3 *= 3)
        {
            if (ms_cnt == ms_cap)
            {
                ms_cap <<= 1;
                ms = (seq_t *)realloc(ms, ms_cap * sizeof(seq_t));
            }
            ms[ms_cnt++] = (seq_t)p2 * (seq_t)p3;
        }
    }

    /* Build a min‑heap: each node is the next candidate of a progression. */
    Node *heap = (Node *)malloc(ms_cnt * sizeof(Node));
    for (size_t i = 0; i < ms_cnt; ++i)
    {
        heap[i].m   = ms[i];
        heap[i].val = (seq_t)5 * ms[i];
    }
    free(ms); /* no longer needed */

    /* Heapify. */
    for (size_t i = (ms_cnt >> 1); i > 0; --i)
        sift_down(heap, ms_cnt, i - 1);

    /* Extract the minimum n times. */
    seq_t result = 0;
    for (idx_t step = 1; step <= n; ++step)
    {
        result = heap[0].val;
        heap[0].val += (seq_t)6 * heap[0].m; /* advance to next term in the same progression */
        sift_down(heap, ms_cnt, 0);
    }

    free(heap);
    return result;
}
