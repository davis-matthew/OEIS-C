/*
 * A121016  Numbers whose binary expansion is properly periodic.
 * https://oeis.org/A121016
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 19, u32 n <= 2618, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A121016 – numbers whose binary expansion is properly periodic.
   The algorithm generates all primitive binary patterns of length up to
   a chosen maximum (16 bits).  For each pattern it pushes the smallest
   number that uses it (two repetitions).  A binary min‑heap keeps the
   next candidate from each pattern; the sequence values are output in
   ascending order while duplicates are skipped.  The function can be
   called in any order; results are memoised. */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <limits.h>

/* Node in the min‑heap, representing a pattern B (length d) repeated m times.
   N = value of B repeated m times. */
typedef struct {
    seq_t N;    /* current number (N_m) */
    seq_t B;    /* base pattern value */
    unsigned d; /* length of pattern in bits */
    unsigned m; /* current repetition count (>=2) */
} Node;

static Node *heap = NULL;
static size_t heap_cap = 0;
static size_t heap_size = 0;

static seq_t *cache = NULL;
static size_t cache_cap = 0;
static size_t cache_count = 0;

static bool initialized = false;
static const unsigned D_MAX = 16; /* maximum pattern length to generate */

/* --- Utility functions --------------------------------------------------- */

/* Return true iff the binary representation of B (exactly d bits, msb 1)
   is primitive (cannot be expressed as a shorter pattern repeated). */
static bool is_primitive(uint64_t B, unsigned d)
{
    if (d == 1) return true;
    for (unsigned k = 1; k < d; ++k) {
        if (d % k == 0) {
            unsigned len = k;
            unsigned repeat = d / k;
            uint64_t sub = B >> (d - len);
            uint64_t repeated = sub;
            for (unsigned r = 1; r < repeat; ++r) {
                repeated = (repeated << len) | sub;
            }
            if (repeated == B) return false;
        }
    }
    return true;
}

/* --- Heap operations ----------------------------------------------------- */

static void heap_swap(Node *a, Node *b)
{
    Node tmp = *a;
    *a = *b;
    *b = tmp;
}

static void heap_push(Node const *item)
{
    if (heap_size == heap_cap) {
        size_t new_cap = heap_cap ? heap_cap * 2 : 64;
        Node *new_heap = (Node *)realloc(heap, new_cap * sizeof(Node));
        if (!new_heap) exit(EXIT_FAILURE);
        heap = new_heap;
        heap_cap = new_cap;
    }
    heap[heap_size++] = *item;
    size_t pos = heap_size - 1;
    while (pos > 0) {
        size_t parent = (pos - 1) >> 1;
        if (heap[pos].N >= heap[parent].N) break;
        heap_swap(&heap[pos], &heap[parent]);
        pos = parent;
    }
}

static Node heap_pop_min(void)
{
    Node min = heap[0];
    heap[0] = heap[--heap_size];
    size_t pos = 0;
    while (1) {
        size_t left = (pos << 1) + 1;
        if (left >= heap_size) break;
        size_t right = left + 1;
        size_t child = (right < heap_size && heap[right].N < heap[left].N) ? right : left;
        if (heap[child].N >= heap[pos].N) break;
        heap_swap(&heap[pos], &heap[child]);
        pos = child;
    }
    return min;
}

/* --- Cache handling ------------------------------------------------------ */

static void cache_append(seq_t value)
{
    if (cache_count == cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 64;
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache) exit(EXIT_FAILURE);
        cache = new_cache;
        cache_cap = new_cap;
    }
    cache[cache_count++] = value;
}

/* --- Sequence initialisation --------------------------------------------- */

static void init_sequence(void)
{
    if (initialized) return;

    /* Generate all primitive patterns of length d (1 <= d <= D_MAX). */
    for (unsigned d = 1; d <= D_MAX; ++d) {
        uint64_t start = 1ULL << (d - 1);            /* msb must be 1 */
        uint64_t end   = (1ULL << d) - 1ULL;         /* all d‑bit patterns */
        for (uint64_t B = start; B <= end; ++B) {
            if (!is_primitive(B, d)) continue;
            seq_t Bseq = (seq_t)B;
            /* First number: two repetitions of B. */
            seq_t N2 = Bseq + (Bseq << d);
            Node n = {N2, Bseq, d, 2};
            heap_push(&n);
        }
    }
    initialized = true;
}

/* --- Main function ------------------------------------------------------- */

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined for n=0, but safe */

    init_sequence();

    /* Generate cached values up to requested n. */
    while (cache_count < (size_t)n) {
        Node cur = heap_pop_min();

        /* Store only distinct numbers. */
        if (cache_count == 0 || cur.N != cache[cache_count - 1]) {
            cache_append(cur.N);
        }

        /* Generate next repetition of the same pattern if it fits. */
        unsigned shift = cur.m * cur.d;
        /* For built‑in unsigned types, guard against shift overflow. */
        if (sizeof(seq_t) <= 8) {
            if (shift < (sizeof(seq_t) * CHAR_BIT)) {
                seq_t nextN = cur.N + (cur.B << shift);
                Node nxt = {nextN, cur.B, cur.d, cur.m + 1};
                heap_push(&nxt);
            }
        } else { /* arbitrary‑precision: no overflow guard needed */
            seq_t nextN = cur.N + (cur.B << shift);
            Node nxt = {nextN, cur.B, cur.d, cur.m + 1};
            heap_push(&nxt);
        }
    }

    return cache[n - 1];
}
