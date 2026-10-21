/*
 * A096916  Lesser prime factor of n-th product of two distinct primes.
 * https://oeis.org/A096916
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = the smaller prime factor of the n‑th product of two
 *            distinct primes * (i.e. the least prime p_i such that p_i·p_j is
 *            the n‑th smallest product with i<j). * * The routine generates
 *            all products p_i·p_j (i<j) in ascending order using a *
 *            min‑heap. It keeps the heap entries small by storing t...
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* ---------- Prime sieve --------------------------------------------------- */
static void sieve_primes(idx_t limit, uint32_t **pr, idx_t *cnt)
{
    if (limit < 2) {
        *pr   = NULL;
        *cnt  = 0;
        return;
    }

    uint8_t *is_comp = (uint8_t *)calloc((size_t)limit + 1, 1);
    if (!is_comp) {
        *pr = NULL;
        *cnt = 0;
        return;
    }

    for (idx_t i = 2; (i * i) <= limit; ++i) {
        if (!is_comp[i]) {
            for (idx_t j = i * i; j <= limit; j += i)
                is_comp[j] = 1;
        }
    }

    idx_t m = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (!is_comp[i]) ++m;

    uint32_t *arr = (uint32_t *)malloc((size_t)m * sizeof(uint32_t));
    if (!arr) {
        free(is_comp);
        *pr = NULL;
        *cnt = 0;
        return;
    }

    idx_t index = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (!is_comp[i]) arr[index++] = (uint32_t)i;

    free(is_comp);
    *pr  = arr;
    *cnt = m;
}

/* ---------- Heap for (i,j) pairs ---------------------------------------- */
struct Node {
    uint64_t prod;  /* p_i * p_j */
    idx_t   i;      /* prime index i   */
    idx_t   j;      /* prime index j   */
};

static void heap_push(struct Node *heap, idx_t *size, struct Node node)
{
    ++(*size);
    heap[*size] = node;
    idx_t pos = *size;
    while (pos > 1) {
        idx_t parent = pos >> 1;
        if (heap[parent].prod <= heap[pos].prod) break;
        struct Node tmp = heap[parent];
        heap[parent]   = heap[pos];
        heap[pos]      = tmp;
        pos = parent;
    }
}

static struct Node heap_pop(struct Node *heap, idx_t *size)
{
    struct Node min = heap[1];
    heap[1] = heap[*size];
    --(*size);
    idx_t pos = 1;
    while (1) {
        idx_t left  = pos << 1;
        idx_t right = left + 1;
        if (left > *size) break;
        idx_t child = left;
        if (right <= *size && heap[right].prod < heap[left].prod)
            child = right;
        if (heap[child].prod >= heap[pos].prod) break;
        struct Node tmp = heap[pos];
        heap[pos]       = heap[child];
        heap[child]     = tmp;
        pos = child;
    }
    return min;
}

/* ---------- Main sequence function --------------------------------------- */
seq_t a(idx_t n)
{
    if (n < 1) return 0;   /* unused, but harmless */

    /* Estimate the number of primes needed.  For n≤10 000 a few ×10⁴ primes
     * are sufficient.  The formula n*20+100 is a generous over‑estimate. */
    idx_t limit = n * 20 + 100;
    if (limit < 200) limit = 200;

    uint32_t *primes = NULL;
    idx_t prime_cnt = 0;
    sieve_primes(limit, &primes, &prime_cnt);
    if (prime_cnt < 2) {          /* not enough primes – should not happen */
        free(primes);
        return 0;
    }

    /* Allocate a heap large enough for at most prime_cnt‑1 elements. */
    struct Node *heap = (struct Node *)malloc((size_t)prime_cnt * sizeof(struct Node));
    if (!heap) {
        free(primes);
        return 0;
    }

    idx_t heap_size = 0;

    /* Insert the first pair for every prime: (i, i+1) with i < prime_cnt-1. */
    for (idx_t i = 0; i + 1 < prime_cnt; ++i) {
        struct Node node;
        node.prod = (uint64_t)primes[i] * (uint64_t)primes[i + 1];
        node.i    = i;
        node.j    = i + 1;
        heap_push(heap, &heap_size, node);
    }

    seq_t result = (seq_t)primes[0];  /* default */
    for (idx_t k = 1; k <= n; ++k) {
        struct Node cur = heap_pop(heap, &heap_size);
        result = (seq_t)primes[cur.i];          /* smaller prime of the product */
        if (cur.j + 1 < prime_cnt) {            /* push next pair with same i */
            struct Node next;
            next.prod = (uint64_t)primes[cur.i] * (uint64_t)primes[cur.j + 1];
            next.i    = cur.i;
            next.j    = cur.j + 1;
            heap_push(heap, &heap_size, next);
        }
    }

    free(primes);
    free(heap);
    return result;
}
