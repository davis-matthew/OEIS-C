/*
 * A009087  Numbers whose number of divisors is prime (i.e., numbers of the
 *          form p^(q-1) for primes p,q).
 * https://oeis.org/A009087
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nice, nonn, easy
 * Formula:   numbers of the form p^(q-1), where p and q are primes
 * Range:     u8 n <= 11, u16 n <= 534, u32 n <= 9946, u64 n <= 9946,
 *            u128 n <= 9946, big any n
 * Helpers:   big_pow, build_primes_upto, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation uses a min‑heap to generate the sequence in sorted order. */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/big_pow.h"
#include "../utils/build_primes_upto.h"
#include "../utils/seq_realloc.h"

static uint64_t *primes = NULL;
static size_t primes_count = 0;

static const size_t exponent_list_len = 18;
static const idx_t exponent_list[18] = {
    1, 2, 4, 6, 10, 12, 16, 18,
    22, 28, 30, 36, 40, 42, 46, 52, 54, 60
};

typedef struct {
    seq_t  val;   /* current value p^(q-1) */
    size_t p_idx; /* index in primes array */
    size_t e_idx; /* index in exponent_list array */
} Node;

/* --- global heap data ------------------------------------------- */
static Node *heap = NULL;
static size_t heap_size = 0;      /* current number of elements */

/* --- cache for computed terms ----------------------------------- */
static seq_t *cache = NULL;
static size_t cache_size = 0;
static size_t cache_capacity = 0;

/* --- helper functions for heap ----------------------------------- */
static void sift_down(size_t i) {
    while (true) {
        size_t left  = 2 * i + 1;
        size_t right = left + 1;
        size_t smallest = i;

        if (left < heap_size && heap[left].val < heap[smallest].val)
            smallest = left;
        if (right < heap_size && heap[right].val < heap[smallest].val)
            smallest = right;
        if (smallest == i) break;
        Node tmp = heap[i];
        heap[i] = heap[smallest];
        heap[smallest] = tmp;
        i = smallest;
    }
}

static void sift_up(size_t i) {
    while (i > 0) {
        size_t parent = (i - 1) >> 1;
        if (heap[parent].val <= heap[i].val) break;
        Node tmp = heap[parent];
        heap[parent] = heap[i];
        heap[i] = tmp;
        i = parent;
    }
}

/* --- ensure all data structures are initialized ---------------- */
static void init_sequence(void) {
    if (primes) return;                    /* already initialized */

    /* Build a list of primes up to a safe upper limit.
       600000 yields more than 50000 primes, which is far enough for 10000 terms. */
    const idx_t prime_limit = 600000;
    size_t cnt = 0;
    build_primes_upto(prime_limit, &primes, &cnt);
    primes_count = cnt;

    /* Allocate heap and fill with first powers (exponent 1). */
    heap = (Node *)malloc(primes_count * sizeof(Node));
    for (size_t i = 0; i < primes_count; ++i) {
        heap[i].val   = (seq_t)primes[i];   /* p^(1) */
        heap[i].p_idx = i;
        heap[i].e_idx = 0;                  /* exponent_list[0] == 1 */
    }

    /* Build min‑heap */
    for (size_t i = (primes_count >> 1); i > 0; --i)
        sift_down(i - 1);
    heap_size = primes_count;

    /* Initial cache allocation */
    cache = seq_realloc(NULL, 0, 64);
    cache_capacity = 64;
}

/* --- ensure cache has room for at least 'needed' entries --------- */
static void ensure_cache_capacity(size_t needed) {
    if (needed <= cache_capacity) return;
    size_t new_cap = cache_capacity ? cache_capacity << 1 : 64;
    if (needed > new_cap) new_cap = needed;
    cache = seq_realloc(cache, cache_capacity, new_cap);
    cache_capacity = new_cap;
}

/* --- generate terms up to 'up_to' (1‑based index) ---------------- */
static void generate_terms(idx_t up_to) {
    init_sequence();
    while (cache_size < up_to) {
        /* Extract smallest element from heap */
        Node node = heap[0];
        heap[0] = heap[heap_size - 1];
        --heap_size;
        sift_down(0);

        /* Store popped value */
        ensure_cache_capacity(cache_size + 1);
        cache[cache_size++] = node.val;

        /* Push next power for the same prime if available */
        size_t next_e = node.e_idx + 1;
        if (next_e < exponent_list_len) {
            seq_t next_val = big_pow((seq_t)primes[node.p_idx],
                                     (seq_t)exponent_list[next_e]);
            Node new_node = { next_val, node.p_idx, next_e };
            heap[heap_size++] = new_node;
            sift_up(heap_size - 1);
        }
    }
}

/* -------------------------------------------------------------- */
/* Public API: return a(n) for 1‑based index n.                  */
seq_t a(idx_t n) {
    if (n == 0) return 0;                /* undefined, but safeguard */
    if (n > cache_size) generate_terms(n);
    return cache[n - 1];
}
