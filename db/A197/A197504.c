/*
 * A197504  1 together with the odd numbers m >= 3 for which phi(2*m)/2 =
 *          phi(m)/2 is odd, where phi = A000010 (Euler's totient).
 * https://oeis.org/A197504
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1) = 1; for n>=2, a(n) is the (n-1)-th smallest odd power of a
 *            prime p ≡ 3 (mod 4)
 * Range:     u8 n <= 36, u16 n <= 3330, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/build_primes_upto.h"

typedef uint64_t idx_t;   /* provided by the build */
typedef seq_t seq_t;      /* provided by the build */

/*--- heap -----------------------------------------------------------*/
struct heap_node {
    seq_t  val;   /* current power to emit */
    uint64_t prime;
};

static struct heap_node *heap = NULL;
static idx_t heap_size = 0;
static idx_t heap_cap  = 0;

static void heap_push(uint64_t p, seq_t v) {
    if (heap_size == heap_cap) {
        /* should not happen if heap_cap >= number of primes */
        size_t new_cap = heap_cap ? heap_cap << 1 : 16;
        heap = (struct heap_node *)realloc(heap, new_cap * sizeof(struct heap_node));
        heap_cap = (idx_t)new_cap;
    }
    size_t i = heap_size++;
    heap[i].prime = p;
    heap[i].val   = v;
    while (i > 0) {
        size_t parent = (i - 1) >> 1;
        if (heap[i].val >= heap[parent].val) break;
        struct heap_node tmp = heap[i];
        heap[i] = heap[parent];
        heap[parent] = tmp;
        i = parent;
    }
}

static struct heap_node heap_pop(void) {
    struct heap_node res = heap[0];
    size_t i = 0;
    heap[0] = heap[--heap_size];
    while (1) {
        size_t left  = (i << 1) + 1;
        size_t right = left + 1;
        size_t smallest = i;
        if (left  < heap_size && heap[left].val  < heap[smallest].val) smallest = left;
        if (right < heap_size && heap[right].val < heap[smallest].val) smallest = right;
        if (smallest == i) break;
        struct heap_node tmp = heap[i];
        heap[i] = heap[smallest];
        heap[smallest] = tmp;
        i = smallest;
    }
    return res;
}

/*--- sequence storage ----------------------------------------------*/
static seq_t *seq = NULL;
static idx_t   seq_len = 0;
static idx_t   seq_cap = 0;

static void ensure_seq_cap(idx_t need) {
    if (seq_cap >= need) return;
    size_t new_cap = seq_cap ? seq_cap << 1 : 16;
    if (new_cap < (size_t)need) new_cap = (size_t)need;
    seq = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
    seq_cap = (idx_t)new_cap;
}

/*--- primes ≡ 3 (mod 4) --------------------------------------------*/
static uint64_t *primes3 = NULL;
static idx_t   primes3_cnt = 0;

/*--- initialization -----------------------------------------------*/
static bool initialized = false;
static bool seq_size_is_small = (sizeof(seq_t) <= 8);
static seq_t seq_max_small = (seq_t)(-1);  /* only valid if seq_size_is_small */

static void init(void) {
    if (initialized) return;
    initialized = true;

    /* limit for sieve: generous enough for n up to ~10 000 */
    const idx_t INITIAL_LIMIT = 2000000ULL;
    idx_t limit = INITIAL_LIMIT;

    if (seq_size_is_small) {
        idx_t max_val_small = (idx_t)seq_max_small;
        if (limit > max_val_small) limit = max_val_small;
    }

    /* build all primes up to 'limit' and filter those ≡3 (mod 4) */
    uint64_t *allprimes = NULL;
    size_t allcnt = 0;
    build_primes_upto(limit, &allprimes, &allcnt);

    primes3_cnt = 0;
    primes3 = (uint64_t *)malloc(allcnt * sizeof(uint64_t));
    for (size_t i = 0; i < allcnt; ++i) {
        if (allprimes[i] % 4ULL == 3ULL) {
            primes3[primes3_cnt++] = allprimes[i];
        }
    }
    free(allprimes);

    /* heap capacity: one entry per prime (powers may replace themselves) */
    heap_cap = primes3_cnt;
    heap = (struct heap_node *)malloc(heap_cap * sizeof(struct heap_node));
    heap_size = 0;
    for (size_t i = 0; i < (size_t)primes3_cnt; ++i) {
        heap_push(primes3[i], (seq_t)primes3[i]); /* first power */
    }

    /* start sequence with 1 */
    seq_cap = 16;
    seq = (seq_t *)malloc(seq_cap * sizeof(seq_t));
    seq_len = 1;
    seq[0] = (seq_t)1;
}

/*--- generate terms up to at least index 'n' -----------------------*/
static void generate_terms(idx_t n) {
    while (seq_len < n) {
        struct heap_node node = heap_pop();

        ensure_seq_cap(seq_len + 1);
        seq[seq_len++] = node.val;

        /* compute next power of this prime */
        seq_t next_val;
        if (seq_size_is_small) {
            __uint128_t prod = (__uint128_t)node.val * node.prime;
            if (prod > (__uint128_t)seq_max_small) {
                continue; /* overflow – stop expanding this prime */
            }
            next_val = (seq_t)prod;
        } else {
            /* arbitrary-precision or 128‑bit: multiplication guaranteed to fit */
            next_val = node.val * (seq_t)node.prime;
        }
        heap_push(node.prime, next_val);
    }
}

/*--- public function -----------------------------------------------*/
seq_t a(idx_t n) {
    if (n == 0) return 0;  /* defined as 0 for safety */
    init();
    if (n > seq_len) generate_terms(n);
    return seq[n - 1];
}
