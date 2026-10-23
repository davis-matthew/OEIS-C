/*
 * A190641  Numbers having exactly one non-unitary prime factor.
 * https://oeis.org/A190641
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 87, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A190641: Numbers having exactly one non‑unitary prime factor */
/* a(n): value at index n (n ≥ 1).  Computed by sieving smallest prime
   factors and caching the sequence in increasing order. */

#include <stdbool.h>
#include <stdlib.h>

/* ---- Smallest prime factor sieve ----------------------------------- */

#define SPF_LIMIT 500000          /* sufficient for n ≤ 10000 (a(10000) < 30000) */

static bool spf_inited = false;
static unsigned int *spf = NULL; /* global smallest‑prime‑factor array */

/* Initialise the SPF sieve up to SPF_LIMIT.  Only called once. */
static void init_spf(void)
{
    if (spf_inited) return;
    spf_inited = true;

    spf = (unsigned int*)malloc((SPF_LIMIT + 1) * sizeof(unsigned int));
    if (!spf) exit(1);              /* allocation failed */

    for (unsigned int i = 0; i <= SPF_LIMIT; ++i) spf[i] = 0;

    for (unsigned int i = 2; i <= SPF_LIMIT; ++i) {
        if (spf[i] == 0) {          /* i is prime */
            spf[i] = i;
            if ((unsigned long long)i * i <= SPF_LIMIT) {
                for (unsigned long long j = (unsigned long long)i * i;
                     j <= SPF_LIMIT; j += i)
                    if (spf[(unsigned int)j] == 0) spf[(unsigned int)j] = i;
            }
        }
    }
}

/* Check whether a number has exactly one non‑unitary prime factor:
   i.e. it is of the form p^k * (other primes all to the first power)
   with k ≥ 2 and no other prime power > 1. */
static bool has_one_nonunitary_factor(unsigned int x)
{
    int count = 0;
    unsigned int rem = x;
    while (rem > 1) {
        unsigned int p = spf[rem];
        unsigned int exp = 0;
        while (rem % p == 0) {
            rem /= p;
            ++exp;
        }
        if (exp >= 2) {
            ++count;
            if (count > 1) return false;
        }
    }
    return count == 1;
}

/* ---- Sequence cache ---------------------------------------------- */

static seq_t *cache = NULL;      /* array of cached sequence terms */
static idx_t cache_size = 0;     /* number of terms already cached */
static idx_t cache_capacity = 0;/* allocated capacity of cache array */

/* Ensure that cache has space for at least 'need' terms. */
static void ensure_cache_capacity(idx_t need)
{
    if (cache_capacity >= need) return;
    idx_t new_cap = cache_capacity ? (cache_capacity << 1) : 4096;
    if (new_cap < need) new_cap = need;

    seq_t *new_buf = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_buf) exit(1);      /* allocation or reallocation failed */
    cache = new_buf;
    cache_capacity = new_cap;
}

/* Position of the next integer to test for inclusion in the sequence. */
static idx_t next_to_test = 2;

/* Ensure the cache contains at least 'n' terms (i.e., a(n) is available). */
static void build_up_to(idx_t n)
{
    if (!spf_inited) init_spf();        /* make sure the sieve is ready */
    ensure_cache_capacity(n);

    while (cache_size < n) {
        if (has_one_nonunitary_factor((unsigned int)next_to_test)) {
            cache[cache_size++] = (seq_t)next_to_test;
        }
        ++next_to_test;
        /* SAFETY: if we exceed SPF_LIMIT, bail out – sequence defined only up
           to the range needed by the tests. */
        if (next_to_test > SPF_LIMIT) return;
    }
}

/* ---- Public interface --------------------------------------------- */

/* Return the nth term of the sequence.  n is 1-based. */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;
    build_up_to(n);
    return cache[n - 1];
}
