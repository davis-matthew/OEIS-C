/*
 * A130281  Integers n > 1 such that A130280(4n^2) < n, i.e., there is an m <
 *          n, m > 1 such that 4n^2(m^2 - 1) + 1 is a square.
 * https://oeis.org/A130281
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 32, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A130281:  integers n>1 such that there is an m,
    1<m<n, with 4n²(m²–1)+1 a perfect square.  The terms are produced
    by generating the second solution of the Pell equation
    t² – (4(m²–1))n² = 1 for every m>1 and collecting the n-values,
    sorted by size and deduplicated.  A min‑heap is used to merge the
    infinite sequences of solutions of each m.  */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Smallest m to initialise (m>1) */
#define MAX_M 40000

/* ------------------------------------------------------------ */
/*  Node holding a Pell state for a given m                     */
/* ------------------------------------------------------------ */
typedef struct {
    seq_t t_cur;   /* current t value   */
    seq_t n_cur;   /* current n value   */
    seq_t t1;      /* fundamental t    */
    seq_t n1;      /* fundamental n (m)*/
    seq_t D;       /* 4(m²-1)           */
} node_t;

/* ------------------------------------------------------------ */
/*  Global state (kept static for repeated calls)               */
/* ------------------------------------------------------------ */
static bool      initialized = false;
static seq_t    *cache      = NULL;
static size_t    cache_len  = 0;
static size_t    cache_cap  = 0;

static node_t   nodes[MAX_M];
static int      heap[MAX_M];
static size_t   heap_size  = 0;

/* ------------------------------------------------------------ */
/*  Heap helper: push a node index onto the min‑heap            */
/* ------------------------------------------------------------ */
static void heap_push(int idx)
{
    size_t i = heap_size++;
    while (i > 0) {
        size_t parent = (i - 1) >> 1;
        if (nodes[heap[parent]].n_cur <= nodes[idx].n_cur)
            break;
        heap[i] = heap[parent];
        i = parent;
    }
    heap[i] = idx;
}

/* ------------------------------------------------------------ */
/*  Heap helper: pop the node with smallest n_cur               */
/* ------------------------------------------------------------ */
static int heap_pop(void)
{
    int ret  = heap[0];
    int last = heap[--heap_size];
    size_t i = 0;
    while (1) {
        size_t left = (i << 1) + 1;
        if (left >= heap_size)
            break;
        size_t right   = left + 1;
        size_t smallest = left;
        if (right < heap_size &&
            nodes[heap[right]].n_cur < nodes[heap[left]].n_cur)
            smallest = right;
        if (nodes[heap[smallest]].n_cur >= nodes[last].n_cur)
            break;
        heap[i] = heap[smallest];
        i = smallest;
    }
    heap[i] = last;
    return ret;
}

/* ------------------------------------------------------------ */
/*  Grow the cache array                                           */
/* ------------------------------------------------------------ */
static void cache_grow(void)
{
    if (cache_cap == 0)
        cache_cap = 16;
    else
        cache_cap <<= 1;
    cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
}

/* ------------------------------------------------------------ */
/*  Initialise the nodes and the heap                            */
/* ------------------------------------------------------------ */
static void init(void)
{
    heap_size = 0;
    for (size_t i = 0, m = 2; m <= MAX_M; ++m, ++i) {
        seq_t m_seq = (seq_t)m;
        seq_t n1    = m_seq;
        seq_t t1    = 2 * m_seq * m_seq - (seq_t)1;
        seq_t D     = 4 * (m_seq * m_seq - (seq_t)1);

        /* second solution of the Pell equation (k=2) */
        seq_t t_cur = t1 * t1 + D * n1 * n1;
        seq_t n_cur = 2 * t1 * n1;

        nodes[i].t_cur = t_cur;
        nodes[i].n_cur = n_cur;
        nodes[i].t1    = t1;
        nodes[i].n1    = n1;
        nodes[i].D     = D;
        heap_push((int)i);
    }

    cache_len = 0;
    cache_cap = 0;
    cache     = NULL;
    initialized = true;
}

/* ------------------------------------------------------------ */
/*  Public function a(idx_t n)                                   */
/* ------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    if (!initialized)
        init();

    while (cache_len < (size_t)n) {
        int nodeIdx = heap_pop();
        seq_t cand = nodes[nodeIdx].n_cur;

        /* keep unique values */
        if (cache_len == 0 || cand != cache[cache_len - 1]) {
            if (cache_len == cache_cap)
                cache_grow();
            cache[cache_len++] = cand;
        }

        /* advance this Pell sequence to the next solution */
        seq_t t_cur = nodes[nodeIdx].t_cur;
        seq_t n_cur = nodes[nodeIdx].n_cur;
        seq_t t1    = nodes[nodeIdx].t1;
        seq_t n1    = nodes[nodeIdx].n1;
        seq_t D     = nodes[nodeIdx].D;

        seq_t t_next = t_cur * t1 + D * n_cur * n1;
        seq_t n_next = t_cur * n1 + t1 * n_cur;

        nodes[nodeIdx].t_cur = t_next;
        nodes[nodeIdx].n_cur = n_next;

        heap_push(nodeIdx);
    }

    return cache[n - 1];
}
