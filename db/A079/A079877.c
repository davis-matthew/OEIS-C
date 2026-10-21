/*
 * A079877  Numbers that are divisible by at least one pair of twin primes
 *          (A001097).
 * https://oeis.org/A079877
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 1000, u64 n <= 1000, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: union of multiples of twin‑prime products. */
/* a(1) = 15, a(2) = 30, a(3) = 35, … */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

typedef struct { seq_t val; size_t idx; } Node;

static bool        initialized = false;
static seq_t      *base_vals   = NULL;    /* product of each twin‑prime pair   */
static size_t       base_cnt    = 0;        /* number of such products           */
static Node       *heap        = NULL;    /* min‑heap of next multiples        */
static size_t       heap_sz     = 0;        /* current heap size                 */
static seq_t      *seq_arr     = NULL;    /* sequence values (unique)          */
static size_t       seq_len    = 0;        /* number of terms computed so far   */
static size_t       seq_cap    = 0;        /* capacity of seq_arr               */

/* ---------- heap helpers ---------- */
static void heap_push(seq_t val, size_t idx)
{
    Node node = { val, idx };
    size_t i = heap_sz++;
    while (i > 0) {
        size_t parent = (i - 1) >> 1;
        if (heap[parent].val <= node.val) break;
        heap[i] = heap[parent];
        i = parent;
    }
    heap[i] = node;
}

static Node heap_pop(void)
{
    Node res      = heap[0];
    heap[0]       = heap[--heap_sz];
    size_t i = 0;
    while (1) {
        size_t left  = (i << 1) + 1;
        if (left >= heap_sz) break;
        size_t right = left + 1;
        size_t child = (right < heap_sz && heap[right].val < heap[left].val) ? right : left;
        if (heap[i].val <= heap[child].val) break;
        Node tmp     = heap[i];
        heap[i]      = heap[child];
        heap[child]  = tmp;
        i = child;
    }
    return res;
}

/* ---------- sequence storage ---------- */
static void seq_push(seq_t val)
{
    if (seq_len == seq_cap) {
        seq_cap = seq_cap ? seq_cap << 1 : 64;
        seq_arr = (seq_t *)realloc(seq_arr, seq_cap * sizeof(seq_t));
    }
    seq_arr[seq_len++] = val;
}

/* ---------- initialization ---------- */
static void init(void)
{
    if (initialized) return;

    /* Prime sieve up to a fixed bound. */
    const int PRIME_LIMIT = 2000000;          /* 2 × 10⁶  → ~1.2 × 10⁴ twin primes */
    char *is_prime = (char *)calloc(PRIME_LIMIT + 1, 1);
    for (int i = 2; i <= PRIME_LIMIT; ++i) is_prime[i] = 1;
    for (int i = 2; (long long)i * i <= PRIME_LIMIT; ++i) {
        if (is_prime[i]) {
            for (int j = i * i; j <= PRIME_LIMIT; j += i) is_prime[j] = 0;
        }
    }

    /* Collect twin‑prime products */
    size_t cap = 1024;
    base_vals = (seq_t *)malloc(cap * sizeof(seq_t));
    for (int p = 2; p + 2 <= PRIME_LIMIT; ++p) {
        if (is_prime[p] && is_prime[p + 2]) {
            seq_t prod = (seq_t)p * (seq_t)(p + 2);
            if (base_cnt == cap) {
                cap <<= 1;
                base_vals = (seq_t *)realloc(base_vals, cap * sizeof(seq_t));
            }
            base_vals[base_cnt++] = prod;
        }
    }
    free(is_prime);

    /* Build initial min‑heap of first multiples (which are the products themselves) */
    heap = (Node *)malloc(base_cnt * sizeof(Node));
    heap_sz = 0;
    for (size_t i = 0; i < base_cnt; ++i) heap_push(base_vals[i], i);

    initialized = true;
}

/* ---------- public function ---------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* sequence is 1‑based; 0 is unused */
    init();

    /* Generate terms until we have at least n of them */
    while (seq_len < n) {
        Node node = heap_pop();
        seq_t val = node.val;

        /* Store unique values only */
        if (seq_len == 0 || val != seq_arr[seq_len - 1])
            seq_push(val);

        /* Advance the base to its next multiple and re‑insert into the heap */
        seq_t next_val = val + base_vals[node.idx];
        heap_push(next_val, node.idx);
    }

    return seq_arr[n - 1];
}
