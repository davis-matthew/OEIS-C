/*
 * A107982  Gaussian-Pythagorean semiprimes. Products of a prime of the form 2
 *          or 4n+1 (A002313) and a prime of the form 4n+3 (A002145).
 * https://oeis.org/A107982
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = p * q, where p is prime 2 or ≡1 (mod 4) and q is prime ≡3
 *            (mod 4). The sequence is sorted increasingly
 * Range:     u8 none, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* internal node for the heap */
typedef struct {
    seq_t val;
    size_t a_idx;   /* index in primes_A */
    size_t b_idx;   /* index in primes_B */
} Node;

/* static global state */
static seq_t   *terms   = NULL;
static size_t  terms_sz = 0;
static size_t  terms_cap = 0;

static uint8_t *is_prime_map = NULL;
static uint64_t sieve_limit  = 0;

static uint64_t *primes_A = NULL;
static uint64_t *primes_B = NULL;
static size_t   primes_A_sz = 0;
static size_t   primes_B_sz = 0;

static Node *heap    = NULL;
static size_t heap_sz = 0;

/* ------------------------------------------------------------------ */
/* sieve that partitions primes into the two residue classes             */
static void build_sieve(uint64_t limit)
{
    if (limit <= sieve_limit) return;             /* already sufficient */

    /* allocate map */
    uint8_t *tmp = (uint8_t *)malloc((size_t)(limit + 1));
    if (!tmp) exit(1);
    for (uint64_t i = 0; i <= limit; ++i) tmp[i] = 1;
    tmp[0] = tmp[1] = 0;

    for (uint64_t i = 2; i * i <= limit; ++i)
        if (tmp[i])
            for (uint64_t j = i * i; j <= limit; j += i)
                tmp[j] = 0;

    /* count primes in each class */
    size_t cntA = 0, cntB = 0;
    for (uint64_t i = 2; i <= limit; ++i)
        if (tmp[i]) {
            if (i == 2 || i % 4 == 1) ++cntA;
            if (i % 4 == 3) ++cntB;
        }

    /* free old prime lists */
    free(primes_A); free(primes_B);
    free(is_prime_map);

    primes_A = (uint64_t *)malloc(cntA * sizeof(uint64_t));
    primes_B = (uint64_t *)malloc(cntB * sizeof(uint64_t));
    if (!primes_A || !primes_B) exit(1);

    size_t ai = 0, bi = 0;
    for (uint64_t i = 2; i <= limit; ++i)
        if (tmp[i]) {
            if (i == 2 || i % 4 == 1) primes_A[ai++] = i;
            if (i % 4 == 3) primes_B[bi++] = i;
        }

    primes_A_sz = cntA;
    primes_B_sz = cntB;
    sieve_limit  = limit;
    is_prime_map = tmp;   /* keep map for potential future use */
}

/* ------------------------------------------------------------------ */
/* heap helpers                                                        */
static inline void heap_swap(Node *x, Node *y)
{
    Node tmp = *x; *x = *y; *y = tmp;
}

static void sift_down(size_t i)
{
    while (1) {
        size_t l = 2 * i + 1;
        size_t r = l + 1;
        size_t smallest = i;

        if (l < heap_sz && heap[l].val < heap[smallest].val) smallest = l;
        if (r < heap_sz && heap[r].val < heap[smallest].val) smallest = r;

        if (smallest == i) break;
        heap_swap(&heap[i], &heap[smallest]);
        i = smallest;
    }
}

static void sift_up(size_t i)
{
    while (i > 0) {
        size_t p = (i - 1) >> 1;
        if (heap[p].val <= heap[i].val) break;
        heap_swap(&heap[p], &heap[i]);
        i = p;
    }
}

/* ------------------------------------------------------------------ */
/* initialise heap with the first column of products                   */
static void init_heap(void)
{
    if (!heap) {
        heap = (Node *)malloc(primes_A_sz * sizeof(Node));
        if (!heap) exit(1);
    } else {
        heap = (Node *)realloc(heap, primes_A_sz * sizeof(Node));
        if (!heap) exit(1);
    }
    heap_sz = primes_A_sz;

    for (size_t i = 0; i < heap_sz; ++i) {
        heap[i].a_idx = i;
        heap[i].b_idx = 0;
        heap[i].val = ((seq_t)primes_A[i]) * ((seq_t)primes_B[0]);
    }

    for (size_t i = (heap_sz >> 1); i > 0; --i)
        sift_down(i - 1);
}

/* ------------------------------------------------------------------ */
/* fill terms array up to at least n terms                            */
static void ensure_terms(idx_t n)
{
    /* enlarge prime lists if necessary to guarantee enough pairs */
    while (primes_A_sz * primes_B_sz < (size_t)n) {
        uint64_t next_limit = sieve_limit ? sieve_limit * 2 : 10000;
        build_sieve(next_limit);
        init_heap();
    }

    while (terms_sz < (size_t)n) {
        Node cur = heap[0];
        seq_t val = cur.val;   /* extract */

        /* remove root */
        heap[0] = heap[heap_sz - 1];
        --heap_sz;
        sift_down(0);

        /* push next product for same A-prime */
        size_t nb = cur.b_idx + 1;
        if (nb < primes_B_sz) {
            cur.b_idx = nb;
            cur.val = ((seq_t)primes_A[cur.a_idx]) * ((seq_t)primes_B[nb]);
            heap[heap_sz++] = cur;
            sift_up(heap_sz - 1);
        }

        /* append to terms list */
        if (terms_sz == terms_cap) {
            terms_cap = terms_cap ? terms_cap * 2 : 16;
            seq_t *tmp = (seq_t *)realloc(terms, terms_cap * sizeof(seq_t));
            if (!tmp) exit(1);
            terms = tmp;
        }
        terms[terms_sz++] = val;
    }
}

/* ------------------------------------------------------------------ */
/* public interface                                                    */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* out of spec, return 0 */

    static bool initialized = false;
    if (!initialized) {
        initialized = true;
        terms = NULL; terms_sz = 0; terms_cap = 0;
        build_sieve(20000);                /* initial moderate sieve */
        init_heap();
    }

    ensure_terms(n);
    return terms[(size_t)n - 1];
}
