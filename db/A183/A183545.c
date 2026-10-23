/*
 * A183545  Ordering of the numbers in the tree A183543; complement of
 *          A183544.
 * https://oeis.org/A183545
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the order of the numbers in the tree generated * by: 3
 *            is in S; if n∈S then * -2 + ⌊(n+2)φ⌋ and n + ⌊(n+2)φ⌋ are in S,
 *            * where φ=(1+√5)/2 is the golden ratio. * a(n) lists the
 *            elements of S in increasing order, with a(1)=3. * This
 *            implementation builds the tree with a min‑heap and a hash...
 * Range:     u8 n <= 57, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   _floor_isqrt_u64, hash_contains
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#include "../utils/_floor_isqrt_u64.h"
#include "../utils/hash_contains.h"

/* Compute the smallest power of two ≥ v.  v>0. */
static size_t next_pow2(size_t v)
{
    if (v == 0) return 1;
    v--;
    v |= v >> 1;
    v |= v >> 2;
    v |= v >> 4;
    v |= v >> 8;
    v |= v >> 16;
#if SIZE_MAX > 0xffffffff
    v |= v >> 32;
#endif
    return v + 1;
}

static void hash_insert(idx_t key, idx_t *tab, size_t cap)
{
    size_t h = (size_t)key & (cap - 1);
    while (tab[h]) h = (h + 1) & (cap - 1);
    tab[h] = key;
}

/* Min‑heap over idx_t values.  heap is a pointer to the array,
   hs is the current size, hcap is the capacity. */
static void heap_push(idx_t **heap, size_t *hs, size_t *hcap, idx_t val)
{
    if (*hs == *hcap) {
        *hcap = (*hcap == 0) ? 16 : (*hcap << 1);
        *heap = (idx_t *)realloc(*heap, *hcap * sizeof(idx_t));
        if (!*heap) exit(1);
    }
    size_t i = (*hs)++;
    while (i > 0) {
        size_t p = (i - 1) >> 1;
        if ((*heap)[p] <= val) break;
        (*heap)[i] = (*heap)[p];
        i = p;
    }
    (*heap)[i] = val;
}

static idx_t heap_pop(idx_t *heap, size_t *hs)
{
    idx_t top = heap[0];
    idx_t last = heap[--(*hs)];
    size_t i = 0;
    while (1) {
        size_t l = (i << 1) + 1;
        if (l >= *hs) break;
        size_t r = l + 1;
        size_t s = l;
        if (r < *hs && heap[r] < heap[l]) s = r;
        if (heap[s] >= last) break;
        heap[i] = heap[s];
        i = s;
    }
    heap[i] = last;
    return top;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* undefined for 0, harmless */

    /* Allocate structures.  4×n leaves room for all vertices up to depth ~log₂n. */
    size_t cap = next_pow2((size_t)n * 4 + 1);
    idx_t *visited = (idx_t *)calloc(cap, sizeof(idx_t));
    if (!visited) exit(1);

    idx_t *heap = (idx_t *)NULL;
    size_t hsize = 0, hcap = 0;

    /* seed root 3 */
    hash_insert(3, visited, cap);
    heap_push(&heap, &hsize, &hcap, 3);

    idx_t cur = 0;
    idx_t count = 0;

    while (count < n) {
        cur = heap_pop(heap, &hsize);
        count++;

        if (count == n) {
            free(heap);
            free(visited);
            return (seq_t)cur;
        }

        /* Generate the two children for the current value cur. */
        idx_t k = cur;
        idx_t val = k + 2;                           /* (k+2) */
        idx_t sq = val * val;                       /* (k+2)^2 */
        idx_t sq5 = 5 * sq;                         /* 5*(k+2)^2 */
        idx_t sqrt_term = _floor_isqrt_u64(sq5);     /* floor(sqrt(5)* (k+2)) */
        idx_t t = (val + sqrt_term) / 2;             /* floor((k+2)*φ) */

        idx_t child1 = t - 2;        /* -2 + floor((k+2)*φ) */
        idx_t child2 = k + t;        /* k + floor((k+2)*φ) */

        if (!hash_contains(child1, visited, cap)) {
            hash_insert(child1, visited, cap);
            heap_push(&heap, &hsize, &hcap, child1);
        }
        if (!hash_contains(child2, visited, cap)) {
            hash_insert(child2, visited, cap);
            heap_push(&heap, &hsize, &hcap, child2);
        }
    }

    /* control never reaches here */
    free(heap);
    free(visited);
    return 0;
}
