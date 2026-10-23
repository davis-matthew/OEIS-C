/*
 * A184338  "Types" (parities of i and j) when all the numbers ir+j are
 *          ranked, where r=golden ratio, i>=0, j>=0.
 * https://oeis.org/A184338
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 96, u16 n <= 96, u32 n <= 96, u64 n <= 96, u128 n <= 96,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Sequence A184338: Types
 * For a given index n (1‑based) return the parity type of the pair (i,j) that
 * becomes the n-th smallest number in the ordering of { i·φ + j | i≥0, j≥0 }
 * where φ = (1+√5)/2 is the golden ratio.
 *
 * Scheme:
 *   - Approximate φ by the convergent 1346269/832040 (Fibonacci ratio F31/F30).
 *   - Define an integer key K(i,j) = j·Q + i·P where P/Q ≈ φ.
 *   - K preserves the order of i·φ + j, because the approximation error is
 *     < 1/Q and < 1 per pair.
 *   - Generate the first n pairs in ascending order of K using a min‑heap.
 *   - The heap now contains only unique pairs thanks to a simple open‑address
 *     hash table that records visited (i,j) pairs.
 *   - When the n‑th pair is popped, map its parities to the required type:
 *     (odd,odd)   → 1
 *     (odd,even)  → 2
 *     (even,odd)  → 3
 *     (even,even) → 4
 */

#include <stdlib.h>
#include <stdint.h>

/* Approximation of φ: P/Q = 1346269 / 832040  (F31/F30). */
#define P 1346269U
#define Q 832040U

/* Maximum number of nodes we ever keep in the heap; generous safety bound. */
#define HEAP_FACTOR 4
/* Initial capacity of the hash table for visited pairs (power of two). */
#define HASH_FACTOR   4

/* Structure for heap elements. */
typedef struct {
    uint64_t key;   /* j·Q + i·P */
    idx_t i;
    idx_t j;
} node_t;

/* Simple binary min‑heap. */
static void heap_push(node_t *heap, size_t *size, node_t n)
{
    size_t idx = *size;
    heap[idx] = n;
    ++*size;
    /* sift up */
    while (idx > 0) {
        size_t parent = (idx - 1) >> 1;
        if (heap[parent].key <= heap[idx].key) break;
        node_t tmp = heap[parent];
        heap[parent] = heap[idx];
        heap[idx] = tmp;
        idx = parent;
    }
}

static node_t heap_pop(node_t *heap, size_t *size)
{
    node_t top = heap[0];
    --*size;
    heap[0] = heap[*size];
    /* sift down */
    size_t idx = 0;
    while (1) {
        size_t l = (idx << 1) + 1;
        if (l >= *size) break;
        size_t r = l + 1;
        size_t smallest = l;
        if (r < *size && heap[r].key < heap[l].key) smallest = r;
        if (heap[smallest].key >= heap[idx].key) break;
        node_t tmp = heap[smallest];
        heap[smallest] = heap[idx];
        heap[idx] = tmp;
        idx = smallest;
    }
    return top;
}

/* Simple open‑address hash table for visited pairs.  Keys are 64‑bit (i<<32)|j. */
/* All entries hold 0 when empty; (0,0) is never inserted again, so 0 is safe. */
static int hash_insert(uint64_t *table, size_t capacity, uint64_t key)
{
    size_t idx = (key * 11400714819323198485ULL) >> (64 - __builtin_ctzl(capacity));
    while (1) {
        if (table[idx] == 0) {
            table[idx] = key;
            return 1;   /* inserted */
        }
        if (table[idx] == key) return 0;   /* already present */
        idx = (idx + 1) & (capacity - 1);
    }
}

/* Main function. */
seq_t a(idx_t n)
{
    /* Allocate data structures. */
    size_t heap_cap = HEAP_FACTOR * n + 10;
    node_t *heap = (node_t*)malloc(heap_cap * sizeof(node_t));
    if (!heap) return (seq_t)0; /* fallback on allocation failure */

    size_t hash_size = HASH_FACTOR * n + 10;
    /* round up to power of two */
    hash_size = 1ULL << (64 - __builtin_clzl(hash_size));
    uint64_t *hash = (uint64_t*)calloc(hash_size, sizeof(uint64_t));
    if (!hash) {
        free(heap);
        return (seq_t)0;
    }

    /* Start with (0,0).  Its key is 0. */
    size_t hsz = 0;
    heap_push(heap, &hsz, (node_t){ .key = 0, .i = 0, .j = 0 });

    seq_t result = (seq_t)0;
    idx_t count = 0;
    while (count < n && hsz > 0) {
        node_t cur = heap_pop(heap, &hsz);
        ++count;
        if (count == n) {
            int i_odd = cur.i & 1;
            int j_odd = cur.j & 1;
            if (i_odd && j_odd) result = 1;
            else if (i_odd && !j_odd) result = 2;
            else if (!i_odd && j_odd) result = 3;
            else result = 4;
            break;
        }
        /* generate neighbors (i+1,j) and (i,j+1) */
        idx_t ni = cur.i + 1;
        idx_t nj = cur.j;
        uint64_t key_pair = ((uint64_t)ni << 32) | (uint64_t)nj;
        if (hash_insert(hash, hash_size, key_pair)) {
            uint64_t k = (uint64_t)nj * Q + (uint64_t)ni * P;
            heap_push(heap, &hsz, (node_t){ .key = k, .i = ni, .j = nj });
        }

        ni = cur.i;
        nj = cur.j + 1;
        key_pair = ((uint64_t)ni << 32) | (uint64_t)nj;
        if (hash_insert(hash, hash_size, key_pair)) {
            uint64_t k = (uint64_t)nj * Q + (uint64_t)ni * P;
            heap_push(heap, &hsz, (node_t){ .key = k, .i = ni, .j = nj });
        }
    }

    free(heap);
    free(hash);
    return result;
}
