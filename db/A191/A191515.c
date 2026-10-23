/*
 * A191515  Number of vertices of outdegree >=2 in the rooted tree having
 *          Matula-Goebel number n.
 * https://oeis.org/A191515
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A191515
   a(n) = number of vertices of outdegree ≥ 2 in the rooted tree
   having Matula–Goebel number n.
   Recurrence (from the OEIS entry):
        a(1) = 0
        if n = p(t) (n is the t‑th prime) then a(n) = a(t)
        if n = r·s, r,s ≥ 2 then
            a(n) = 1 + a(r) + a(s) – G(r,0) – G(s,0)
   where G(m,0) = 1 if the root of the tree with number m has
   outdegree ≥ 2 (i.e. m is composite), otherwise 0.
   The implementation below precomputes
   - the smallest prime factor (spf) of every integer up to the
     maximum n requested,
   - the index of each prime number,
   - the sequence values a(n) in increasing order.
   It works for arbitrary integer types seq_t and idx_t and
   compiles both as C11 and as C++17.  Dynamically allocated
   arrays are recreated when a larger n is requested.
   The algorithm runs in O(n log log n) time per new maximum n. */

#include <stdlib.h>
#include <stdint.h>
#ifdef __cplusplus
#include <new>
#endif

/* External types supplied by the build system */
extern struct _seq_t *alloc_cache(idx_t n);  /* ignore, only needed by the build */

/* Static state for memoisation */
static seq_t *a_cache      = NULL;
static idx_t *spf          = NULL;
static idx_t *prime_index  = NULL;
static idx_t  cache_cap    = 0;   /* largest n that has been prepared */

/* Create or resize internal tables up to N (inclusive). */
static void init_arrays(idx_t N)
{
    if (N <= cache_cap)
        return;

    /* free old memory if necessary */
    if (a_cache) {
#ifdef __cplusplus
        delete[] a_cache;
#else
        free(a_cache);
#endif
    }
    if (spf) {
#ifdef __cplusplus
        delete[] spf;
#else
        free(spf);
#endif
    }
    if (prime_index) {
#ifdef __cplusplus
        delete[] prime_index;
#else
        free(prime_index);
#endif
    }

    /* allocate new arrays initialized to zero */
    a_cache = NULL;
    spf     = NULL;
    prime_index = NULL;
#ifdef __cplusplus
    a_cache      = new seq_t[N + 1]();
    spf          = new idx_t[N + 1]();
    prime_index  = new idx_t[N + 1]();
#else
    a_cache      = (seq_t*)malloc((N + 1) * sizeof(seq_t));
    spf          = (idx_t*)malloc((N + 1) * sizeof(idx_t));
    prime_index  = (idx_t*)malloc((N + 1) * sizeof(idx_t));
    /* zero the blocks manually, because malloc may not zero */
    if (a_cache)  memset(a_cache, 0, (N + 1) * sizeof(seq_t));
    if (spf)      memset(spf, 0, (N + 1) * sizeof(idx_t));
    if (prime_index) memset(prime_index, 0, (N + 1) * sizeof(idx_t));
#endif

    if (!a_cache || !spf || !prime_index)
        return;   /* memory allocation failed – caller will see zero */

    /* smallest prime factor sieve */
    for (idx_t i = 2; i <= N; ++i) {
        if (!spf[i]) {            /* i is prime */
            spf[i] = i;
            if (i <= N / i) {     /* avoid overflow */
                for (idx_t j = i * i; j <= N; j += i)
                    if (!spf[j])
                        spf[j] = i;
            }
        }
    }

    /* compute prime indices (1‑based) */
    idx_t pcnt = 0;
    for (idx_t i = 2; i <= N; ++i) {
        if (spf[i] == i) {        /* i is prime */
            ++pcnt;
            prime_index[i] = pcnt;
        }
    }

    /* dynamic programming for the sequence */
    a_cache[1] = (seq_t)0;
    for (idx_t n = 2; n <= N; ++n) {
        if (spf[n] == n) {        /* n is prime -> n = p(t) */
            idx_t t = prime_index[n];
            a_cache[n] = a_cache[t];
        } else {                  /* composite n = r · s, r ≤ s */
            idx_t r = spf[n];
            idx_t s = n / r;
            seq_t val = (seq_t)1 + a_cache[r] + a_cache[s];
            if (spf[r] != r)  /* G(r,0) = 1 if composite */
                val -= (seq_t)1;
            if (spf[s] != s)  /* G(s,0) = 1 if composite */
                val -= (seq_t)1;
            a_cache[n] = val;
        }
    }

    cache_cap = N;
}

/* Needed public function */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;          /* out of range → 0 */

    init_arrays(n);               /* ensure tables ready */
    return a_cache[n];
}
