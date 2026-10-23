/*
 * A121572  Subprimorials: inverse binomial transform of primorials (A002110).
 * https://oeis.org/A121572
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=0..n} (-1)^(n-k) * C(n,k) * Prime(k)#, with
 *            Prime(0)# = 1
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 9, u64 n <= 15, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* ---------- cached values ---------- */
static seq_t *a_cache     = NULL;   /* a(n) values */
static seq_t *prim_cache  = NULL;   /* primorials p_n# */
static uint64_t *prime_list = NULL; /* list of first primes */

/* ---------- cache sizes and state ---------- */
static size_t a_cap      = 0;
static size_t prim_cap   = 0;
static size_t prime_cap  = 0;
static idx_t  a_computed_n = 0;     /* largest n stored in a_cache   */
static size_t prime_count  = 0;     /* number of primes known */

/* ---------- build primes up to a given count ---------- */
static void ensure_primes_count(idx_t n)
{
    if (prime_count >= (size_t)n) return;

    uint64_t candidate = 2;
    if (prime_count > 0) candidate = prime_list[prime_count - 1] + 1;

    while (prime_count < (size_t)n)
    {
        bool is_prime = true;
        for (size_t i = 0; i < prime_count; ++i)
        {
            uint64_t p = prime_list[i];
            if (p * p > candidate) break;
            if (candidate % p == 0) { is_prime = false; break; }
        }
        if (is_prime) {
            if (prime_cap == prime_count) {
                size_t new_cap = prime_cap ? prime_cap * 2 : 16;
                uint64_t *new_ptr = (uint64_t *)realloc(prime_list,
                                                        new_cap * sizeof(uint64_t));
                prime_list = new_ptr;
                prime_cap  = new_cap;
            }
            prime_list[prime_count++] = candidate;
        }
        ++candidate;
    }
}

/* ---------- expand caches to at least n ---------- */
static void expand_to_n(idx_t n)
{
    /* initialise caches if first call */
    if (a_cache == NULL) {
        a_cache = (seq_t *)malloc(sizeof(seq_t));
        prim_cache = (seq_t *)malloc(sizeof(seq_t));
        a_cap = 1;
        prim_cap = 1;
        a_cache[0] = (seq_t)1;        /* a(0) = 1 */
        prim_cache[0] = (seq_t)1;     /* 0# = 1 */
        a_computed_n = 0;
    }
    if (n <= a_computed_n) return;

    /* enlarge caches if necessary */
    if ((size_t)n + 1 > a_cap) {
        size_t new_cap = a_cap ? a_cap * 2 : 16;
        while (new_cap <= (size_t)n + 1) new_cap *= 2;
        seq_t *new_a    = (seq_t *)realloc(a_cache,    new_cap * sizeof(seq_t));
        seq_t *new_prim = (seq_t *)realloc(prim_cache, new_cap * sizeof(seq_t));
        a_cache  = new_a;
        prim_cache = new_prim;
        a_cap = new_cap;
        prim_cap = new_cap;
    }

    /* ensure enough primes for subsequent primorials */
    ensure_primes_count(n);

    /* compute missing terms */
    for (idx_t k = a_computed_n + 1; k <= n; ++k)
    {
        /* prim(k) = prim(k-1) * prime(k-1) */
        prim_cache[k] = prim_cache[k-1] * (seq_t)prime_list[k-1];

        /* a(k) = prim(k) - Σ_{i=0}^{k-1} C(k,i) * a(i) */
        seq_t sum   = (seq_t)0;
        seq_t coeff = (seq_t)1; /* C(k,0) */
        for (idx_t i = 0; i < k; ++i) {
            sum += coeff * a_cache[i];
            coeff = coeff * (seq_t)(k - i) / (seq_t)(i + 1);
        }
        a_cache[k] = prim_cache[k] - sum;
    }
    a_computed_n = n;
}

/* ---------- public function ---------- */
seq_t a(idx_t n)
{
    expand_to_n(n);
    return a_cache[n];
}
