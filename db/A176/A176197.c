/*
 * A176197  Sum of 4 distinct nonzero fourth powers.
 * https://oeis.org/A176197
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the n‑th smallest sum a^4+b^4+c^4+d^4 with 1 ≤ a < b < c
 *            < d. The function returns this value for any n ≥ 1, using a
 *            best‑first search with a priority queue
 * Range:     u8 none, u16 n <= 43, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>

/* ------------------------------------------------------------------ */
/* Small utilities --------------------------------------------------- */
static inline seq_t fourth_power(idx_t e)
{
    seq_t r = (seq_t)e * e;   /* e^2  */
    r     = r * r;            /* e^4  */
    return r;
}

/* ------------------------------------------------------------------ */
/* Visited hash set (open addressing, linear probing) ---------------- */
typedef struct {
    uint64_t *table;   /* 0 = empty */
    size_t   size;
    size_t   capacity;
    size_t   mask;
} visited_t;

static void visited_init(visited_t *v)
{
    v->capacity = 16;
    v->mask     = v->capacity - 1;
    v->size     = 0;
    v->table = (uint64_t*)calloc(v->capacity, sizeof(uint64_t));
}

static void visited_resize(visited_t *v)
{
    size_t newcap = v->capacity << 1;
    uint64_t *newtab = (uint64_t*)calloc(newcap, sizeof(uint64_t));
    size_t newmask = newcap - 1;
    for (size_t i = 0; i < v->capacity; ++i) {
        uint64_t key = v->table[i];
        if (!key) continue;
        size_t idx = (key ^ (key >> 33)) & newmask;
        while (newtab[idx]) idx = (idx + 1) & newmask;
        newtab[idx] = key;
    }
    free(v->table);
    v->table = newtab;
    v->capacity = newcap;
    v->mask = newmask;
}

static bool visited_insert(visited_t *v, uint64_t key)
{
    if (key == 0) return false;          /* zero not used */
    if ((v->size + 1) * 2 > v->capacity) visited_resize(v);
    size_t idx = (key ^ (key >> 33)) & v->mask;
    while (v->table[idx]) {
        if (v->table[idx] == key) return false; /* already present */
        idx = (idx + 1) & v->mask;
    }
    v->table[idx] = key;
    ++v->size;
    return true;
}

/* ------------------------------------------------------------------ */
/* Min-heap for nodes ----------------------------------------------- */
typedef struct {
    seq_t  sum;
    uint32_t a, b, c, d;     /* 1‑based indices, a<b<c<d */
} node_t;

typedef struct {
    node_t *data;
    size_t  sz;
    size_t  cap;
} heap_t;

static void heap_init(heap_t *h)
{
    h->cap = 16;
    h->sz  = 0;
    h->data = (node_t*)calloc(h->cap, sizeof(node_t));
}

static void heap_push(heap_t *h, node_t n)
{
    if (h->sz == h->cap) {
        h->cap <<= 1;
        h->data = (node_t*)realloc(h->data, h->cap * sizeof(node_t));
    }
    size_t i = h->sz++;
    while (i) {
        size_t p = (i - 1) >> 1;
        if (h->data[p].sum <= n.sum) break;
        h->data[i] = h->data[p];
        i = p;
    }
    h->data[i] = n;
}

static node_t heap_pop(heap_t *h)
{
    node_t top = h->data[0];
    node_t last = h->data[--h->sz];
    size_t i = 0, child;
    while ((child = (i << 1) + 1) < h->sz) {
        if (child + 1 < h->sz && h->data[child + 1].sum < h->data[child].sum)
            ++child;
        if (h->data[child].sum >= last.sum) break;
        h->data[i] = h->data[child];
        i = child;
    }
    h->data[i] = last;
    return top;
}

/* ------------------------------------------------------------------ */
/* Power table ------------------------------------------------------- */
typedef struct {
    seq_t *data;   /* 1‑based: data[0] unused */
    size_t len;    /* number of allocated entries (including index 0) */
} p4_tab_t;

