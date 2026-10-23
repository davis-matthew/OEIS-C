/*
 * A138671  Indices of 1's in Stewart's choral sequence.
 * https://oeis.org/A138671
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 3^{m+1}n + (5·3^m−1)/2 for m,n ≥ 0; the sequence consists
 *            of all such values sorted in increasing order
 * Range:     u8 n <= 5, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

/* ------------------------------------------------------------ */
/*  Static tables for the formal progressions  (m = 0…MAX_M-1)  */
/* ------------------------------------------------------------ */
#define MAX_M 30                            /* enough for n ≈ 10 000 */

static bool initialized = false;

/* For each m: cur_val[m] = current term of the progression,
   step_arr[m] = 3^{m+1} (the step between successive terms). */
static seq_t *cur_val = NULL;
static seq_t *step_arr = NULL;

/* ------------------------------------------------------------ */
/*  Min‑heap of the current front term of every progression    */
/* ------------------------------------------------------------ */
typedef struct {
    seq_t val;      /* current value of the progression */
    int   m;        /* progression index            */
} heap_node;

static heap_node *heap = NULL;    /* array of heap nodes      */
static int heap_size = 0;         /* current heap size        */

/* swap two nodes in the heap */
static void heap_swap(heap_node *a, heap_node *b)
{
    heap_node tmp = *a;
    *a = *b;
    *b = tmp;
}

/* push a node into the heap (heap invariant maintained) */
static void heap_push(seq_t val, int m)
{
    int i = heap_size++;
    /* sift up */
    while (i > 0) {
        int parent = (i - 1) >> 1;
        if (val < heap[parent].val) {
            heap[i] = heap[parent];
            i = parent;
        } else break;
    }
    heap[i].val = val;
    heap[i].m   = m;
}

/* pop the smallest node from the heap (heap invariant maintained) */
static heap_node heap_pop(void)
{
    heap_node root = heap[0];
    heap_node last = heap[--heap_size];
    int i = 0;

    /* sift down */
    while (1) {
        int left = (i << 1) + 1;
        if (left >= heap_size) break;
        int right = left + 1;
        int child = (right < heap_size && heap[right].val < heap[left].val)
                        ? right : left;

        if (last.val > heap[child].val) {
            heap[i] = heap[child];
            i = child;
        } else break;
    }
    heap[i] = last;
    return root;
}

/* ------------------------------------------------------------ */
/*  Memoization of produced terms                               */
/* ------------------------------------------------------------ */
static seq_t *cache = NULL;
static size_t cache_len = 0, cache_cap = 0;

/* grow cache if necessary */
static void ensure_cache_capacity(size_t needed)
{
    if (cache_cap >= needed) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    if (new_cap < needed) new_cap = needed;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();                 /* allocation failure */
    cache = tmp;
    cache_cap = new_cap;
}

/* ------------------------------------------------------------ */
/*  Initialization of progressions and heap                    */
/* ------------------------------------------------------------ */
static void init_progressions(void)
{
    /* allocate dynamic arrays */
    cur_val  = (seq_t *)malloc(MAX_M * sizeof(seq_t));
    step_arr = (seq_t *)malloc(MAX_M * sizeof(seq_t));
    heap     = (heap_node *)malloc(MAX_M * sizeof(heap_node));
    if (!cur_val || !step_arr || !heap) abort();

    /* initialise each progression */
    seq_t pow3 = (seq_t)1;            /* 3^0 */
    for (int m = 0; m < MAX_M; ++m) {
        cur_val[m]  = ((seq_t)5 * pow3 - (seq_t)1) / (seq_t)2;      /* (5·3^m−1)/2 */
        step_arr[m] = pow3 * (seq_t)3;                              /* 3^{m+1}   */
        pow3 = pow3 * (seq_t)3;                                    /* next power */
    }

    /* build initial heap */
    heap_size = MAX_M;
    for (int i = 0; i < MAX_M; ++i) {
        heap[i].val = cur_val[i];
        heap[i].m   = i;
    }
    /* heapify (min‑heap) */
    for (int i = (heap_size >> 1) - 1; i >= 0; --i) {
        int j = i;
        while (1) {
            int left = (j << 1) + 1;
            if (left >= heap_size) break;
            int right = left + 1;
            int child = (right < heap_size && heap[right].val < heap[left].val)
                            ? right : left;

            if (heap[j].val > heap[child].val) {
                heap_swap(&heap[j], &heap[child]);
                j = child;
            } else break;
        }
    }

    initialized = true;
}

/* ------------------------------------------------------------ */
/*  Public function                                            */
/* ------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (!initialized) init_progressions();

    /* grow memoization cache if needed */
    if (cache_len <= (size_t)n) {
        ensure_cache_capacity(n + 1);

        /* generate terms until we reach index n */
        seq_t last_val = (cache_len > 0) ? cache[cache_len - 1] : 0;  /* dummy */
        bool first = (cache_len == 0);

        while (cache_len <= (size_t)n) {
            heap_node node = heap_pop();
            seq_t val = node.val;
            int m = node.m;

            /* store unique term */
            if (first || val != last_val) {
                cache[cache_len++] = val;
                last_val = val;
                first = false;
            }

            /* advance this progression by its step */
            heap_push(val + step_arr[m], m);
        }
    }

    return cache[(size_t)n];
}
