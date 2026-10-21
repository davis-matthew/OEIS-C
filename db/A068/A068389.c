/*
 * A068389  Differences between the primes generating the n-th prime power.
 * https://oeis.org/A068389
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 none, i16 none, i32 none, i64 n <= 64, i128 n <= 64,
 *            big any n
 * Helpers:   build_primes_upto, seq_mul, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequences A068389 – differences between the primes generating successive
   prime powers.  The function builds the sequence on demand using a
   priority‑queue of prime powers and memoises the differences up to the
   largest n requested.  It works for any seq_t and idx_t supplied by the
   build system. */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/build_primes_upto.h"
#include "../utils/seq_mul.h"
#include "../utils/seq_pow.h"

/* Node holding a current prime power and its generating prime. */
struct Node
{
    seq_t val;      /* current prime power */
    uint64_t prime; /* generating prime */
};

/* Simple binary min‑heap of Node ordered by increasing .val. */
struct Heap
{
    struct Node *data;
    size_t size;
    size_t cap;
};

static void heap_push(struct Heap *h, seq_t val, uint64_t prime)
{
    if (h->size == h->cap)
    {
        h->cap = h->cap ? h->cap << 1 : 8;
        h->data = (struct Node *)realloc(h->data, h->cap * sizeof(struct Node));
    }
    size_t i = h->size++;
    h->data[i] = (struct Node){val, prime};
    while (i > 0)
    {
        size_t p = (i - 1) >> 1;
        if (h->data[p].val <= h->data[i].val) break;
        struct Node tmp = h->data[p];
        h->data[p] = h->data[i];
        h->data[i] = tmp;
        i = p;
    }
}

static struct Node heap_pop(struct Heap *h)
{
    struct Node ret = h->data[0];
    h->data[0] = h->data[--h->size];
    size_t i = 0;
    for (;;)
    {
        size_t l = (i << 1) + 1;
        size_t r = l + 1;
        size_t smallest = i;
        if (l < h->size && h->data[l].val < h->data[smallest].val)
            smallest = l;
        if (r < h->size && h->data[r].val < h->data[smallest].val)
            smallest = r;
        if (smallest == i) break;
        struct Node tmp = h->data[i];
        h->data[i] = h->data[smallest];
        h->data[smallest] = tmp;
        i = smallest;
    }
    return ret;
}

/* Memoised state for the sequence. */
struct SeqState
{
    seq_t *diffs;      /* stored differences a(0), a(1), ... */
    size_t len;        /* number of stored differences */
    size_t cap;        /* capacity of diffs array */
    struct Heap heap;  /* heap of next prime powers */
    uint64_t *primes;  /* list of primes up to LIMIT */
    size_t pcnt;       /* number of primes in that list */
    seq_t prev_prime;  /* generating prime of the last processed power */
    size_t num_powers; /* how many prime powers have been processed */
    bool init;
};

static struct SeqState state = {0};

static void init_state(void)
{
    if (state.init) return;
    /* Limit large enough for n up to ~10 000. */
    const idx_t LIMIT = 250000;
    build_primes_upto(LIMIT, &state.primes, &state.pcnt);

    state.heap.size = 0;
    state.heap.cap   = state.pcnt;
    state.heap.data  = (struct Node *)malloc(state.heap.cap * sizeof(struct Node));

    /* Initialise heap with p² for every prime p. */
    for (size_t i = 0; i < state.pcnt; ++i)
    {
        seq_t val = seq_mul((seq_t)state.primes[i], (seq_t)state.primes[i]);
        heap_push(&state.heap, val, state.primes[i]);
    }

    state.diffs     = NULL;
    state.len       = 0;
    state.cap       = 0;
    state.num_powers= 0;
    state.init      = true;
}

seq_t a(idx_t n)
{
    init_state();
    /* Need n+2 prime powers to compute a(n) because a(n) is the
       difference of generating primes of power number n+1 and n+2. */
    while (state.num_powers <= (size_t)n + 1)
    {
        struct Node node = heap_pop(&state.heap);
        seq_t curr = (seq_t)node.prime;
        if (state.num_powers == 0)
        {
            /* First power – nothing to compare yet. */
            state.prev_prime = curr;
        }
        else
        {
            seq_t diff = curr - state.prev_prime;
            if (state.len == state.cap)
            {
                state.cap = state.cap ? state.cap << 1 : 128;
                state.diffs = (seq_t *)realloc(state.diffs,
                                               state.cap * sizeof(seq_t));
            }
            state.diffs[state.len++] = diff;
            state.prev_prime = curr;
        }
        ++state.num_powers;
        /* Push the next power of this prime into the heap. */
        seq_t next_val = seq_mul(node.val, (seq_t)node.prime);
        heap_push(&state.heap, next_val, node.prime);
    }
    return state.diffs[(size_t)n];
}
