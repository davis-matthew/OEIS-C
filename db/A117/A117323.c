/*
 * A117323  Semiprime(n) modulo prime(n).
 * https://oeis.org/A117323
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = A001358(n) modulo A000040(n)
 * Range:     u8 none, u16 none, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/build_primes_upto.h"   /* provides build_primes_upto() */

struct Node {
    seq_t val;
    idx_t i;   /* index of the first prime */
    idx_t j;   /* index of the second prime (>= i) */
};

static bool initialized = false;
static uint64_t *primes = NULL;           /* array of primes (uint64_t)          */
static size_t prime_cnt = 0;              /* number of primes                    */
static struct Node *heap = NULL;          /* min‑heap of Node                    */
static size_t heap_sz = 0;                /* current heap size                   */

static seq_t *semis = NULL;               /* array of semiprimes                */
static size_t semis_cap = 0, semis_len = 0;

/* ---------- heap helpers ---------- */
static void heap_swap(struct Node *a, struct Node *b) {
    struct Node tmp = *a; *a = *b; *b = tmp;
}

static void heap_bubble_up(size_t idx) {
    while (idx > 0) {
        size_t p = (idx - 1) >> 1;
        if (heap[p].val <= heap[idx].val) break;
        heap_swap(&heap[p], &heap[idx]);
        idx = p;
    }
}

static void heap_push(struct Node n) {
    if (heap_sz == prime_cnt) return; /* should not happen */
    heap[heap_sz++] = n;
    heap_bubble_up(heap_sz - 1);
}

static void heap_bubble_down(size_t idx) {
    while (1) {
        size_t l = (idx << 1) + 1;
        if (l >= heap_sz) break;
        size_t r = l + 1;
        size_t child = (r < heap_sz && heap[r].val < heap[l].val) ? r : l;
        if (heap[idx].val <= heap[child].val) break;
        heap_swap(&heap[idx], &heap[child]);
        idx = child;
    }
}

static struct Node heap_pop(void) {
    struct Node res = heap[0];
    heap[0] = heap[--heap_sz];
    heap_bubble_down(0);
    return res;
}

/* ---------- semiprime generation ---------- */
static void init(void) {
    /* choose a limit large enough to get >10000 primes (prime(10000) ≈ 104729) */
    const idx_t PRIME_LIMIT = 200000;        /* 2e5 is safely above prime_10000 */
    build_primes_upto(PRIME_LIMIT, &primes, &prime_cnt);

    /* allocate heap */
    heap = (struct Node *)malloc(sizeof(struct Node) * prime_cnt);
    heap_sz = 0;

    /* initial nodes: p_i * p_i for all primes */
    for (idx_t i = 0; i < prime_cnt; ++i) {
        struct Node n = {
            .val = (seq_t)primes[i] * (seq_t)primes[i],
            .i   = i,
            .j   = i
        };
        heap_push(n);
    }

    /* semiprime array initialization */
    semis_cap = 16;
    semis = (seq_t *)malloc(sizeof(seq_t) * semis_cap);
    semis_len = 0;

    initialized = true;
}

/* enlarge semis array on demand */
static void semis_ensure(size_t needed) {
    if (semis_cap >= needed) return;
    size_t new_cap = semis_cap ? semis_cap * 2 : 16;
    while (new_cap < needed) new_cap <<= 1;
    semis = (seq_t *)realloc(semis, new_cap * sizeof(seq_t));
    semis_cap = new_cap;
}

/* generate semiprimes up to 'target' count */
static void ensure_semiprimes(idx_t target) {
    while (semis_len < target) {
        struct Node cur = heap_pop();

        semis_ensure(semis_len + 1);
        semis[semis_len++] = cur.val;

        /* push next in the same sequence */
        idx_t next_j = cur.j + 1;
        if (next_j < prime_cnt) {
            struct Node nxt = {
                .val = (seq_t)primes[cur.i] * (seq_t)primes[next_j],
                .i   = cur.i,
                .j   = next_j
            };
            heap_push(nxt);
        }
    }
}

/* ---------- main sequence function ---------- */
seq_t a(idx_t n) {
    if (!initialized) init();

    if (n == 0) return 0;   /* offset is 1; undefined for n=0 */

    if (n > prime_cnt) {    /* extend prime list if requested n exceeds current prime count */
        /* simple expansion: rebuild with larger limit (rare for n<=10000) */
        idx_t new_limit = (idx_t)(primes[prime_cnt - 1]) * 2;
        uint64_t *new_primes = NULL;
        size_t new_cnt = 0;
        build_primes_upto(new_limit, &new_primes, &new_cnt);

        free(primes);
        free(heap);
        free(semis);

        primes = new_primes;
        prime_cnt = new_cnt;
        heap = (struct Node *)malloc(sizeof(struct Node) * prime_cnt);
        heap_sz = 0;
        for (idx_t i = 0; i < prime_cnt; ++i) {
            struct Node node = {
                .val = (seq_t)primes[i] * (seq_t)primes[i],
                .i   = i,
                .j   = i
            };
            heap_push(node);
        }
        semis_cap = 16;
        semis = (seq_t *)malloc(sizeof(seq_t) * semis_cap);
        semis_len = 0;
    }

    ensure_semiprimes(n);

    seq_t semiprime = semis[n - 1];
    seq_t prime    = (seq_t)primes[n - 1];
    return semiprime % prime;
}
