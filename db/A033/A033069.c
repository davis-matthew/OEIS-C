/*
 * A033069  Numbers with the property that all pairs of consecutive base-4
 *          digits differ by 1.
 * https://oeis.org/A033069
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 17, u16 n <= 227, u32 n <= 7520, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A033069: numbers in which every pair of consecutive base‑4 digits differ by 1.
   We generate the terms in increasing order using a min‑heap. */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

static seq_t *heap_vals = NULL;    /* heap of numbers */
static uint8_t *heap_last = NULL;  /* corresponding last digit of each heap element */
static size_t heap_size = 0;       /* current number of elements in the heap */
static size_t heap_cap  = 0;       /* capacity of the heap arrays */

static seq_t *terms = NULL;        /* generated sequence values */
static size_t terms_size = 0;      /* number of values stored */
static size_t terms_cap  = 0;      /* capacity of the terms array */

/*--- heap helpers ----------------------------------------------------------*/
static void heap_resize(void)
{
    size_t new_cap = heap_cap ? heap_cap * 2 : 8;
    heap_vals = (seq_t *)realloc(heap_vals, new_cap * sizeof(seq_t));
    heap_last = (uint8_t *)realloc(heap_last, new_cap * sizeof(uint8_t));
    heap_cap = new_cap;
}

static void heap_push(seq_t val, uint8_t last)
{
    if (heap_size == heap_cap) heap_resize();
    heap_vals[heap_size] = val;
    heap_last[heap_size] = last;
    size_t i = heap_size++;
    while (i > 0) {
        size_t parent = (i - 1) >> 1;
        if (heap_vals[parent] <= heap_vals[i]) break;
        /* swap */
        seq_t tmpv = heap_vals[parent];
        heap_vals[parent] = heap_vals[i];
        heap_vals[i] = tmpv;
        uint8_t tmpl = heap_last[parent];
        heap_last[parent] = heap_last[i];
        heap_last[i] = tmpl;
        i = parent;
    }
}

static void heap_pop(seq_t *val, uint8_t *last)
{
    *val = heap_vals[0];
    *last = heap_last[0];
    heap_vals[0] = heap_vals[--heap_size];
    heap_last[0] = heap_last[heap_size];
    size_t i = 0;
    while (1) {
        size_t left = (i << 1) + 1;
        size_t right = left + 1;
        size_t smallest = i;
        if (left < heap_size && heap_vals[left] < heap_vals[smallest]) smallest = left;
        if (right < heap_size && heap_vals[right] < heap_vals[smallest]) smallest = right;
        if (smallest == i) break;
        /* swap */
        seq_t tmpv = heap_vals[i];
        heap_vals[i] = heap_vals[smallest];
        heap_vals[smallest] = tmpv;
        uint8_t tmpl = heap_last[i];
        heap_last[i] = heap_last[smallest];
        heap_last[smallest] = tmpl;
        i = smallest;
    }
}

/*--- term generation --------------------------------------------------------*/
static void ensure_terms_size(idx_t n)
{
    if ((idx_t)terms_size >= n) return;
    /* Grow terms array if needed */
    if (terms_cap < n) {
        terms_cap = (terms_cap ? terms_cap * 2 : 8);
        if (terms_cap < n) terms_cap = n;
        terms = (seq_t *)realloc(terms, terms_cap * sizeof(seq_t));
    }

    /* Add initial single‑digit numbers if the heap is empty */
    if (heap_size == 0) {
        heap_push((seq_t)1, 1);
        heap_push((seq_t)2, 2);
        heap_push((seq_t)3, 3);
    }

    /* Generate terms until we have at least n of them */
    while ((idx_t)terms_size < n) {
        seq_t val;
        uint8_t last;
        heap_pop(&val, &last);
        terms[terms_size++] = val;

        /* generate successors */
        if (last > 0) {
            seq_t new_val = val * ((seq_t)4) + (seq_t)(last - 1);
            heap_push(new_val, (uint8_t)(last - 1));
        }
        if (last < 3) {
            seq_t new_val = val * ((seq_t)4) + (seq_t)(last + 1);
            heap_push(new_val, (uint8_t)(last + 1));
        }
    }
}

/*--- exported function ------------------------------------------------------*/
seq_t a(idx_t n)
{
    /* Non‑zero offset: a(1) is the first term listed. */
    if (n == 0) return 0;  /* behaviour below the offset does not matter */
    ensure_terms_size(n);
    return terms[n - 1];
}
