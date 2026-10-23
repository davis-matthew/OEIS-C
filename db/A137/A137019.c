/*
 * A137019  Numbers k such that k and k^2 use only the digits 1, 2, 7 and 9.
 * https://oeis.org/A137019
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, more
 * Formula:   the function computes a(n) by brute‑force search over * numbers
 *            that use only the digits 1,2,7,9 and for which k² also * uses
 *            only these digits. Each such k is stored in increasing order *
 *            so that a(n) is the n‑th element of that sequence (1‑based)
 * Range:     u8 n <= 3, u16 n <= 3, u32 n <= 4, u64 n <= 4, u128 n <= 4,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* ---------- static data structures for memoisation ---------- */
static seq_t *found = NULL;          /* found terms */
static size_t found_size = 0, found_cap = 0;

static seq_t *heap = NULL;           /* min‑heap of candidates */
static size_t heap_size = 0, heap_cap = 0;

/* base digits that may appear in a valid k */
static const seq_t base_digits[4] = { (seq_t)1, (seq_t)2, (seq_t)7, (seq_t)9 };

/* ---------- helper functions --------------------------------- */

/* push a new element onto the min‑heap (binary heap) */
static void heap_push(seq_t val)
{
    if (heap_size == heap_cap) {
        size_t newcap = heap_cap ? heap_cap * 2 : 16;
        seq_t *tmp = (seq_t *)realloc(heap, newcap * sizeof(seq_t));
        if (!tmp) abort();
        heap = tmp;
        heap_cap = newcap;
    }
    size_t i = heap_size++;
    heap[i] = val;
    /* sift up */
    while (i > 0) {
        size_t parent = (i - 1) >> 1;
        if (heap[parent] <= heap[i]) break;
        seq_t tmp = heap[parent];
        heap[parent] = heap[i];
        heap[i] = tmp;
        i = parent;
    }
}

/* pop the smallest element from the heap; returns false if heap empty */
static bool heap_pop(seq_t *out)
{
    if (heap_size == 0) return false;
    *out = heap[0];
    heap[0] = heap[--heap_size];
    /* sift down */
    size_t i = 0;
    while (true) {
        size_t left = (i << 1) + 1;
        if (left >= heap_size) break;
        size_t right = left + 1;
        size_t smallest = left;
        if (right < heap_size && heap[right] < heap[left]) smallest = right;
        if (heap[i] <= heap[smallest]) break;
        seq_t tmp = heap[i];
        heap[i] = heap[smallest];
        heap[smallest] = tmp;
        i = smallest;
    }
    return true;
}

/* test whether all decimal digits of x belong to {1,2,7,9} */
static bool allowed_digits(seq_t x)
{
    if (x == (seq_t)0) return false;      /* zero has digit 0, not allowed */
    while (x) {
        seq_t d = x % (seq_t)10;
        if (d != (seq_t)1 && d != (seq_t)2 && d != (seq_t)7 && d != (seq_t)9)
            return false;
        x /= (seq_t)10;
    }
    return true;
}

/* ensure the found array has room for at least one more element */
static void grow_found(void)
{
    if (found_size == found_cap) {
        size_t newcap = found_cap ? found_cap * 2 : 16;
        seq_t *tmp = (seq_t *)realloc(found, newcap * sizeof(seq_t));
        if (!tmp) abort();
        found = tmp;
        found_cap = newcap;
    }
}

/* ---------- sequence function -------------------------------- */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;     /* undefined for n < 1 */

    /* initialise static structures on first call */
    if (!found) {
        found_cap = 16;
        found = (seq_t *)malloc(found_cap * sizeof(seq_t));
        if (!found) abort();
    }
    if (!heap) {
        heap_cap = 16;
        heap = (seq_t *)malloc(heap_cap * sizeof(seq_t));
        if (!heap) abort();
    }
    if (heap_size == 0) {
        /* seed heap with the four base digits */
        for (int i = 0; i < 4; ++i) heap_push(base_digits[i]);
    }

    /* generate terms until we reach the requested index */
    while (found_size < (size_t)n) {
        seq_t k;
        if (!heap_pop(&k)) break;              /* no more candidates */

        /* test if k² has only allowed digits */
        seq_t k2 = k * k;
        if (allowed_digits(k2)) {
            grow_found();
            found[found_size++] = k;
        }

        /* generate the next numbers by appending each allowed digit */
        for (int i = 0; i < 4; ++i) {
            seq_t tent = k * (seq_t)10;
            /* overflow check: for 64‑bit seq_t, overflow wraps, so compare */
            if (tent < k) continue;
            seq_t next = tent + base_digits[i];
            if (next < tent) continue;
            heap_push(next);
        }
    }

    /* the array is 0‑based; n is 1‑based */
    return found[(size_t)n - 1];
}
