/*
 * A073220  Terms of A072390 (sums of two powers of 13) divided by 2.
 * https://oeis.org/A073220
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 14, u32 n <= 32, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) is the n‑th term of the sorted list of 
 * (13^p + 13^q)/2 for integers 0 ≤ q ≤ p, written with a1 = 1.
 *
 * The implementation uses a min‑heap to generate the terms in order,
 * together with a small visited table to avoid inserting the same
 * pair (p,q) more than once.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

static seq_t *powers   = NULL;      /* all 13^k encountered so far   */
static size_t pow_len  = 0;          /* number of valid entries       */
static size_t pow_cap  = 0;          /* allocated capacity            */

/* expand powers array so that it contains 13^k for all k ≤ k_needed */
static void ensure_pow(idx_t k_needed)
{
    if (pow_len == 0) {                          /* initialisation */
        pow_cap = 4;
        powers = (seq_t*)malloc(pow_cap * sizeof(seq_t));
        if (!powers) abort();
        powers[0] = (seq_t)1;                   /* 13^0 */
        pow_len  = 1;
    }
    while (pow_len <= (size_t)k_needed) {
        if (pow_len == pow_cap) {
            pow_cap = pow_cap ? (pow_cap << 1) : 4;
            seq_t *tmp = (seq_t*)realloc(powers, pow_cap * sizeof(seq_t));
            if (!tmp) abort();
            powers = tmp;
        }
        powers[pow_len] = powers[pow_len-1] * (seq_t)13;
        ++pow_len;
    }
}

/* -----  min-heap of (value, i, j)  ------------------------------------ */
struct Node { seq_t val; idx_t i; idx_t j; };

static struct Node *heap    = NULL;
static size_t heap_size  = 0;
static size_t heap_cap   = 0;

static void heap_swap(size_t a, size_t b)
{
    struct Node tmp = heap[a];
    heap[a] = heap[b];
    heap[b] = tmp;
}

static void heap_push(struct Node node)
{
    if (heap_size == heap_cap) {
        heap_cap = heap_cap ? (heap_cap << 1) : 4;
        struct Node *tmp = (struct Node*)realloc(heap, heap_cap * sizeof(struct Node));
        if (!tmp) abort();
        heap = tmp;
    }
    heap[heap_size] = node;
    size_t pos = heap_size++;
    while (pos > 0) {
        size_t parent = (pos - 1) >> 1;
        if (heap[parent].val <= heap[pos].val) break;
        heap_swap(parent, pos);
        pos = parent;
    }
}

static struct Node heap_pop(void)
{
    struct Node top = heap[0];
    --heap_size;
    if (heap_size > 0) {
        heap[0] = heap[heap_size];
        size_t pos = 0;
        while (1) {
            size_t left = (pos << 1) + 1;
            if (left >= heap_size) break;
            size_t right = left + 1;
            size_t smallest = left;
            if (right < heap_size && heap[right].val < heap[left].val)
                smallest = right;
            if (heap[pos].val <= heap[smallest].val) break;
            heap_swap(pos, smallest);
            pos = smallest;
        }
    }
    return top;
}

/* -----  visited table  ------------------------------------------------- */
static uint8_t **visited_rows = NULL;   /* visited_rows[i][j] == 1 if pair (i,j) queued */
static size_t visited_len   = 0;        /* number of allocated rows */

static void ensure_row(idx_t i)
{
    if (visited_len <= (size_t)i) {
        size_t new_len = visited_len ? (visited_len << 1) : 4;
        while (new_len <= (size_t)i) new_len <<= 1;
        uint8_t **tmp = (uint8_t**)realloc(visited_rows, new_len * sizeof(uint8_t*));
        if (!tmp) abort();
        visited_rows = tmp;
        for (size_t k = visited_len; k < new_len; ++k) visited_rows[k] = NULL;
        visited_len = new_len;
    }
    if (!visited_rows[i]) {
        visited_rows[i] = (uint8_t*)calloc(i + 1, sizeof(uint8_t));
        if (!visited_rows[i]) abort();
    }
}

/* -----  cache of generated terms  ------------------------------------- */
static seq_t *terms      = NULL;
static size_t terms_len  = 0;
static size_t terms_cap  = 0;

/* -----  generate next permutation of the sequence  -------------------- */
static void generate_next(void)
{
    if (terms_len == 0) {                     /* initialise generator */
        ensure_pow(0);
        ensure_row(0);
        visited_rows[0][0] = 1;
        struct Node first = { (powers[0] + powers[0]) / 2, 0, 0 };
        heap_push(first);
    }

    struct Node cur = heap_pop();             /* smallest candidate */

    if (terms_len == terms_cap) {
        terms_cap = terms_cap ? (terms_cap << 1) : 4;
        seq_t *tmp = (seq_t*)realloc(terms, terms_cap * sizeof(seq_t));
        if (!tmp) abort();
        terms = tmp;
    }
    terms[terms_len++] = cur.val;

    /* push (i+1, j) if not yet queued */
    idx_t ii = cur.i + 1;
    ensure_pow(ii);
    ensure_row(ii);
    if (!visited_rows[ii][cur.j]) {
        visited_rows[ii][cur.j] = 1;
        struct Node next1 = { (powers[ii] + powers[cur.j]) / 2, ii, cur.j };
        heap_push(next1);
    }

    /* push (i, j+1) if valid and not yet queued */
    if (cur.j < cur.i) {
        idx_t jj = cur.j + 1;
        ensure_row(cur.i);
        if (!visited_rows[cur.i][jj]) {
            visited_rows[cur.i][jj] = 1;
            struct Node next2 = { (powers[cur.i] + powers[jj]) / 2, cur.i, jj };
            heap_push(next2);
        }
    }
}

/* ---------------------------------------------------------------------- */
/* Public function required by the problem statement.                    */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;           /* offset is 1; behaviour below is irrelevant */
    while (terms_len < (size_t)n)
        generate_next();
    return terms[n - 1];
}