static void p4_init(p4_tab_t *t)
{
    t->len = 1;                   /* allocate index 0 unused */
    t->data = (seq_t*)calloc(2, sizeof(seq_t));
}

static void p4_ensure(p4_tab_t *t, idx_t idx)
{
    if (t->len > idx) return;
    size_t newlen = (size_t)idx + 1;   /* allocate up to idx (inclusive) */
    t->data = (seq_t*)realloc(t->data, newlen * sizeof(seq_t));
    for (idx_t i = (idx_t)t->len; i <= idx; ++i)
        t->data[i] = fourth_power(i);
    t->len = newlen;
}

/* ------------------------------------------------------------------ */
/* Cache of computed terms ------------------------------------------ */
static void cache_ensure(seq_t **cache, size_t *cap, size_t need)
{
    if (*cap >= need) return;
    size_t newcap = *cap ? *cap : 16;
    while (newcap < need) newcap <<= 1;
    *cache = (seq_t*)realloc(*cache, newcap * sizeof(seq_t));
    *cap = newcap;
}

/* ------------------------------------------------------------------ */
/* Main function ------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* undefined, but return 0 */

    /* Static state */
    static bool initialized = false;
    static seq_t *cache = NULL;
    static size_t cache_len = 0, cache_cap = 0;
    static heap_t heap;
    static visited_t vis;
    static p4_tab_t p4tab;

    if (!initialized) {
        heap_init(&heap);
        visited_init(&vis);
        p4_init(&p4tab);
        initialized = true;
    }

    /* Ensure we have enough cache space */
    if (n > cache_len) {
        cache_ensure(&cache, &cache_cap, n);
        /* Generate terms until we have n terms */
        while (cache_len < n) {
            node_t cur;
            if (cache_len == 0) {
                /* First term: (1,2,3,4) */
                cur.a = 1; cur.b = 2; cur.c = 3; cur.d = 4;
                p4_ensure(&p4tab, cur.d);
                cur.sum = p4tab.data[cur.a] + p4tab.data[cur.b]
                        + p4tab.data[cur.c] + p4tab.data[cur.d];
                heap_push(&heap, cur);
                visited_insert(&vis,
                              ((uint64_t)cur.a << 48) |
                              ((uint64_t)cur.b << 32) |
                              ((uint64_t)cur.c << 16) |
                              (uint64_t)cur.d);
            }
            /* Pop smallest sum */
            cur = heap_pop(&heap);
            cache[cache_len++] = cur.sum;

            /* Generate neighbors by incrementing one index */
            uint32_t indices[4] = {cur.a, cur.b, cur.c, cur.d};
            for (int pos = 0; pos < 4; ++pos) {
                uint32_t nc[4] = {indices[0], indices[1], indices[2], indices[3]};
                /* Increment chosen position */
                ++nc[pos];
                /* Reset following indices to minimal legal values */
                for (int j = pos + 1; j < 4; ++j)
                    nc[j] = nc[j - 1] + 1;
                /* Encode key */
                uint64_t key = ((uint64_t)nc[0] << 48) |
                               ((uint64_t)nc[1] << 32) |
                               ((uint64_t)nc[2] << 16) |
                               (uint64_t)nc[3];
                if (!visited_insert(&vis, key))
                    continue;          /* already seen */
                p4_ensure(&p4tab, nc[3]);  /* ensure powers up to largest index */
                node_t nxt;
                nxt.a = nc[0]; nxt.b = nc[1]; nxt.c = nc[2]; nxt.d = nc[3];
                nxt.sum = p4tab.data[nxt.a] + p4tab.data[nxt.b]
                        + p4tab.data[nxt.c] + p4tab.data[nxt.d];
                heap_push(&heap, nxt);
            }
        }
    }

    return cache[n - 1];
}
