/*
 * A030515  Numbers with exactly 6 divisors.
 * https://oeis.org/A030515
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   The sequence consists of all integers of the form p⁵ or p·q² *
 *            where p and q are distinct primes. It is the union of A050997
 *            and * A054753
 * Range:     u8 none, u16 none, u32 n <= 1000, u64 n <= 1000, u128 n <= 1000,
 *            big any n
 * Helpers:   build_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <limits.h>

/* The utilities provided by the build environment. */
#include "../utils/build_primes.h"

/* Helper: compute p⁵ with seq_t arithmetic. */
static inline seq_t pow5(uint64_t p)
{
    seq_t r = (seq_t)p;
    r *= (seq_t)p;
    r *= (seq_t)p;
    r *= (seq_t)p;
    r *= (seq_t)p;
    return r;
}

/* Candidate for the p·q² family. */
typedef struct {
    seq_t value;     /* current value q² * p */
    idx_t p_idx;     /* index of p in primes[] */
    idx_t q_idx;     /* index of q in primes[] */
} QCand;

/* Simple binary min‑heap for QCand. */
static QCand *heap      = NULL;
static size_t  heap_size = 0;
static size_t  heap_cap  = 0;

static inline void sift_up(size_t i)
{
    while (i > 0) {
        size_t parent = (i - 1) >> 1;
        if (heap[parent].value <= heap[i].value) break;
        QCand tmp = heap[parent];
        heap[parent] = heap[i];
        heap[i] = tmp;
        i = parent;
    }
}

static inline void sift_down(size_t i)
{
    while (1) {
        size_t left = (i << 1) + 1;
        size_t right = left + 1;
        size_t smallest = i;
        if (left < heap_size && heap[left].value < heap[smallest].value)
            smallest = left;
        if (right < heap_size && heap[right].value < heap[smallest].value)
            smallest = right;
        if (smallest == i) break;
        QCand tmp = heap[i];
        heap[i] = heap[smallest];
        heap[smallest] = tmp;
        i = smallest;
    }
}

static inline void heap_init(size_t capacity)
{
    heap_cap = capacity;
    heap_size = 0;
    heap = (QCand *)malloc(sizeof(QCand) * heap_cap);
}

static inline void heap_push(QCand cand)
{
    if (heap_size == heap_cap) {
        heap_cap *= 2;
        heap = (QCand *)realloc(heap, sizeof(QCand) * heap_cap);
    }
    heap[heap_size++] = cand;
    sift_up(heap_size - 1);
}

static inline QCand heap_pop(void)
{
    QCand root = heap[0];
    heap[0] = heap[--heap_size];
    sift_down(0);
    return root;
}

/* Prime table and related state. */
#define PRIME_LIMIT 2000000

static uint64_t *primes      = NULL;
static size_t   prime_cnt    = 0;

/* Heap state for the p·q² family. */
static size_t   q_heap_cap   = 0;

/* Sequence cache. */
static seq_t  *seq_cache    = NULL;
static size_t  seq_cap      = 0;   /* allocated capacity */
static size_t  seq_len      = 0;   /* current length */

/* Initialization of primes */
static void init_primes(void)
{
    if (primes != NULL) return;          /* already initialized */
    idx_t *p_arr = NULL;
    size_t cnt = 0;
    build_primes(PRIME_LIMIT, &p_arr, &cnt);
    prime_cnt = cnt;
    primes = (uint64_t *)p_arr;
}

/* Ensure the sequence cache is at least size n. */
static void grow_seq_cache(size_t needed)
{
    if (seq_cap >= needed) return;
    while (seq_cap < needed) {
        seq_cap = seq_cap == 0 ? 128 : seq_cap << 1;
    }
    seq_cache = (seq_t *)realloc(seq_cache, sizeof(seq_t) * seq_cap);
}

/* Helper: push the next candidate for a given q_idx. */
static void push_next_candidate(idx_t q_idx, idx_t *p_idx)
{
    /* Move p_idx to next prime different from q. */
    while (*p_idx < prime_cnt && primes[*p_idx] == primes[q_idx]) (*p_idx)++;
    if (*p_idx >= prime_cnt) return; /* no more candidates for this q */

    seq_t sq = (seq_t)primes[q_idx] * (seq_t)primes[q_idx];
    seq_t val = (seq_t)primes[*p_idx] * sq;
    QCand cand = { val, *p_idx, q_idx };
    heap_push(cand);
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return 0; /* offset starts at 1; ignore n==0 */

    init_primes();

    /* Ensure sequence cache capacity. */
    grow_seq_cache((size_t)n);

    /* First time: build the heap covering all q primes. */
    if (heap == NULL) {
        heap_init(prime_cnt);
        /* For each q, find the first valid p. */
        for (idx_t q = 0; q < prime_cnt; ++q) {
            idx_t p = 0;
            push_next_candidate(q, &p);
        }
    }

    /* Generate terms until we reach nth term. */
    while (seq_len < (size_t)n) {
        seq_t next_q = (heap_size > 0) ? heap[0].value : (seq_t)(-1);
        /* Compute next p^5 value. */
        static idx_t p5_idx = 0;
        static seq_t next_p5_val = 0;
        if (p5_idx == 0) {
            if (p5_idx < prime_cnt)
                next_p5_val = pow5(primes[p5_idx]);
            else
                next_p5_val = (seq_t)(-1);
        }

        seq_t val;
        if (next_p5_val <= next_q) {
            /* Use p^5. */
            val = next_p5_val;
            /* Advance to next prime and recompute. */
            do {
                ++p5_idx;
            } while (p5_idx < prime_cnt && primes[p5_idx] == 0);
            if (p5_idx < prime_cnt)
                next_p5_val = pow5(primes[p5_idx]);
            else
                next_p5_val = (seq_t)(-1);
        } else {
            /* Use smallest p·q² candidate. */
            QCand d = heap_pop();
            val = d.value;
            /* Advance p for this q and push new candidate. */
            ++d.p_idx;
            push_next_candidate(d.q_idx, &d.p_idx);
        }

        seq_cache[seq_len++] = val;
    }

    return seq_cache[(size_t)n - 1];
}
