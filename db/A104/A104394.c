/*
 * A104394  Sums of 4 distinct positive pentatope numbers (A000332).
 * https://oeis.org/A104394
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   encode_key, pent_val
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: computes sums of four distinct positive pentatope numbers
   (P(n)=n*(n+1)*(n+2)*(n+3)/24) in increasing order.
   The sequence is generated lazily with a min‑heap of states (i,j,k,l)
   and a hash set to avoid revisiting states.  The function returns the
   n‑th smallest distinct sum (a(1)=56).  All arithmetic uses seq_t. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/encode_key.h"
#include "../utils/pent_val.h"

/* Forward declarations of helper functions */
static seq_t pent_val(idx_t i);
static void ensure_pent(idx_t m);
static void heap_push(idx_t i, idx_t j, idx_t k, idx_t l, seq_t sum);
static void heap_pop(idx_t *i, idx_t *j, idx_t *k, idx_t *l, seq_t *sum);
static bool visit_insert(idx_t i, idx_t j, idx_t k, idx_t l);

/* ----- global state ----- */
static idx_t max_pent_idx = 0;           /* highest index whose P(n) known   */
static seq_t *pent = NULL;              /* pent[i] = P(i), 1‑based          */
static size_t pent_cap = 0;              /* capacity of pent array            */

static size_t results_cap = 0;           /* capacity of results array         */
static size_t results_size = 0;          /* number of values computed         */
static seq_t *results = NULL;            /* array of computed sums            */

static size_t heap_cap = 0;              /* capacity of heap array            */
static size_t heap_size = 0;             /* current number of heap elements   */
static struct HeapNode {
    seq_t sum;
    idx_t i, j, k, l;
} *heap = NULL;

#define HASH_CAP 262144ULL          /* power of two > 2×max generated states */
static uint64_t *hash_keys = NULL;   /* hash table for visited states */

static void ensure_pent(idx_t m)
{
    if (m <= max_pent_idx) return;
    while (max_pent_idx < m) {
        seq_t val = pent_val(max_pent_idx + 1);
        if (pent_cap == 0) {
            pent_cap = 64;
            pent = (seq_t *)malloc(pent_cap * sizeof(seq_t));
        } else if (pent_cap <= max_pent_idx + 1) {
            pent_cap <<= 1;
            pent = (seq_t *)realloc(pent, pent_cap * sizeof(seq_t));
        }
        pent[++max_pent_idx] = val;
    }
}

static bool visit_insert(idx_t i, idx_t j, idx_t k, idx_t l)
{
    uint64_t key = encode_key(i, j, k, l);
    if (hash_keys == NULL) {
        hash_keys = (uint64_t *)calloc(HASH_CAP, sizeof(uint64_t));
    }
    size_t pos = key & (HASH_CAP - 1);
    while (true) {
        if (hash_keys[pos] == 0) {          /* empty slot */
            hash_keys[pos] = key;
            return true;                    /* inserted */
        }
        if (hash_keys[pos] == key)          /* already visited */
            return false;
        pos = (pos + 1) & (HASH_CAP - 1);   /* probe next */
    }
}

static void heap_push(idx_t i, idx_t j, idx_t k, idx_t l, seq_t sum)
{
    if (heap_size == heap_cap) {
        heap_cap = heap_cap ? heap_cap << 1 : 128;
        heap = (struct HeapNode *)realloc(heap, heap_cap * sizeof(struct HeapNode));
    }
    size_t pos = heap_size++;
    while (pos) {
        size_t parent = (pos - 1) >> 1;
        if (heap[parent].sum <= sum) break;
        heap[pos] = heap[parent];
        pos = parent;
    }
    heap[pos].i = i; heap[pos].j = j; heap[pos].k = k; heap[pos].l = l; heap[pos].sum = sum;
}

static void heap_pop(idx_t *i, idx_t *j, idx_t *k, idx_t *l, seq_t *sum)
{
    if (heap_size == 0) { *sum = 0; return; }
    *sum = heap[0].sum;
    *i   = heap[0].i;
    *j   = heap[0].j;
    *k   = heap[0].k;
    *l   = heap[0].l;
    struct HeapNode last = heap[--heap_size];
    size_t pos = 0;
    while (true) {
        size_t left  = pos * 2 + 1;
        size_t right = left + 1;
        if (left >= heap_size) break;
        size_t child = (right < heap_size && heap[right].sum < heap[left].sum) ? right : left;
        if (heap[child].sum >= last.sum) break;
        heap[pos] = heap[child];
        pos = child;
    }
    heap[pos] = last;
}

/* ----- main sequence function ----- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* safety for offset */

    /* initialisation on first call */
    if (results_size == 0) {
        /* allocate initial storage */
        results_cap = 128;
        results = (seq_t *)malloc(results_cap * sizeof(seq_t));

        /* seed pent array up to 4 for the first state */
        ensure_pent(4);

        /* seed heap and visited set with the minimal combination (1,2,3,4) */
        seq_t s = pent[1] + pent[2] + pent[3] + pent[4];
        heap_push(1, 2, 3, 4, s);
        visit_insert(1, 2, 3, 4);
    }

    /* generate results until we have at least n */
    while (results_size < n) {
        idx_t i, j, k, l;
        seq_t cur_sum;
        heap_pop(&i, &j, &k, &l, &cur_sum);
        /* store unique sums only */
        if (results_size == 0 || cur_sum != results[results_size - 1]) {
            if (results_size == results_cap) {
                results_cap <<= 1;
                results = (seq_t *)realloc(results, results_cap * sizeof(seq_t));
            }
            results[results_size++] = cur_sum;
        }

        /* generate successor states */
        /* increment i if allowed */
        if (i + 1 < j) {
            idx_t ni = i + 1;
            ensure_pent(ni);
            seq_t ns = cur_sum - pent[i] + pent[ni];
            if (visit_insert(ni, j, k, l))
                heap_push(ni, j, k, l, ns);
        }
        /* increment j if allowed */
        if (j + 1 < k) {
            idx_t nj = j + 1;
            ensure_pent(nj);
            seq_t ns = cur_sum - pent[j] + pent[nj];
            if (visit_insert(i, nj, k, l))
                heap_push(i, nj, k, l, ns);
        }
        /* increment k if allowed */
        if (k + 1 < l) {
            idx_t nk = k + 1;
            ensure_pent(nk);
            seq_t ns = cur_sum - pent[k] + pent[nk];
            if (visit_insert(i, j, nk, l))
                heap_push(i, j, nk, l, ns);
        }
        /* always increment l (no upper bound needed) */
        idx_t nl = l + 1;
        ensure_pent(nl);
        seq_t ns = cur_sum - pent[l] + pent[nl];
        if (visit_insert(i, j, k, nl))
            heap_push(i, j, k, nl, ns);
    }

    return results[n - 1];
}
