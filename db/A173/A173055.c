/*
 * A173055  Numbers of the form a^b+b^a, a and b are odd primes, b > a.
 * https://oeis.org/A173055
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n‑th smallest value of a^b + b^a where a and b are
 *            odd primes, b > a
 * Range:     u8 none, u16 n <= 2, u32 n <= 7, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   big_pow, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/big_pow.h"     /* seq_t big_pow(seq_t base, seq_t exp)          */
#include "../utils/next_prime.h"  /* uint64_t next_prime(uint64_t start)          */

/* -------------------------------------------------------------------- */
typedef struct {
    seq_t val;        /* a^b + b^a */
    size_t a_idx;     /* index of a in primes array   */
    size_t b_idx;     /* index of b in primes array   */
} node_t;

/* -------------------------------------------------------------------- */
/* Dynamic array helpers (generic, cast to concrete type after call).   */
static void *resize_arr(void *ptr, size_t *cap, size_t new_len, size_t elem_size)
{
    if (new_len > *cap) {
        size_t new_cap = (*cap == 0) ? 8 : (*cap << 1);
        while (new_cap < new_len) new_cap <<= 1;
        ptr = realloc(ptr, new_cap * elem_size);
        *cap = new_cap;
    }
    return ptr;
}

/* -------------------------------------------------------------------- */
/* Global state (static to persist across calls)                        */
static uint64_t *primes      = NULL;   /* list of odd primes (3,5,7,…)      */
static size_t   primes_len   = 0;      /* current number of primes         */
static size_t   primes_cap   = 0;      /* allocated capacity for primes    */
static uint64_t last_prime   = 2;      /* last prime generated (starts with 2) */

static node_t *heap         = NULL;   /* min‑heap of candidate values    */
static size_t  heap_sz      = 0;      /* number of elements in heap       */
static size_t  heap_cap     = 0;      /* capacity of heap array           */

static seq_t *cache         = NULL;   /* cache of computed terms          */
static size_t  cache_sz     = 0;
static size_t  cache_cap    = 0;

/* -------------------------------------------------------------------- */
/* Add the next odd prime and, if applicable, create the node for the   */
/* pair (previous_prime, new_prime).                                    */
static void add_prime(void)
{
    last_prime = next_prime(last_prime);                       /* 3,5,7,… */
    primes = (uint64_t *)resize_arr(primes, &primes_cap, primes_len + 1,
                                    sizeof(uint64_t));
    primes[primes_len++] = last_prime;

    /* When we have at least two primes, the new prime can serve as b for the
       immediately preceding prime a. Create the node for that pair.              */
    if (primes_len >= 2) {
        size_t a_idx = primes_len - 2;   /* previous prime          */
        size_t b_idx = primes_len - 1;   /* new prime               */
        seq_t  a_val = (seq_t)primes[a_idx];
        seq_t  b_val = (seq_t)primes[b_idx];
        seq_t  val   = big_pow(a_val, b_val) + big_pow(b_val, a_val);

        heap = (node_t *)resize_arr(heap, &heap_cap, heap_sz + 1,
                                    sizeof(node_t));
        heap[heap_sz++] = (node_t){val, a_idx, b_idx};
    }
}

/* -------------------------------------------------------------------- */
/* Ensure we have at least 'need_index' primes (0‑based).              */
static void ensure_primes_until(size_t need_index)
{
    while (primes_len <= need_index) add_prime();
}

/* -------------------------------------------------------------------- */
/* Min‑heap push operation.                                            */
static void heap_push(node_t node)
{
    heap = (node_t *)resize_arr(heap, &heap_cap, heap_sz + 1, sizeof(node_t));
    size_t i = heap_sz++;
    heap[i] = node;
    while (i) {
        size_t p = (i - 1) >> 1;
        if (heap[p].val <= heap[i].val) break;
        node_t tmp = heap[p]; heap[p] = heap[i]; heap[i] = tmp;
        i = p;
    }
}

/* -------------------------------------------------------------------- */
/* Min‑heap pop operation.                                             */
static node_t heap_pop(void)
{
    node_t top = heap[0];
    heap_sz--;
    if (heap_sz) {
        heap[0] = heap[heap_sz];
        size_t i = 0;
        while (1) {
            size_t l = (i << 1) + 1;
            size_t r = l + 1;
            size_t smallest = i;
            if (l < heap_sz && heap[l].val < heap[smallest].val) smallest = l;
            if (r < heap_sz && heap[r].val < heap[smallest].val) smallest = r;
            if (smallest == i) break;
            node_t tmp = heap[i]; heap[i] = heap[smallest]; heap[smallest] = tmp;
            i = smallest;
        }
    }
    return top;
}

/* -------------------------------------------------------------------- */
/* Push a node for the pair (a_idx, b_idx). Assumes that primes array has
   at least b_idx+1 elements.                                           */
static void push_node(size_t a_idx, size_t b_idx)
{
    seq_t a_val = (seq_t)primes[a_idx];
    seq_t b_val = (seq_t)primes[b_idx];
    seq_t val   = big_pow(a_val, b_val) + big_pow(b_val, a_val);
    heap_push((node_t){val, a_idx, b_idx});
}

/* -------------------------------------------------------------------- */
/* Add one value to the cache.                                           */
static void cache_push(seq_t val)
{
    cache = (seq_t *)resize_arr(cache, &cache_cap, cache_sz + 1,
                                sizeof(seq_t));
    cache[cache_sz++] = val;
}

/* -------------------------------------------------------------------- */
/* One‑time initialization of the first two odd primes and their node. */
static void init_once(void)
{
    add_prime(); /* primes[0] = 3, no node yet */
    add_prime(); /* primes[1] = 5, node (3,5) added */
}

/* -------------------------------------------------------------------- */
/* Main sequence function.                                             */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined, but harmless */

    /* Ensure that initial state is set up once. */
    if (cache_sz == 0) init_once();

    size_t target = (size_t)n;
    while (cache_sz < target) {
        /* Extract smallest pending value. */
        node_t cur = heap_pop();
        cache_push(cur.val);

        /* Attempt to push the next candidate for the same 'a' (next larger prime for 'b'). */
        size_t next_b_idx = cur.b_idx + 1;
        ensure_primes_until(next_b_idx);  /* makes sure primes[next_b_idx] exists */
        push_node(cur.a_idx, next_b_idx);
    }
    return cache[target - 1];
}
