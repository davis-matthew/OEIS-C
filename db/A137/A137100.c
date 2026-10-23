/*
 * A137100  Numbers k such that k and k^2 use only the digits 2, 4, 6 and 7.
 * https://oeis.org/A137100
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, more
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 4, u64 n <= 4, u128 n <= 4,
 *            big any n
 * Helpers:   square_digits_ok
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A137100: numbers k such that k and k^2 consist only of the digits
   2, 4, 6 and 7.  The function computes a(n) by searching the integers
   containing only those digits and testing the square by a simple
   decimal‑arithmetic algorithm that works for any width of seq_t. */

/* Standard headers */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/square_digits_ok.h"

/* --- Helper: check that all decimal digits of x are in {2,4,6,7}. --- */
static bool digits_ok(seq_t x)
{
    if (x == 0) return false;               /* zero is not allowed */
    while (x > 0) {
        seq_t d = x % 10;
        if (d != 2 && d != 4 && d != 6 && d != 7) return false;
        x /= 10;
    }
    return true;
}

/* --- Min‑heap of candidates (ascending order). --- */
static seq_t *heap = NULL;   /* dynamic array of heap elements   */
static size_t heap_size = 0; /* current number of elements      */
static size_t heap_cap  = 0; /* allocated capacity      */

static void heap_grow(size_t min_cap)
{
    if (heap_cap >= min_cap)
        return;
    size_t new_cap = heap_cap ? heap_cap << 1 : 16;
    while (new_cap < min_cap) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(heap, new_cap * sizeof(seq_t));
    heap = tmp;
    heap_cap = new_cap;
}

static void heap_push(seq_t val)
{
    heap_grow(heap_size + 1);
    size_t idx = heap_size++;
    while (idx > 0) {
        size_t parent = (idx - 1) >> 1;
        if (heap[parent] <= val) break;
        heap[idx] = heap[parent];
        idx = parent;
    }
    heap[idx] = val;
}

static seq_t heap_pop_min(void)
{
    seq_t min = heap[0];
    seq_t last = heap[--heap_size];
    size_t idx = 0;
    while (1) {
        size_t left = (idx << 1) + 1;
        if (left >= heap_size) break;
        size_t right = left + 1;
        size_t child = left;
        if (right < heap_size && heap[right] < heap[left])
            child = right;
        if (last <= heap[child]) break;
        heap[idx] = heap[child];
        idx = child;
    }
    heap[idx] = last;
    return min;
}

/* --- Cache of found terms. --- */
static seq_t *cache = NULL;   /* array of found terms           */
static size_t cache_size = 0; /* number of terms stored         */
static size_t cache_cap  = 0; /* allocated capacity             */

static void cache_grow(size_t min_cap)
{
    if (cache_cap >= min_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap << 1 : 16;
    while (new_cap < min_cap) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache = tmp;
    cache_cap = new_cap;
}

/* Build sequence terms until at least n terms are stored. */
static void ensure_terms(idx_t n)
{
    if (n <= cache_size)
        return;

    /* initialise heap on first call */
    if (heap_size == 0) {
        const int init_digits[4] = {2, 4, 6, 7};
        for (int i = 0; i < 4; ++i)
            heap_push((seq_t)init_digits[i]);
    }

    const int next_digits[4] = {2, 4, 6, 7};

    while ((size_t)n > cache_size) {
        seq_t cand = heap_pop_min();

        /* Test candidate. */
        if (digits_ok(cand) && square_digits_ok(cand)) {
            cache_grow(cache_size + 1);
            cache[cache_size++] = cand;
        }

        /* Extend candidate with each allowed digit. */
        for (int i = 0; i < 4; ++i) {
            seq_t nxt = cand * 10 + next_digits[i];
            heap_push(nxt);
        }
    }
}

/* Public function required by the problem statement. */
seq_t a(idx_t n)
{
    /* a(1) is the first term; indices below 1 are undefined. */
    if (n == 0) return 0;      /* guard against accidental zero call */
    ensure_terms(n);
    return cache[n - 1];
}
