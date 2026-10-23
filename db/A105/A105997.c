/*
 * A105997  Semiprime function n -> A001358(n) applied three times to n.
 * https://oeis.org/A105997
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = A001358(A001358(A001358(n)))
 * Range:     u8 n <= 10, u16 n <= 1107, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Where A001358(k) = the k‑th semiprime (product of two primes, with multiplicity). */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* ----------  local types  ------------------------------------------ */

typedef struct {        /* node for the min‑heap of semiprime products   */
    int i;              /* index of the first prime (≤ j)      */
    int j;              /* index of the second prime (≥ i)     */
    uint64_t prod;      /* product of primes[i] * primes[j]    */
} Node;

/* ----------  global state  ------------------------------------------ */

static uint32_t *primes      = NULL;   /* array of primes up to LIMIT   */
static idx_t     primes_cnt  = 0;      /* number of primes loaded       */
static bool      primes_ready = false;

static Node  *heap       = NULL;       /* min‑heap of upcoming products */
static int    heap_size  = 0;          /* current number of heap nodes  */
static int    heap_cap   = 0;          /* capacity of heap array        */
static bool   heap_ready = false;

static seq_t *semiprimes   = NULL;      /* generated semiprimes in order */
static idx_t  semip_cnt    = 0;          /* number of semiprimes generated */
static idx_t  semip_cap    = 0;          /* capacity of the array          */

/* ----------  helper functions  ------------------------------------- */

/* simple sieve: generate all primes up to LIMIT */
static void build_primes(void)
{
    const uint32_t LIMIT = 20000000;          /* enough for n ≤ 100 000  */
    uint8_t *is_prime = (uint8_t *)calloc(LIMIT + 1, sizeof(uint8_t));
    if (!is_prime) exit(1);

    is_prime[0] = is_prime[1] = 1;
    for (uint32_t i = 2; (uint64_t)i * i <= LIMIT; ++i)
        if (!is_prime[i])
            for (uint32_t j = i * i; j <= LIMIT; j += i)
                is_prime[j] = 1;

    /* count primes */
    primes_cnt = 0;
    for (uint32_t i = 2; i <= LIMIT; ++i)
        if (!is_prime[i]) ++primes_cnt;

    primes = (uint32_t *)malloc(primes_cnt * sizeof(uint32_t));
    if (!primes) exit(1);

    uint32_t k = 0;
    for (uint32_t i = 2; i <= LIMIT; ++i)
        if (!is_prime[i]) primes[k++] = i;

    free(is_prime);
    primes_ready = true;
}

/* heap operations (min‑heap on prod) */

static inline void heap_swap(int a, int b)
{
    Node tmp = heap[a];
    heap[a] = heap[b];
    heap[b] = tmp;
}

static void heap_bubble_up(int idx)
{
    while (idx > 0) {
        int parent = (idx - 1) >> 1;
        if (heap[parent].prod <= heap[idx].prod) break;
        heap_swap(parent, idx);
        idx = parent;
    }
}

static void heap_bubble_down(int idx)
{
    while (true) {
        int left  = (idx << 1) + 1;
        int right = left + 1;
        int smallest = idx;

        if (left  < heap_size && heap[left].prod  < heap[smallest].prod) smallest = left;
        if (right < heap_size && heap[right].prod < heap[smallest].prod) smallest = right;

        if (smallest == idx) break;
        heap_swap(idx, smallest);
        idx = smallest;
    }
}

static void heap_push(Node nd)
{
    if (heap_size == heap_cap) {
        heap_cap = (heap_cap ? heap_cap << 1 : 8);
        heap = (Node *)realloc(heap, heap_cap * sizeof(Node));
        if (!heap) exit(1);
    }
    heap[heap_size++] = nd;
    heap_bubble_up(heap_size - 1);
}

static Node heap_pop(void)
{
    Node top = heap[0];
    heap[0] = heap[--heap_size];
    if (heap_size > 0)
        heap_bubble_down(0);
    return top;
}

/* initialise the heap with the diagonal products p[i]*p[i] */
static void init_heap(void)
{
    heap_cap = primes_cnt;
    heap     = (Node *)malloc(heap_cap * sizeof(Node));
    if (!heap) exit(1);

    for (int i = 0; i < (int)primes_cnt; ++i) {
        heap[i].i    = i;
        heap[i].j    = i;
        heap[i].prod = (uint64_t)primes[i] * primes[i];
    }
    heap_size = (int)primes_cnt;

    for (int idx = (heap_size >> 1) - 1; idx >= 0; --idx)
        heap_bubble_down(idx);

    heap_ready = true;
}

/* generate one more semiprime and append it to the list */
static void generate_next_semiprime(void)
{
    Node cur = heap_pop();                    /* smallest pending product */

    if (semip_cnt == semip_cap) {
        semip_cap = (semip_cap ? semip_cap << 1 : 8192);
        semiprimes = (seq_t *)realloc(semiprimes, semip_cap * sizeof(seq_t));
        if (!semiprimes) exit(1);
    }

    semiprimes[semip_cnt++] = (seq_t)cur.prod;

    /* push the next product with the same first prime */
    int i = cur.i;
    int j = cur.j + 1;
    if (j < (int)primes_cnt) {
        Node nxt;
        nxt.i    = i;
        nxt.j    = j;
        nxt.prod = (uint64_t)primes[i] * primes[j];
        heap_push(nxt);
    }
}

/* return the nth semiprime (1‑based).  If n > current size, generate more. */
static seq_t nth_semiprime(idx_t n)
{
    if (n == 0) return 0;            /* safety */

    if (semip_cnt >= n) return semiprimes[n - 1];

    if (!primes_ready) build_primes();
    if (!heap_ready) init_heap();

    while (semip_cnt < n) generate_next_semiprime();

    return semiprimes[n - 1];
}

/* ----------  public routine  ---------------------------------------- */

seq_t a(idx_t n)
{
    seq_t s1 = nth_semiprime(n);
    seq_t s2 = nth_semiprime((idx_t)s1);
    seq_t s3 = nth_semiprime((idx_t)s2);
    return s3;
}
