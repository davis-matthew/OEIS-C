/*
 * A108348  Numbers of the form p^k + p^(k-1) + ... + p + 1 (where p is a
 *          prime and k>=0) in ascending order.
 * https://oeis.org/A108348
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A108348: Numbers of the form (p^(k+1)-1)/(p-1) (p prime, k≥0)
   sorted in ascending order.  The implementation keeps a min‑heap of the
   next value for each prime and extracts the global minimum, suppressing
   duplicates.  Values are stored in a static array that grows as needed. */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

/* ------------------------------------------------------------------ */
/* State ------------------------------------------------------------- */
/* ------------------------------------------------------------------ */
typedef struct {
    uint32_t *primes;       /* primes array                                 */
    size_t    prime_cnt;    /* number of primes                               */

    struct heap_node *heap; /* min‑heap of size prime_cnt                     */

    seq_t *vals;        /* unique sorted sequence values                 */
    size_t vals_cap;    /* capacity of vals array                        */
    size_t vals_size;   /* number of values stored                       */
    seq_t  last_val;    /* last value stored to skip duplicates          */
} seq_state_t;

struct heap_node {
    uint32_t p;   /* prime concerned */
    seq_t    val; /* current geometric sum */
};

static seq_state_t state = {0};

/* ------------------------------------------------------------------ */
/* Prime generation (simple sieve up to LIMIT) ---------------------- */
/* ------------------------------------------------------------------ */
static void init_primes(void)
{
    /* The bound is large enough to cover thousands of terms. */
    const uint32_t LIMIT = 500000;

    /* Allocate the sieve array.  In C++ we must cast the return value. */
    char *is_comp = (char*)calloc((size_t)LIMIT + 1, 1);
    if (!is_comp) exit(EXIT_FAILURE);

    /* Temporary array for primes. */
    uint32_t *pr = NULL;
    size_t cap = 200000;   /* initial capacity */
    pr = (uint32_t*)malloc(cap * sizeof(uint32_t));
    if (!pr) exit(EXIT_FAILURE);
    size_t cnt = 0;

    for (uint32_t i = 2; i <= LIMIT; ++i) {
        if (!is_comp[i]) {
            if (cnt == cap) {
                cap <<= 1;
                pr = (uint32_t*)realloc(pr, cap * sizeof(uint32_t));
                if (!pr) exit(EXIT_FAILURE);
            }
            pr[cnt++] = i;
            if ((uint64_t)i * i <= LIMIT) {
                for (uint64_t j = (uint64_t)i * i; j <= LIMIT; j += i)
                    is_comp[(size_t)j] = 1;
            }
        }
    }
    free(is_comp);

    state.primes    = pr;
    state.prime_cnt = cnt;
}

/* ------------------------------------------------------------------ */
/* Heap helpers ----------------------------------------------------- */
/* ------------------------------------------------------------------ */
static void heap_sift_down(size_t idx)
{
    size_t size = state.prime_cnt;
    while (true) {
        size_t l = 2 * idx + 1;
        if (l >= size) break;
        size_t r = l + 1;
        size_t smallest = l;
        if (r < size && state.heap[r].val < state.heap[l].val)
            smallest = r;
        if (state.heap[smallest].val < state.heap[idx].val) {
            struct heap_node tmp = state.heap[smallest];
            state.heap[smallest] = state.heap[idx];
            state.heap[idx] = tmp;
            idx = smallest;
        } else break;
    }
}

static void heap_build(void)
{
    for (int64_t i = (int64_t)state.prime_cnt / 2 - 1; i >= 0; --i)
        heap_sift_down((size_t)i);
}

/* ------------------------------------------------------------------ */
/* Value storage ---------------------------------------------------- */
/* ------------------------------------------------------------------ */
static void append_value(seq_t v)
{
    if (state.vals_size == state.vals_cap) {
        state.vals_cap <<= 1;
        state.vals = (seq_t*)realloc(state.vals, state.vals_cap * sizeof(seq_t));
        if (!state.vals) exit(EXIT_FAILURE);
    }
    state.vals[state.vals_size++] = v;
}

/* ------------------------------------------------------------------ */
/* Initialization wrapper ------------------------------------------- */
/* ------------------------------------------------------------------ */
static void ensure_initialized(void)
{
    static bool initialized = false;
    if (!initialized) {
        /* Generate primes. */
        init_primes();

        /* Allocate heap and initialize first sums (k = 1). */
        state.heap = (struct heap_node*)malloc(state.prime_cnt * sizeof(struct heap_node));
        if (!state.heap) exit(EXIT_FAILURE);
        for (size_t i = 0; i < state.prime_cnt; ++i) {
            state.heap[i].p   = state.primes[i];
            state.heap[i].val = (seq_t)(state.primes[i] + 1u); /* p^1 + ... + 1 with k=1 */
        }
        heap_build();

        /* Prepare the value array. */
        state.vals_cap = 1024;
        state.vals = (seq_t*)malloc(state.vals_cap * sizeof(seq_t));
        if (!state.vals) exit(EXIT_FAILURE);
        state.vals_size = 0;
        state.last_val  = 0;

        /* The term 1 (k = 0) appears only once. */
        append_value((seq_t)1);
        state.last_val = (seq_t)1;

        initialized = true;
    }
}

/* ------------------------------------------------------------------ */
/* Main sequence function -------------------------------------------- */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;     /* undefined for n=0 */

    ensure_initialized();

    /* Generate values until we have at least n terms. */
    while (state.vals_size < (size_t)n) {
        /* Smallest current sum from the heap. */
        seq_t cur = state.heap[0].val;

        /* Store unique value. */
        if (state.vals_size == 0 || cur != state.last_val) {
            append_value(cur);
            state.last_val = cur;
        }

        /* Compute next sum for the same prime: S' = p * S + 1. */
        uint32_t p = state.heap[0].p;
        seq_t next = cur * (seq_t)p + (seq_t)1;
        state.heap[0].val = next;

        heap_sift_down(0);
    }

    return state.vals[(size_t)n - 1];
}
