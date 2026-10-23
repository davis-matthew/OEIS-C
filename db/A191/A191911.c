/*
 * A191911  Semiprimes p*q such that the concatenation p||q is again a
 *          semiprime, where p <= q.
 * https://oeis.org/A191911
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th smallest semiprime p*q (p <= q) such that the
 *            decimal concatenation p||q is also a semiprime. The function
 *            generates primes up to 1,000,000, builds a min‑heap of products
 *            p*q, and discovers the required sequence entries in increasing
 *            order
 * Range:     u8 n <= 39, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* ---------- Prime precomputation (linear sieve) ---------- */
#define SIEVE_LIMIT 1000000

static uint32_t spf[SIEVE_LIMIT + 1];   /* smallest prime factor */
static uint32_t primes[SIEVE_LIMIT + 1];/* primes in ascending order */
static size_t   prime_cnt = 0;          /* number of primes */

/* Linear sieve filling spf[] and primes[] */
static void init_sieve(void)
{
    static bool done = false;
    if (done) return;
    done = true;

    for (uint32_t i = 2; i <= SIEVE_LIMIT; ++i) {
        if (!spf[i]) {
            spf[i] = i;
            primes[prime_cnt++] = i;
        }
        for (size_t j = 0; j < prime_cnt; ++j) {
            uint32_t p = primes[j];
            uint64_t v = (uint64_t)i * p;
            if (v > SIEVE_LIMIT) break;
            if (!spf[v]) spf[v] = p;
            if (i % p == 0) break;
        }
    }
}

/* Smallest prime factor of n (n <= 10^12). Uses the primes array
   computed above. */
static uint32_t smallest_factor(uint64_t n)
{
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint32_t p = primes[i];
        uint64_t pp = (uint64_t)p * p;
        if (pp > n) break;
        if (n % p == 0) return p;
    }
    return (uint32_t)n; /* n is prime */
}

/* Returns true iff n is a semiprime (product of exactly two primes). */
static bool is_semiprime(uint64_t n)
{
    uint32_t f = smallest_factor(n);
    if (f == (uint32_t)n) return false;          /* n is prime */
    uint64_t other = n / f;
    uint32_t g = smallest_factor(other);
    return g == (uint32_t)other;                /* other is prime */
}

/* -------------------- utilities -------------------- */
static inline int digits_uint64(uint64_t v)
{
    int d = 0;
    do { ++d; v /= 10; } while (v);
    return d;
}

/* Pre‑computed powers of 10 up to 10^19. */
static const uint64_t pow10_tab[20] = {
    1ULL,
    10ULL,
    100ULL,
    1000ULL,
    10000ULL,
    100000ULL,
    1000000ULL,
    10000000ULL,
    100000000ULL,
    1000000000ULL,
    10000000000ULL,
    100000000000ULL,
    1000000000000ULL,
    10000000000000ULL,
    100000000000000ULL,
    1000000000000000ULL,
    10000000000000000ULL,
    100000000000000000ULL,
    1000000000000000000ULL
};
static inline uint64_t pow10_uint(uint32_t e)
{
    return pow10_tab[e];
}

/* Test whether the concatenation p||q is a semiprime. */
static bool concat_is_semiprime(uint32_t p, uint32_t q)
{
    uint32_t dq = (uint32_t)digits_uint64(q);
    uint64_t concat = (uint64_t)p * pow10_uint(dq) + q;
    return is_semiprime(concat);
}

/* -------------------- heap for products -------------------- */
typedef struct {
    uint64_t prod; /* p * q */
    size_t   i;    /* index of p in primes[] */
    size_t   j;    /* index of q in primes[] */
} Node;

static Node *heap_arr  = NULL;
static size_t heap_sz  = 0;
static size_t pair_cnt = 0;    /* number of primes used for pairs */

static inline void heap_swap(Node *a, Node *b)
{
    Node tmp = *a; *a = *b; *b = tmp;
}

static void heap_push(Node v)
{
    heap_arr[heap_sz++] = v;
    size_t idx = heap_sz - 1;
    while (idx > 0) {
        size_t parent = (idx - 1) >> 1;
        if (heap_arr[parent].prod <= heap_arr[idx].prod) break;
        heap_swap(&heap_arr[parent], &heap_arr[idx]);
        idx = parent;
    }
}

static Node heap_pop(void)
{
    Node ret = heap_arr[0];
    heap_arr[0] = heap_arr[--heap_sz];
    size_t idx = 0;
    while (1) {
        size_t l = idx * 2 + 1;
        if (l >= heap_sz) break;
        size_t r = l + 1;
        size_t child = (r < heap_sz && heap_arr[r].prod < heap_arr[l].prod) ? r : l;
        if (heap_arr[idx].prod <= heap_arr[child].prod) break;
        heap_swap(&heap_arr[idx], &heap_arr[child]);
        idx = child;
    }
    return ret;
}

/* -------------------- cache for sequence values -------------------- */
static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;

static void cache_grow(void)
{
    size_t new_cap = cache_cap ? cache_cap << 1 : 64;
    seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
    cache = tmp;
    cache_cap = new_cap;
}

/* -------------------- static initialization -------------------- */
static void init_static(void)
{
    static bool done = false;
    if (done) return;
    done = true;

    init_sieve();                     /* primes up to 1e6 */

    /* Choose primes up to 200000 for the pair generation. */
    const uint32_t PAIR_LIMIT = 200000;
    pair_cnt = 0;
    while (pair_cnt < prime_cnt && primes[pair_cnt] <= PAIR_LIMIT) ++pair_cnt;

    heap_arr = (Node*)malloc(pair_cnt * sizeof(Node));
    heap_sz = 0;
    for (size_t i = 0; i < pair_cnt; ++i) {
        uint64_t prod = (uint64_t)primes[i] * primes[i];
        heap_push((Node){prod, i, i});
    }

    cache_cap = 64;
    cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
    cache_len = 0;
}

/* -------------------- main sequence function -------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;
    init_static();

    while (cache_len < n) {
        Node cur = heap_pop();
        uint32_t p = primes[cur.i];
        uint32_t q = primes[cur.j];

        if (concat_is_semiprime(p, q)) {
            if (cache_len >= cache_cap) cache_grow();
            cache[cache_len++] = (seq_t)cur.prod;
        }

        if (cur.j + 1 < pair_cnt) {
            uint64_t next_prod = (uint64_t)p * primes[cur.j + 1];
            heap_push((Node){next_prod, cur.i, cur.j + 1});
        }
    }
    return cache[n - 1];
}
