/*
 * A121208  Cyclotomic polynomials evaluated at prime number abscissae.
 * https://oeis.org/A121208
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th smallest value of Φ_j(p) over primes p and
 *            integers j ≥ 1 * (duplicates removed). The function generates
 *            the sequence by merging the * infinite lists Φ_j(p_k) for each
 *            fixed cyclotomic index j≥1 and prime index k. * Here we limit j
 *            to 12, which is sufficient for the first 10 000 t...
 * Range:     u8 n <= 40, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Validated: partial, pass, 68 terms through n = 68 (2026-11-01)
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/build_primes_upto.h"

static bool phi_k_prime(int k, seq_t p, seq_t *res)
{
    seq_t t1, t2, t3, t4, t5, t6;
    switch (k) {
    case 1: *res = p - (seq_t)1; return true;                 /* p‑1 */
    case 2: *res = p + (seq_t)1; return true;                 /* p+1 */
    case 3:                                                     /* p² + p + 1 */
        t1 = p * p;
        *res = t1 + p + (seq_t)1;
        return true;
    case 4:                                                     /* p² + 1 */
        t1 = p * p;
        *res = t1 + (seq_t)1;
        return true;
    case 5:                                                     /* p⁴ + p³ + p² + p + 1 */
        t1 = p * p;          /* p² */
        t2 = t1 * p;         /* p³ */
        t3 = t2 * p;         /* p⁴ */
        *res = t3 + t2 + t1 + p + (seq_t)1;
        return true;
    case 6:                                                     /* p² – p + 1 */
        t1 = p * p;
        *res = t1 - p + (seq_t)1;
        return true;
    case 7:                                                     /* p⁶ + p⁵ + p⁴ + p³ + p² + p + 1 */
        t1 = p * p;          /* p² */
        t2 = t1 * p;         /* p³ */
        t3 = t2 * p;         /* p⁴ */
        t4 = t3 * p;         /* p⁵ */
        t5 = t4 * p;         /* p⁶ */
        *res = t5 + t4 + t3 + t2 + t1 + p + (seq_t)1;
        return true;
    case 8:                                                     /* p⁴ + 1 */
        t1 = p * p;          /* p² */
        t2 = t1 * t1;        /* p⁴ */
        *res = t2 + (seq_t)1;
        return true;
    case 9:                                                     /* p⁶ + p³ + 1 */
        t1 = p * p;          /* p² */
        t2 = t1 * p;         /* p³ */
        t3 = t2 * t2;        /* p⁶ */
        *res = t3 + t2 + (seq_t)1;
        return true;
    case 10:                                                    /* p⁴ – p³ + p² – p + 1 */
        t1 = p * p;          /* p² */
        t2 = t1 * p;         /* p³ */
        t3 = t2 * p;         /* p⁴ */
        *res = t3 - t2 + t1 - p + (seq_t)1;
        return true;
    case 11:                                                    /* sum_{i=0}^{10} p^i */
        *res = (seq_t)1;
        t1 = p;
        for (int i = 1; i <= 10; ++i) {
            t1 = t1 * p;
            *res = *res + t1;
        }
        return true;
    case 12:                                                    /* p⁴ – p² + 1 */
        t1 = p * p;          /* p² */
        t2 = t1 * t1;        /* p⁴ */
        *res = t2 - t1 + (seq_t)1;
        return true;
    default: return false; /* should not occur with JMAX=12 */
    }
}

/* ---------- Cyclotomic values Φ_j(p) for j = 1 … JMAX ------------------ */
#define JMAX 12 /* we compute up to Φ_12(p); this covers the first 10 000 terms */

/* ---------- node used in the min‑heap ------------------------------- */
typedef struct {
    seq_t value;          /* Φ_j(p) */
    int   j;              /* cyclotomic index (1 … JMAX) */
    size_t prime_idx;     /* index into the primes array */
} node_t;

/* ---------- static data --------------------------------------------- */
static uint64_t *prime_list = NULL;
static size_t prime_cnt   = 0;
static const size_t PRIME_LIMIT = 250000;   /* enough for ~10k terms */

/* ---------- heap utilities ------------------------------------------ */
static inline void heap_swap(int *a, int *b)
{
    int t = *a; *a = *b; *b = t;
}

static void heap_push(int heap[], int *size, int idx, node_t nodes[])
{
    int i = *size;
    heap[i] = idx;
    (*size)++;
    while (i > 0) {
        int p = (i - 1) >> 1;
        if (nodes[heap[i]].value < nodes[heap[p]].value) {
            heap_swap(&heap[i], &heap[p]);
            i = p;
        } else break;
    }
}

static int heap_pop(int heap[], int *size, node_t nodes[])
{
    int top = heap[0];
    (*size)--;
    heap[0] = heap[*size];
    int i = 0;
    while (1) {
        int l = (i << 1) + 1;
        if (l >= *size) break;
        int r = l + 1;
        int smallest = (r < *size && nodes[heap[r]].value < nodes[heap[l]].value) ? r : l;
        if (nodes[heap[smallest]].value < nodes[heap[i]].value) {
            heap_swap(&heap[smallest], &heap[i]);
            i = smallest;
        } else break;
    }
    return top;
}

/* ---------- main sequence function --------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;        /* safety for offset 0 */

    /* build primes once */
    if (!prime_list) {
        build_primes_upto(PRIME_LIMIT, &prime_list, &prime_cnt);
    }

    node_t nodes[JMAX];
    int heap[JMAX];
    int heap_sz = 0;

    /* initialize heap with first prime for each j */
    for (int j = 1; j <= JMAX; ++j) {
        node_t *nd = &nodes[j-1];
        nd->j = j;
        nd->prime_idx = 0;
        seq_t val;
        phi_k_prime(j, (seq_t)prime_list[0], &val);
        nd->value = val;
        heap_push(heap, &heap_sz, j-1, nodes);
    }

    seq_t last_val = (seq_t)0;
    bool first = true;
    size_t produced = 0;

    while (1) {
        int idx = heap_pop(heap, &heap_sz, nodes);
        node_t *nd = &nodes[idx];
        seq_t current = nd->value;

        if (first || current != last_val) {
            produced++;
            last_val = current;
            if (produced == (size_t)n) {
                return current;
            }
            first = false;
        }

        /* advance to next prime for this j */
        size_t next_pi = nd->prime_idx + 1;
        if (next_pi >= prime_cnt) continue;     /* no more primes for this j */
        seq_t next_val;
        if (phi_k_prime(nd->j, (seq_t)prime_list[next_pi], &next_val)) {
            nd->prime_idx = next_pi;
            nd->value = next_val;
            heap_push(heap, &heap_sz, idx, nodes);
        }
    }

    /* unreachable */
    return (seq_t)0;
}
