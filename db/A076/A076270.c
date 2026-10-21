/*
 * A076270  Numbers of the form (3^{mr}-1)/(3^r-1) for positive integers m, r.
 * https://oeis.org/A076270
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = numbers of the form (3^{m r}−1)/(3^r−1) for positive
 *            integers m,r
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

struct Node {
    seq_t val;
    idx_t r;
    idx_t m;
};

static bool init_done = false;
static size_t heap_size;
static struct Node *heap_nodes = NULL;
static seq_t *pow3 = NULL;          /* pow3[r] = 3^r */
static idx_t R_MAX_GLOBAL = 0;

static size_t cache_size = 0;
static size_t cache_cap  = 0;
static seq_t *cache      = NULL;

/* Compare two heap nodes (min‑heap) */
static inline bool less_node(const struct Node *a, const struct Node *b)
{
    if (a->val < b->val) return true;
    if (b->val < a->val) return false;
    return a->r < b->r;   /* tie‑breaker, deterministic */
}

static inline void swap_nodes(size_t i, size_t j)
{
    struct Node tmp = heap_nodes[i];
    heap_nodes[i] = heap_nodes[j];
    heap_nodes[j] = tmp;
}

/* Build min‑heap property from index idx downwards */
static void heapify_down(size_t idx)
{
    for (;;)
    {
        size_t smallest = idx;
        size_t left = 2*idx + 1;
        size_t right = left + 1;

        if (left < heap_size && less_node(&heap_nodes[left], &heap_nodes[smallest]))
            smallest = left;
        if (right < heap_size && less_node(&heap_nodes[right], &heap_nodes[smallest]))
            smallest = right;
        if (smallest == idx) break;

        swap_nodes(idx, smallest);
        idx = smallest;
    }
}

/* Ensure cache can store at least 'need' elements */
static void ensure_cache_cap(size_t need)
{
    if (need <= cache_cap) return;
    size_t new_cap = need * 2;
    if (new_cap < 64) new_cap = 64;
    seq_t *new_buf = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache = new_buf;
    cache_cap = new_cap;
}

/* Initialise heap, pow3 array and cache (once) */
static void init_once(void)
{
    if (init_done) return;

    /* --- Pre‑compute powers of 3 up to the first overflow or 2000 --- */
    const idx_t MAX_R = 2000;
    pow3 = (seq_t *)malloc((MAX_R + 1) * sizeof(seq_t));
    pow3[0] = (seq_t)1;
    idx_t r_limit = 0;

    for (idx_t i = 1; i <= MAX_R; ++i) {
        seq_t nxt = pow3[i-1] * (seq_t)3;
        /* Detect overflow: for unsigned types wrap around, so nex < prev if overflow */
        bool overflow = nxt < pow3[i-1];
        if (overflow) {
            r_limit = i - 1;   /* do not store the overflowing value */
            break;
        }
        pow3[i] = nxt;
        if (i == MAX_R) r_limit = i;
    }

    R_MAX_GLOBAL = r_limit;

    /* --- Initialise heap with m = 1 for each r --- */
    heap_size = R_MAX_GLOBAL;
    heap_nodes = (struct Node *)malloc(heap_size * sizeof(struct Node));
    for (idx_t i = 1; i <= R_MAX_GLOBAL; ++i) {
        heap_nodes[i-1].val = (seq_t)1;
        heap_nodes[i-1].r   = i;
        heap_nodes[i-1].m   = 1;
    }
    /* Build min‑heap */
    for (size_t i = heap_size/2; i-- > 0; )
        heapify_down(i);

    /* --- Initialise cache --- */
    cache_cap = 0;
    cache     = NULL;
    cache_size = 0;

    init_done = true;
}

/* Generate terms up to index 'n' (1‑based) */
static void generate_terms(idx_t n)
{
    while (cache_size < (size_t)n) {
        /* Extract current minimum */
        struct Node min = heap_nodes[0];
        seq_t val = min.val;

        /* Store unique value */
        if (cache_size == 0 || val != cache[cache_size - 1]) {
            ensure_cache_cap(cache_size + 1);
            cache[cache_size++] = val;
        }

        /* Advance in the same stream (same r) */
        idx_t r = heap_nodes[0].r;
        heap_nodes[0].m += 1;
        seq_t old_val = heap_nodes[0].val;
        heap_nodes[0].val = old_val * pow3[r] + (seq_t)1;
        heapify_down(0);
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* offset is 1, but guard against 0 */
    init_once();
    generate_terms(n);
    return cache[n-1];
}
