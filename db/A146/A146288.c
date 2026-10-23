/*
 * A146288  Number of divisors of the n-th prime signature number
 *          (A025487(n)).
 * https://oeis.org/A146288
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of divisors of the n-th prime signature number
 *            (the n-th smallest integer of the form p1^e1 * p2^e2 * ... with
 *            e1 >= e2 >= ...). The value is d(p1^e1 * p2^e2 * ...) = Π (ei+1)
 * Range:     u8 n <= 16, u16 n <= 72, u32 n <= 1988, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include "../utils/build_primes_upto.h"

#ifndef BUCKET_BITS
#  define BUCKET_BITS 15
#endif
#define BUCKET_SIZE (1U << BUCKET_BITS)
#define MAX_SIG_LEN 32

/* ---------- Global data structures ---------- */
typedef struct {
    uint8_t exps[MAX_SIG_LEN];
    uint8_t len;
    seq_t   val;
} SigEntry;

static size_t          visited_cap      = 500000;
static size_t          visited_cnt      = 0;
static SigEntry*       visited          = NULL;
static size_t*         next_in_bucket   = NULL;
static size_t          bucket_head[BUCKET_SIZE];

static size_t          heap_cap         = 500000;
static size_t          heap_cnt         = 0;
static size_t*         heap             = NULL;

static uint64_t*       prime_array      = NULL;
static size_t          prime_cnt        = 0;

static seq_t*          vals             = NULL;
static size_t          vals_cap         = 0;
static size_t          computed_cnt     = 0;          /* number of values stored */

/* ---------- Utility functions ---------- */
static inline size_t hash_exps(const uint8_t* exps, uint8_t len)
{
    size_t h = len << 20;            /* embed length */
    for (size_t i = 0; i < len; ++i)
        h = h * 13331 + exps[i];
    return h & (BUCKET_SIZE - 1);
}

static void heap_push(size_t idx)
{
    ++heap_cnt;
    heap[heap_cnt] = idx;
    size_t i = heap_cnt;
    while (i > 1) {
        size_t parent = i >> 1;
        if (visited[heap[parent]].val <= visited[heap[i]].val)
            break;
        size_t tmp = heap[parent];
        heap[parent] = heap[i];
        heap[i] = tmp;
        i = parent;
    }
}

static size_t heap_pop(void)
{
    if (heap_cnt == 0)
        return (size_t)-1;
    size_t top = heap[1];
    heap[1] = heap[heap_cnt];
    --heap_cnt;
    size_t i = 1;
    while (1) {
        size_t left = i << 1;
        if (left > heap_cnt) break;
        size_t right = left + 1;
        size_t smallest = left;
        if (right <= heap_cnt && visited[heap[right]].val < visited[heap[left]].val)
            smallest = right;
        if (visited[heap[smallest]].val >= visited[heap[i]].val)
            break;
        size_t tmp = heap[i];
        heap[i] = heap[smallest];
        heap[smallest] = tmp;
        i = smallest;
    }
    return top;
}

/* Add a signature if not already present and push its index into the heap */
static void add_and_push(const uint8_t* exps, uint8_t len, seq_t val)
{
    size_t h = hash_exps(exps, len);
    size_t cur = bucket_head[h];
    while (cur != (size_t)-1) {
        if (visited[cur].len == len && memcmp(visited[cur].exps, exps, len) == 0)
            return; /* already known */
        cur = next_in_bucket[cur];
    }
    /* New signature */
    visited[visited_cnt].len = len;
    if (len > 0)
        memcpy(visited[visited_cnt].exps, exps, len);
    visited[visited_cnt].val = val;

    next_in_bucket[visited_cnt] = bucket_head[h];
    bucket_head[h] = visited_cnt;

    heap_push(visited_cnt);
    ++visited_cnt;
}

/* ---------- Initialization ---------- */
static void init_globals(void)
{
    /* Allocate visited structures */
    visited = (SigEntry*) malloc(sizeof(SigEntry) * visited_cap);
    next_in_bucket = (size_t*) malloc(sizeof(size_t) * visited_cap);
    for (size_t i = 0; i < BUCKET_SIZE; ++i)
        bucket_head[i] = (size_t)-1;

    /* Allocate heap */
    heap = (size_t*) malloc(sizeof(size_t) * heap_cap);
    heap_cnt = 0;

    /* Build primes up to a safe bound */
    build_primes_upto(5000, &prime_array, &prime_cnt);

    /* Allocate values array */
    vals_cap = 200000;   /* sufficient for n up to about 100k */
    vals = (seq_t*) malloc(sizeof(seq_t) * (vals_cap + 1));

    /* Insert the initial signature: empty exponents, value 1 */
    uint8_t empty_exp[1] = {0};
    add_and_push(empty_exp, 0, (seq_t)1);
}

/* ---------- Extension routine ---------- */
static void extend_to(size_t limit)
{
    while (computed_cnt < limit && computed_cnt < vals_cap) {
        size_t idx = heap_pop();
        if (idx == (size_t)-1)
            break; /* should not happen */

        /* Compute divisor count: product of (exp+1) */
        seq_t dcnt = (seq_t)1;
        for (uint8_t i = 0; i < visited[idx].len; ++i)
            dcnt *= (visited[idx].exps[i] + 1);

        vals[computed_cnt + 1] = dcnt;   /* a(1) stored at index 1 */
        ++computed_cnt;

        /* Generate children signatures */
        const SigEntry* cur = &visited[idx];

        /* 1. Increase existing exponents */
        for (uint8_t i = 0; i < cur->len; ++i) {
            if (i == 0 || cur->exps[i] + 1 <= cur->exps[i - 1]) {
                uint8_t new_exps[MAX_SIG_LEN];
                memcpy(new_exps, cur->exps, cur->len);
                new_exps[i] += 1;
                seq_t new_val = cur->val * (seq_t)prime_array[i];
                add_and_push(new_exps, cur->len, new_val);
            }
        }

        /* 2. Append a new exponent '1' if allowed */
        if (cur->len < MAX_SIG_LEN) {
            if (cur->len == 0 || 1 <= cur->exps[cur->len - 1]) {
                uint8_t new_exps[MAX_SIG_LEN];
                if (cur->len > 0)
                    memcpy(new_exps, cur->exps, cur->len);
                new_exps[cur->len] = 1;
                seq_t new_val = cur->val *
                                (seq_t)prime_array[(size_t)cur->len];
                add_and_push(new_exps, cur->len + 1, new_val);
            }
        }
    }
}

/* ---------- Public function ---------- */
seq_t a(idx_t n)
{
    static bool initialized = false;
    if (!initialized) {
        init_globals();
        initialized = true;
    }
    if (n == 0)
        return (seq_t)0;

    if ((size_t)n > computed_cnt)
        extend_to((size_t)n);

    return vals[(size_t)n];
}
