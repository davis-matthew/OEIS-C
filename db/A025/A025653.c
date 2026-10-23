/*
 * A025653  Exponent of 5 (value of i) in n-th number of form 5^i*8^j.
 * https://oeis.org/A025653
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = exponent of 5 in the n‑th smallest number of the form
 *            5^i*8^j
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

/* ---------- constants ----------
   We keep the natural logarithms of 5 and 8 as compile‑time constants.
   They are used only for ordering the heap; no arithmetic on seq_t is
   performed with doubles.  ---------------------------------------------------*/
static const double LOG5 = 1.60943791243410037460; /* ln(5)  */
static const double LOG8 = 2.07944154167983592890; /* ln(8)  */

/* ---------- cache of results ----------
   cache holds a(n) values for 1…cache_size.  ---------------------------------------------------*/
static seq_t   *cache    = NULL;
static idx_t    cache_size = 0;
static idx_t    cache_cap  = 0;

/* ---------- visited table ----------
   A small static table that marks whether a pair (i,j) has already
   been enqueued.  The size is chosen to be well above the
   expected exponents for n ≤ 10000.  ----------------------------------------------*/
#define VIS_SIZE 500
static uint8_t visited[VIS_SIZE][VIS_SIZE] = {0};

/* ---------- heap of nodes ----------
   Each node contains the i exponent of 5, the j exponent of 8,
   and the sum i*log5 + j*log8 stored as a double to allow
   comparisons.  -----------------------------------------------*/
typedef struct {
    double val;      /* i*log5 + j*log8           */
    idx_t   i;       /* exponent of 5              */
    idx_t   j;       /* exponent of 8              */
} Node;

static Node *heap       = NULL;
static size_t heap_size = 0;
static size_t heap_cap  = 0;

/* ---------- heap helper functions ----------*/
static inline void heap_swap(Node *a, Node *b)
{
    Node tmp = *a; *a = *b; *b = tmp;
}

static void heap_push(double val, idx_t i, idx_t j)
{
    /* grow if needed */
    if (heap_size == heap_cap) {
        heap_cap = heap_cap ? heap_cap << 1 : 16;
        heap = (Node *)realloc(heap, heap_cap * sizeof(Node));
    }
    heap[heap_size++] = (Node){val, i, j};

    /* sift up */
    size_t idx = heap_size - 1;
    while (idx) {
        size_t parent = (idx - 1) >> 1;
        if (heap[parent].val <= heap[idx].val) break;
        heap_swap(&heap[parent], &heap[idx]);
        idx = parent;
    }
}

static Node heap_pop(void)
{
    Node ret = heap[0];
    heap[0] = heap[--heap_size];

    /* sift down */
    size_t idx = 0;
    while (1) {
        size_t left = (idx << 1) + 1;
        if (left >= heap_size) break;
        size_t right = left + 1;
        size_t child = (right < heap_size && heap[right].val < heap[left].val)
                       ? right : left;
        if (heap[idx].val <= heap[child].val) break;
        heap_swap(&heap[idx], &heap[child]);
        idx = child;
    }
    return ret;
}

/* ---------- main sequence function ----------*/
seq_t a(idx_t n)
{
    /* one‑time initialisation */
    static int initialized = 0;
    if (!initialized) {
        cache_cap = 64;
        cache     = (seq_t *)malloc(cache_cap * sizeof(seq_t));

        heap_cap = 16;
        heap     = (Node *)malloc(heap_cap * sizeof(Node));

        /* seed with (i=0, j=0) */
        visited[0][0] = 1;
        heap_push(0.0, 0, 0);                 /* log(1)=0   */
        initialized = 1;
    }

    /* generate sequence entries until we have at least n of them */
    while (cache_size < n) {
        Node cur = heap_pop();

        /* store exponent of 5 */
        if (cache_size == cache_cap) {
            cache_cap = cache_cap << 1;
            cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
        }
        cache[cache_size++] = (seq_t)cur.i;

        /* push child (i+1, j) if not visited and within bounds */
        idx_t ni = cur.i + 1;
        idx_t nj = cur.j;
        if (ni < VIS_SIZE && !visited[ni][nj]) {
            visited[ni][nj] = 1;
            heap_push(cur.val + LOG5, ni, nj);
        }

        /* push child (i, j+1) if not visited and within bounds */
        ni = cur.i;
        nj = cur.j + 1;
        if (nj < VIS_SIZE && !visited[ni][nj]) {
            visited[ni][nj] = 1;
            heap_push(cur.val + LOG8, ni, nj);
        }
    }

    return cache[n - 1];
}
