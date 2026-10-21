/*
 * A209638  Sequence A209636 (or A209637) sorted into ascending order.
 * https://oeis.org/A209638
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 34, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = n‑th smallest integer that is either a power of two or of the form
 * 2^k * p_i, where p_i is the i‑th prime and i itself is a term of the
 * sequence.  The algorithm generates the terms in ascending order using
 * a min‑heap.  No hard‑coded table is used. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

/* ------------------ prime list (dynamic) -------------------- */
static uint32_t *prime_list   = NULL;   /* holds primes in order, 1‑based */
static size_t prime_cnt       = 0;      /* number of primes stored */
static size_t prime_cap       = 0;      /* capacity of prime_list */
static size_t next_prime_candidate = 2;   /* next integer to test for primality */

/* grow the prime array when needed */
static void ensure_prime_capacity(size_t need)
{
    while (prime_cap < need) {
        size_t new_cap = (prime_cap == 0) ? 16 : prime_cap * 2;
        uint32_t *new_list = (uint32_t *)realloc(prime_list, new_cap * sizeof(uint32_t));
        if (!new_list) exit(EXIT_FAILURE);
        prime_list = new_list;
        prime_cap = new_cap;
    }
}

/* extend the prime list until it contains at least `cnt` primes   */
static void ensure_primes_index(size_t cnt)
{
    if (prime_cnt >= cnt) return;
    ensure_prime_capacity(prime_cnt + 1);

    while (prime_cnt < cnt) {
        size_t cand = next_prime_candidate;
        bool is_prime = true;
        for (size_t j = 0; j < prime_cnt; ++j) {
            uint32_t p = prime_list[j];
            if ((uint64_t)p * p > cand) break;
            if (cand % p == 0) {
                is_prime = false;
                break;
            }
        }
        if (is_prime) {
            prime_list[prime_cnt++] = (uint32_t)cand;
        }
        ++next_prime_candidate;
    }
}

/* return the i-th prime (1‑based) as seq_t */
static seq_t get_prime_by_index(size_t i)
{
    ensure_primes_index(i);
    return (seq_t)prime_list[i - 1];
}

/* ------------------ heap of candidates --------------------- */
typedef struct {
    seq_t  val;          /* candidate value */
    size_t series_id;    /* 0 => powers of 2 series, otherwise term value i */
} Node;

static Node *heap      = NULL;
static size_t heap_sz  = 0;
static size_t heap_cap = 0;

static void heap_grow(void)
{
    size_t new_cap = (heap_cap == 0) ? 256 : heap_cap * 2;
    Node *new_heap = (Node *)realloc(heap, new_cap * sizeof(Node));
    if (!new_heap) exit(EXIT_FAILURE);
    heap = new_heap;
    heap_cap = new_cap;
}

static inline void heap_swap(size_t i, size_t j)
{
    Node tmp = heap[i];
    heap[i] = heap[j];
    heap[j] = tmp;
}

static void heap_push(Node n)
{
    if (heap_sz >= heap_cap) heap_grow();
    size_t i = heap_sz++;
    heap[i] = n;
    /* sift up */
    while (i > 0) {
        size_t p = (i - 1) >> 1;
        if (heap[p].val <= heap[i].val) break;
        heap_swap(i, p);
        i = p;
    }
}

static Node heap_pop(void)
{
    Node res = heap[0];
    heap[0] = heap[--heap_sz];
    /* sift down */
    size_t i = 0;
    while (1) {
        size_t l = (i << 1) + 1;
        if (l >= heap_sz) break;
        size_t r = l + 1;
        size_t best = l;
        if (r < heap_sz && heap[r].val < heap[l].val) best = r;
        if (heap[best].val >= heap[i].val) break;
        heap_swap(i, best);
        i = best;
    }
    return res;
}

/* -------------------- sequence cache ---------------------- */
static seq_t *seq_cache = NULL;
static size_t seq_sz   = 0;
static size_t seq_cap  = 0;

static void seq_grow(void)
{
    size_t new_cap = (seq_cap == 0) ? 256 : seq_cap * 2;
    seq_t *new_seq = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
    if (!new_seq) exit(EXIT_FAILURE);
    seq_cache = new_seq;
    seq_cap = new_cap;
}

/* ensure the cache contains at least n+1 terms (0‑based index) */
static void ensure_terms(idx_t n)
{
    while (seq_sz <= (size_t)n) {
        Node cur = heap_pop();      /* smallest candidate */
        seq_cache[seq_sz++] = cur.val;   /* new sequence element */

        /* push next from the same series (multiply by 2) */
        seq_t next_val = cur.val * ((seq_t)2);
        if (next_val > cur.val) {    /* no overflow */
            heap_push((Node){.val = next_val,
                              .series_id = cur.series_id});
        }

        /* start a new series for this term value if it is >1 */
        if (cur.val > (seq_t)1) {
            seq_t prime_val = get_prime_by_index((size_t)cur.val);
            heap_push((Node){.val = prime_val,
                              .series_id = (size_t)cur.val});
        }
    }
}

/* ------------------- public function --------------------- */
seq_t a(idx_t n)
{
    static bool initialized = false;
    if (!initialized) {
        initialized = true;
        /* initial cache: a(0) = 1 */
        seq_grow();
        seq_cache[seq_sz++] = (seq_t)1;

        /* start the powers‑of‑2 series with its first element 2 */
        heap_push((Node){.val = (seq_t)2, .series_id = 0});
    }
    ensure_terms(n);
    return seq_cache[(size_t)n];
}
