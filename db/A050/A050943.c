/*
 * A050943  The sum of 2 or more consecutive nonzero triangular numbers.
 * https://oeis.org/A050943
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the nth distinct sum of two or more consecutive nonzero
 *            triangular numbers
 * Range:     u8 n <= 39, u16 n <= 2977, u32 n <= 7350, u64 n <= 7350,
 *            u128 n <= 7350, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static const unsigned DEFAULT_CAP = 8;

struct seq_node {
    idx_t i;          /* starting triangular index (>=1)          */
    idx_t m;          /* current length of the run (>=2)         */
    seq_t sum;        /* current sum of the run                  */
};

/* Global state ------------------------------------------------------------ */
static struct seq_node *heap       = NULL;
static size_t heap_size            = 0;
static size_t heap_cap             = 0;

static seq_t *terms                = NULL;
static size_t terms_len             = 0;
static size_t terms_cap             = 0;

static idx_t next_start            = 1;  /* first as yet unused starting index */

/* ------------------------------------------------------------------------ */

/* --- triangular numbers -------------------------------------------------- */
static inline seq_t tri(idx_t k)
{
    return ((seq_t)k * (k + 1)) / 2;
}

/* --- heap manipulation --------------------------------------------------- */
static void heap_push(idx_t i, idx_t m, seq_t sum)
{
    if (heap_size == heap_cap) {
        heap_cap = heap_cap ? heap_cap * 2 : DEFAULT_CAP;
        heap = (struct seq_node *)realloc(heap, heap_cap * sizeof *heap);
    }
    size_t pos = heap_size++;
    heap[pos] = (struct seq_node){i, m, sum};

    /* sift up */
    while (pos > 0) {
        size_t parent = (pos - 1) >> 1;
        if (heap[pos].sum < heap[parent].sum) {
            struct seq_node tmp = heap[pos];
            heap[pos] = heap[parent];
            heap[parent] = tmp;
            pos = parent;
        } else break;
    }
}

static struct seq_node heap_pop(void)
{
    struct seq_node res = heap[0];
    heap[0] = heap[--heap_size];

    /* sift down */
    size_t pos = 0;
    while (1) {
        size_t left = (pos << 1) + 1;
        if (left >= heap_size) break;
        size_t right = left + 1;
        size_t smallest = left;
        if (right < heap_size && heap[right].sum < heap[left].sum)
            smallest = right;
        if (heap[smallest].sum < heap[pos].sum) {
            struct seq_node tmp = heap[pos];
            heap[pos] = heap[smallest];
            heap[smallest] = tmp;
            pos = smallest;
        } else break;
    }
    return res;
}

/* ------------------------------------------------------------------------ */

/* --- ensure we have at least (n+1) terms -------------------------------- */
static void ensure_terms_upto(idx_t n)
{
    if (terms_len > n) return;

    while (terms_len <= n) {
        /* make sure the heap is not empty */
        if (heap_size == 0) {
            seq_t sum = tri(next_start) + tri(next_start + 1);
            heap_push(next_start, 2, sum);
            ++next_start;
        }

        /* take smallest sum */
        struct seq_node cur = heap_pop();

        /* record distinct term */
        if (terms_len == 0 || cur.sum != terms[terms_len - 1]) {
            if (terms_len == terms_cap) {
                terms_cap = terms_cap ? terms_cap * 2 : DEFAULT_CAP;
                terms = (seq_t *)realloc(terms, terms_cap * sizeof *terms);
            }
            terms[terms_len++] = cur.sum;
        }

        /* advance this run to the next length */
        cur.m++;
        cur.sum += tri(cur.i + cur.m - 1);
        heap_push(cur.i, cur.m, cur.sum);

        /* add new starting indices whose first sum is not larger than the new heap minimum */
        while (heap_size > 0) {
            seq_t first = tri(next_start) + tri(next_start + 1);
            if (first <= heap[0].sum) {
                heap_push(next_start, 2, first);
                ++next_start;
            } else {
                break;
            }
        }
    }
}

/* ------------------------------------------------------------------------ */

/* --- public function ----------------------------------------------------- */
seq_t a(idx_t n)
{
    ensure_terms_upto(n);
    return terms[n];
}
