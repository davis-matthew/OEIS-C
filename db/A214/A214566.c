/*
 * A214566  Number of star-vertices in the rooted tree with Matula-Goebel
 *          number n.
 * https://oeis.org/A214566
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the number of star-vertices in the rooted tree with *
 *            Matula-Goebel number n. The value is computed recursively: *
 *            a(1) = 0 * for n > 1: * a(n) = (all children are leaves) ? 1 : 0
 *            + Σ a(child) * where the children of n correspond to the prime
 *            factorisation of n. * Each prime factor p give...
 * Range:     u8 n <= 85, u16 n <= 85, u32 n <= 85, u64 n <= 85, u128 n <= 85,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

/* external types supplied by the build system */
seq_t a(idx_t n);  /* declaration only, defined below */

/* ---------- Prime sieve ----------------------------------------- */
static idx_t *prime_list   = NULL;  /* primes in ascending order   */
static size_t prime_count  = 0;     /* number of primes            */
static idx_t  prime_limit  = 0;     /* maximum prime considered    */

/* Expands the sieve so that all primes ≤ limit are known. */
static void ensure_primes_upto(idx_t limit)
{
    if (limit <= prime_limit) return;

    /* Allocate boolean array for primality marking. */
    size_t sz = (size_t)(limit + 1);
    unsigned char *is_prime = (unsigned char *)calloc(sz, 1);
    if (!is_prime) return;          /* out of memory – silent failure */

    for (idx_t i = 2; i <= limit; ++i) is_prime[i] = 1;
    for (idx_t i = 2; i * i <= limit; ++i)
        if (is_prime[i])
            for (idx_t j = i * i; j <= limit; j += i)
                is_prime[j] = 0;

    /* Count primes and copy them into prime_list. */
    size_t cnt = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (is_prime[i]) ++cnt;

    idx_t *new_list = (idx_t *)malloc(cnt * sizeof(idx_t));
    if (!new_list) { free(is_prime); return; }

    size_t pos = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (is_prime[i]) new_list[pos++] = i;

    /* Replace old list. */
    free(prime_list);
    prime_list = new_list;
    prime_count = cnt;
    prime_limit = limit;

    free(is_prime);
}

/* Binary search for the index (1‑based) of a prime in prime_list. */
static idx_t prime_index_of(idx_t p)
{
    size_t lo = 0, hi = prime_count;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        if (prime_list[mid] == p) return (idx_t)(mid + 1);
        if (prime_list[mid] < p)  lo = mid + 1;
        else                      hi = mid;
    }
    /* Should never happen – all factors are primes known to the sieve. */
    return 0;
}

/* ---------- Memoisation cache ----------------------------------- */
static seq_t  *cache_val   = NULL; /* results */
static unsigned char *cache_flag = NULL; /* 0=unset,1=set */
static idx_t cache_cap = 0;       /* current capacity */

/* Ensures the cache can store values up to index n. */
static void ensure_cache(idx_t n)
{
    if (cache_cap > n) return;
    size_t new_cap = (size_t)(n + 1);
    seq_t  *new_val  = (seq_t *)realloc(cache_val, new_cap * sizeof(seq_t));
    unsigned char *new_flag =
        (unsigned char *)realloc(cache_flag, new_cap * sizeof(unsigned char));
    if (!new_val || !new_flag) return; /* out of memory – silent failure */

    /* Initialise new slots to 0. */
    for (size_t i = cache_cap; i < new_cap; ++i) new_flag[i] = 0;
    cache_val   = new_val;
    cache_flag  = new_flag;
    cache_cap   = (idx_t)new_cap;
}

/* ---------- Recursive computation ------------------------------ */
static seq_t a_rec(idx_t n)
{
    if (n == 1) return 0;

    if (cache_flag[n]) return cache_val[n];

    /* Factorise n using the prime list. */
    idx_t tmp = n;
    seq_t total = 0;
    bool all_leaf = true;

    /* Ensure primes up to sqrt(tmp) – we can simply use prime_limit >= n
     * because we will expand the sieve up to n in the public function. */
    for (size_t pi = 0; pi < prime_count &&
                (unsigned __int128)prime_list[pi] * prime_list[pi] <= tmp; ++pi)
    {
        idx_t p = prime_list[pi];
        if (tmp % p == 0) {
            idx_t child_idx = (idx_t)(pi + 1); /* prime's 1‑based index */
            do {
                tmp /= p;
                seq_t child_val = a_rec(child_idx);
                total += child_val;
                if (child_idx != 1) all_leaf = false;
            } while (tmp % p == 0);
        }
    }

    /* A remaining prime factor. */
    if (tmp > 1) {
        idx_t child_idx = prime_index_of(tmp);
        seq_t child_val = a_rec(child_idx);
        total += child_val;
        if (child_idx != 1) all_leaf = false;
    }

    if (all_leaf) total += 1;        /* current node is a star-vertex */

    cache_val[n] = total;
    cache_flag[n] = 1;
    return total;
}

/* ---------- Public interface ----------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;              /* out of domain but safe */

    ensure_cache(n);
    ensure_primes_upto(n);             /* need primes up to n */
    return a_rec(n);
}
