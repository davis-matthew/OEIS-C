/*
 * A193404  Number of matchings (independent edge subsets) in the rooted tree
 *          with Matula-Goebel number n.
 * https://oeis.org/A193404
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of matchings in the rooted tree with Matula–Goebel
 *            number n. Recurrence for the pair (b(n),c(n)): A(1) = (0,1); if
 *            n is prime with index t: b(n)=c(t), c(n)=b(t)+c(t); if n = r*s
 *            (r,s ≥ 2): b(n)=b(r)*c(s)+c(r)*b(s), c(n)=c(r)*c(s); The final
 *            value is a(n)=b(n)+c(n)
 * Range:     u8 n <= 71, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>

/*--- Define allocation macros for C / C++ ----------*/
#ifdef __cplusplus
#define ALLOC(type, cnt) (new type[(cnt)])
#define FREE(p)          delete[] (p)
#else
#define ALLOC(type, cnt) ((type*)malloc(sizeof(type)*(cnt)))
#define FREE(p)          free(p)
#endif

/*--- Prime handling --------------------------------*/
static bool *sieve = NULL;           /* sieve[i]=true iff i is prime */
static size_t sieve_size = 0;        /* maximum index sieved (inclusive) */
static uint32_t *prime_list = NULL;  /* array of primes */
static size_t prime_count = 0;       /* count of primes in prime_list */

/* Build sieve up to ‘limit’ (inclusive) and rebuild prime_list */
static void ensure_sieve(size_t limit)
{
    if (limit <= sieve_size) return;

    size_t size = limit + 1;
    bool *new_sieve = (bool*)calloc(size, 1);
    if (!new_sieve) abort();
    for (size_t i = 2; i < size; ++i) new_sieve[i] = true;
    for (size_t i = 2; i * i < size; ++i)
        if (new_sieve[i])
            for (size_t j = i * i; j < size; j += i)
                new_sieve[j] = false;

    size_t cnt = 0;
    for (size_t i = 2; i <= limit; ++i)
        if (new_sieve[i]) ++cnt;

    uint32_t *new_primes = cnt ? ALLOC(uint32_t, cnt) : NULL;
    size_t idx = 0;
    for (size_t i = 2; i <= limit; ++i)
        if (new_sieve[i]) new_primes[idx++] = (uint32_t)i;

    FREE(prime_list);
    prime_list = new_primes;
    prime_count = cnt;

    FREE(sieve);
    sieve = new_sieve;
    sieve_size = limit;
}

/* Return 1‑based index of prime n (assumes n is prime) */
static idx_t prime_index(idx_t n)
{
    ensure_sieve((size_t)n);
    size_t lo = 0, hi = prime_count;
    while (lo < hi) {
        size_t mid = (lo + hi) / 2;
        if ((idx_t)prime_list[mid] < n) lo = mid + 1;
        else hi = mid;
    }
    return (idx_t)(lo + 1); /* 1‑based */
}

/*--- Smallest prime factor --------------------------------*/
static uint32_t smallest_prime_factor_uint64(uint64_t n)
{
    if (n % 2 == 0) return 2;
    for (uint64_t d = 3; d * d <= n; d += 2)
        if (n % d == 0) return (uint32_t)d;
    return (uint32_t)n;           /* n is prime */
}

/*--- Prime test --------------------------------*/
static bool is_prime_uint64(uint64_t n)
{
    if (n < 2) return false;
    if (n % 2 == 0) return n == 2;
    for (uint64_t d = 3; d * d <= n; d += 2)
        if (n % d == 0) return false;
    return true;
}

/*--- Caching for pair (b, c) --------------------------------*/
static seq_t *b_cache = NULL;
static seq_t *c_cache = NULL;
static bool *computed = NULL;
static size_t cache_cap = 0;   /* number of slots allocated (>=1-index) */

/* Allocate or grow arrays to accommodate index n */
static void ensure_cache(idx_t n)
{
    if ((size_t)n < cache_cap) return;

    size_t new_cap = cache_cap ? cache_cap * 2 : 1;
    while (new_cap <= (size_t)n) new_cap <<= 1;

    seq_t *new_b = ALLOC(seq_t, new_cap);
    seq_t *new_c = ALLOC(seq_t, new_cap);
    bool *new_comp = ALLOC(bool, new_cap);

    /* Copy old values */
    for (size_t i = 0; i < cache_cap; ++i) {
        new_b[i] = b_cache[i];
        new_c[i] = c_cache[i];
        new_comp[i] = computed[i];
    }
    /* Initialize new computed flags to false */
    for (size_t i = cache_cap; i < new_cap; ++i) new_comp[i] = false;

    /* Replace old arrays */
    FREE(b_cache);
    FREE(c_cache);
    FREE(computed);
    b_cache = new_b;
    c_cache = new_c;
    computed = new_comp;
    cache_cap = new_cap;
}

/* Recursive computation of pair (b, c) */
static void compute_pair(idx_t n)
{
    ensure_cache(n);
    if (computed[n]) return;
    computed[n] = true;

    if (n == 1) {
        b_cache[1] = 0;
        c_cache[1] = 1;
        return;
    }

    if (is_prime_uint64((uint64_t)n)) {
        idx_t t = prime_index(n);
        compute_pair(t);
        b_cache[n] = c_cache[t];
        c_cache[n] = b_cache[t] + c_cache[t];
        return;
    }

    /* Composite case */
    uint32_t r = smallest_prime_factor_uint64((uint64_t)n);
    idx_t s = n / r;
    compute_pair(r);
    compute_pair(s);
    b_cache[n] = b_cache[r] * c_cache[s] + c_cache[r] * b_cache[s];
    c_cache[n] = c_cache[r] * c_cache[s];
}

/*=====================================================*/
/* External function required by the framework          */
seq_t a(idx_t n)
{
    ensure_cache(n);
    compute_pair(n);
    return b_cache[n] + c_cache[n];
}
