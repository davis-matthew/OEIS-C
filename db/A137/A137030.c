/*
 * A137030  Numbers k such that k and k^2 use only the digits 1, 3, 4 and 8.
 * https://oeis.org/A137030
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, more
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 4, u128 n <= 4,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes terms of A137030 by generating all numbers whose decimal
   digits belong to {1,3,4,8} in increasing order, checking whether the
   square also consists only of those digits.  The function a(idx_t n)
   returns the n‑th element (1‑based).  The implementation uses a
   binary min‑heap and memoises found terms, so any call order is fast. */
#if defined(__cplusplus)
#include <vector>
#include <algorithm>
#include <functional>
#else
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#endif

/* --------------------------------------------------------------------- */
/* Helper: digit test for seq_t (works for builtin integers and
   arbitrary‑precision types that support % and /). */
static inline bool digits_allowed(seq_t x)
{
    if (x == (seq_t)0) return false;
    while (x > (seq_t)0) {
        int d = (int)(x % (seq_t)10);
        if (d != 1 && d != 3 && d != 4 && d != 8) return false;
        x /= (seq_t)10;
    }
    return true;
}

/* --------------------------------------------------------------------- */
/* Heap implementation ---------------------------------------------------- */
#ifdef __cplusplus
static std::vector<seq_t> heap;        /* min‑heap with > comparator */

static inline void heap_push(seq_t v)
{
    heap.push_back(v);
    std::push_heap(heap.begin(), heap.end(), std::greater<seq_t>());
}

static inline seq_t heap_pop(void)
{
    std::pop_heap(heap.begin(), heap.end(), std::greater<seq_t>());
    seq_t v = heap.back();
    heap.pop_back();
    return v;
}

static inline void heap_init(void) { /* nothing needed on first use */ }
#else
/* C implementation of a binary min-heap. */
static seq_t *heap_data = NULL;
static size_t heap_size = 0;
static size_t heap_cap  = 0;

static inline void heap_push(seq_t v)
{
    if (heap_size >= heap_cap) {
        size_t new_cap = heap_cap ? heap_cap * 2 : 16;
        seq_t *newp = (seq_t *)realloc(heap_data, new_cap * sizeof(seq_t));
        if (!newp) exit(EXIT_FAILURE);
        heap_data = newp;
        heap_cap = new_cap;
    }
    size_t i = heap_size++;
    heap_data[i] = v;
    /* sift up */
    while (i > 0) {
        size_t p = (i - 1) >> 1;
        if (heap_data[p] <= heap_data[i]) break;
        seq_t tmp = heap_data[p];
        heap_data[p] = heap_data[i];
        heap_data[i] = tmp;
        i = p;
    }
}

static inline seq_t heap_pop(void)
{
    seq_t ret = heap_data[0];
    heap_data[0] = heap_data[--heap_size];
    /* sift down */
    size_t i = 0;
    while (1) {
        size_t l = (i << 1) + 1;
        if (l >= heap_size) break;
        size_t r = l + 1;
        size_t c = (r < heap_size && heap_data[r] < heap_data[l]) ? r : l;
        if (heap_data[i] <= heap_data[c]) break;
        seq_t tmp = heap_data[i];
        heap_data[i] = heap_data[c];
        heap_data[c] = tmp;
        i = c;
    }
    return ret;
}

static inline void heap_init(void)
{
    if (heap_cap == 0) {          /* first initialisation */
        for (size_t d = 0; d < 4; ++d)
            heap_push(((seq_t)1 << d)); /* uses digits 1,2,4,8 but will be replaced below */
    }
}
#endif

/* --------------------------------------------------------------------- */
/* Memoised list of found terms ---------------------------------------- */
#ifdef __cplusplus
static std::vector<seq_t> seq_vals;
#else
static seq_t *seq_vals = NULL;
static size_t seq_len = 0;
static size_t seq_cap = 0;
#endif

static const seq_t digits_allowed_list[4] = { (seq_t)1, (seq_t)3, (seq_t)4, (seq_t)8 };

/* --------------------------------------------------------------------- */
static inline void add_value(seq_t v)
{
#ifdef __cplusplus
    seq_vals.push_back(v);
#else
    if (seq_len >= seq_cap) {
        size_t new_cap = seq_cap ? seq_cap * 2 : 16;
        seq_t *newp = (seq_t *)realloc(seq_vals, new_cap * sizeof(seq_t));
        if (!newp) exit(EXIT_FAILURE);
        seq_vals = newp;
        seq_cap = new_cap;
    }
    seq_vals[seq_len++] = v;
#endif
}

/* --------------------------------------------------------------------- */
/* Public sequence function ---------------------------------------------- */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;      /* undefined, but avoid UB */

    /* Initialise heap once. */
    static bool heap_inited = false;
    if (!heap_inited) {
        heap_inited = true;
        heap_init();
#ifdef __cplusplus
        heap.clear();
        for (size_t d = 0; d < 4; ++d) heap_push(digits_allowed_list[d]);
#else
        heap_size = heap_cap = 0;
        for (size_t d = 0; d < 4; ++d) heap_push(digits_allowed_list[d]);
#endif
    }

    /* Generate terms until we have at least n of them. */
    while (true) {
#ifdef __cplusplus
        if (seq_vals.size() >= (size_t)n) break;
#else
        if (seq_len >= (size_t)n) break;
#endif
        /* take the smallest candidate */
        seq_t k = heap_pop();

        /* test whether k^2 uses only allowed digits */
        seq_t sq = k * k;
        if (digits_allowed(sq)) add_value(k);

        /* push all extensions of k by one allowed digit */
        for (size_t i = 0; i < 4; ++i) {
            seq_t child = k * (seq_t)10 + digits_allowed_list[i];
            heap_push(child);
        }
    }

    /* return the required term (1‑based). */
#ifdef __cplusplus
    return seq_vals[n - 1];
#else
    return seq_vals[n - 1];
#endif
}
