/*
 * A191989  Ordered sequence of nonnegative differences 2f-5g, where f and g
 *          are Lucas numbers (A000032 beginning at 1).
 * https://oeis.org/A191989
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = nth smallest non‑negative value of 2*L(f)−5*L(g) with
 *            f,g≥1 where L is the Lucas sequence (A000032) with
 *            L(1)=1,L(2)=3. The values are sorted and duplicates are removed
 * Range:     u8 n <= 8, u16 n <= 40, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* ----------  Lucas numbers  ---------- */
static seq_t *lucas_tbl = NULL;
static idx_t  lucas_len = 0;
static idx_t  lucas_cap = 0;

static inline void
ensure_lucas_upto(idx_t k)
{
    if (k < lucas_len) return;
    size_t new_cap = lucas_cap ? lucas_cap : 4;
    while ((idx_t)new_cap <= k) new_cap <<= 1;
    if (new_cap != lucas_cap) {
        seq_t *tmp = (seq_t*)realloc(lucas_tbl, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        lucas_tbl = tmp;
        lucas_cap = new_cap;
    }
    for (idx_t i = lucas_len; i <= k; ++i) {
        if (i == 1) lucas_tbl[i] = (seq_t)1;
        else if (i == 2) lucas_tbl[i] = (seq_t)3;
        else lucas_tbl[i] = lucas_tbl[i-1] + lucas_tbl[i-2];
    }
    lucas_len = k + 1;
}

static inline seq_t
lucas(idx_t f)
{
    ensure_lucas_upto(f);
    return lucas_tbl[f];
}

/* ----------  Heap for (value,g) pairs  ---------- */
typedef struct {
    seq_t val;
    idx_t g;
} HeapNode;

static HeapNode *heap = NULL;
static size_t heap_cap = 0;
static size_t heap_size = 0;

static inline void
heap_push(HeapNode node)
{
    if (heap_size == heap_cap) {
        size_t new_cap = heap_cap ? heap_cap << 1 : 4;
        HeapNode *tmp = (HeapNode*)realloc(heap, new_cap * sizeof(HeapNode));
        if (!tmp) abort();
        heap = tmp;
        heap_cap = new_cap;
    }
    size_t i = heap_size++;
    heap[i] = node;
    while (i > 0) {
        size_t p = (i - 1) >> 1;
        if (heap[p].val <= heap[i].val) break;
        HeapNode t = heap[p];
        heap[p] = heap[i];
        heap[i] = t;
        i = p;
    }
}

static inline HeapNode
heap_pop(void)
{
    HeapNode top = heap[0];
    heap[0] = heap[--heap_size];
    size_t i = 0;
    while (1) {
        size_t l = (i << 1) + 1;
        size_t r = l + 1;
        size_t m = i;
        if (l < heap_size && heap[l].val < heap[m].val) m = l;
        if (r < heap_size && heap[r].val < heap[m].val) m = r;
        if (m == i) break;
        HeapNode t = heap[i];
        heap[i] = heap[m];
        heap[m] = t;
        i = m;
    }
    return top;
}

/* ----------  Sequence cache  ---------- */
static seq_t *seq_cache = NULL;
static idx_t cache_cap = 0;
static idx_t cache_len = 0;
static seq_t last_added = 0;
static bool has_last = false;

/* ----------  Current f for each g  ---------- */
static idx_t *curr_f = NULL;
static idx_t curr_f_cap = 0;
static idx_t max_g_initial = 0;

static inline void
ensure_curr_f_upto(idx_t g)
{
    if (g < curr_f_cap) return;
    size_t new_cap = curr_f_cap ? curr_f_cap : 4;
    while ((idx_t)new_cap <= g) new_cap <<= 1;
    idx_t *tmp = (idx_t*)realloc(curr_f, new_cap * sizeof(idx_t));
    if (!tmp) abort();
    curr_f = tmp;
    curr_f_cap = new_cap;
}

/* ----------  Initialisation  ---------- */
static void
initialize_sequence(idx_t n)
{
    if (!seq_cache) {
        cache_cap = n + 10;
        if (cache_cap < 4) cache_cap = 4;
        seq_cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        if (!seq_cache) abort();
        seq_cache[0] = 0;                /* dummy 0‑th element */
        seq_cache[1] = 0;                /* a(1) = 0            */
        cache_len = 1;
        has_last = true;
        last_added = 0;
    } else if (cache_cap < n + 10) {
        size_t new_cap = cache_cap << 1;
        if (new_cap < n + 10) new_cap = n + 10;
        seq_t *tmp = (seq_t*)realloc(seq_cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        seq_cache = tmp;
        cache_cap = new_cap;
    }

    if (!curr_f) {
        curr_f_cap = n + 10;
        if (curr_f_cap < 4) curr_f_cap = 4;
        curr_f = (idx_t*)calloc(curr_f_cap, sizeof(idx_t));
        if (!curr_f) abort();
    } else if (curr_f_cap < n + 10) {
        size_t new_cap = curr_f_cap << 1;
        if (new_cap < n + 10) new_cap = n + 10;
        idx_t *tmp = (idx_t*)realloc(curr_f, new_cap * sizeof(idx_t));
        if (!tmp) abort();
        curr_f = tmp;
        curr_f_cap = new_cap;
    }

    if (!heap) {
        heap_cap = n + 10;
        if (heap_cap < 4) heap_cap = 4;
        heap = (HeapNode*)malloc(heap_cap * sizeof(HeapNode));
        if (!heap) abort();
        heap_size = 0;
    } else if (heap_cap < n + 10) {
        size_t new_cap = heap_cap << 1;
        if (new_cap < n + 10) new_cap = n + 10;
        HeapNode *tmp = (HeapNode*)realloc(heap, new_cap * sizeof(HeapNode));
        if (!tmp) abort();
        heap = tmp;
        heap_cap = new_cap;
    }

    /* Insert initial g values */
    idx_t target_g = n + 10;
    while (max_g_initial < target_g) {
        ++max_g_initial;
        ensure_curr_f_upto(max_g_initial);
        idx_t g = max_g_initial;
        ensure_lucas_upto(g);
        idx_t f = 1;
        seq_t Lg = lucas(g);
        seq_t left, right;
        while (1) {
            seq_t Lf = lucas(f);
            left  = ((seq_t)2) * Lf;
            right = ((seq_t)5) * Lg;
            if (left >= right) break;
            ++f;
        }
        curr_f[g] = f;
        heap_push((HeapNode){ .val = left - right, .g = g });
    }
}

/* ----------  Generate until n terms  ---------- */
static void
generate_until(idx_t n)
{
    while (cache_len < n) {
        if (heap_size == 0) abort();
        HeapNode node = heap_pop();

        if (!has_last || node.val != last_added) {
            has_last = true;
            last_added = node.val;
            if (cache_len + 1 >= cache_cap) {
                size_t new_cap = cache_cap << 1;
                seq_t *tmp = (seq_t*)realloc(seq_cache, new_cap * sizeof(seq_t));
                if (!tmp) abort();
                seq_cache = tmp;
                cache_cap = new_cap;
            }
            seq_cache[++cache_len] = node.val;
        }

        /* Advance f for this g and push next value */
        idx_t g = node.g;
        ++curr_f[g];
        seq_t Lf = lucas(curr_f[g]);
        seq_t left  = ((seq_t)2) * Lf;
        seq_t right = ((seq_t)5) * lucas(g);
        heap_push((HeapNode){ .val = left - right, .g = g });
    }
}

/* ----------  Public API  ---------- */
seq_t a(idx_t n)
{
    if (n < 1) return 0;        /* behaviour below offset is arbitrary */
    if (cache_len < n) {
        initialize_sequence(n);
        generate_until(n);
    }
    return seq_cache[n];
}
