/*
 * A035881  Coordination sequence for diamond structure D^+_10. (Edges defined
 *          by l_1 norm = 1.)
 * https://oeis.org/A035881
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 21, u64 n <= 253, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Coordination sequence for diamond structure D⁺₁₀ (OEIS A035881).
   The generating function is 
        P(x)/((1-x)¹⁰(1+x)¹⁰)   with
   P(x) = 1 + 190x² + 4845x⁴ + 512x⁵ + 43880x⁶ + 23040x⁷ + 187410x⁸
            +107520x⁹ +313780x¹⁰ +107520x¹¹ +187410x¹² +23040x¹³
            +43880x¹⁴ +512x¹⁵ +4845x¹⁶ +190x¹⁸ +1x²⁰.
   From (1‑x)¹⁰(1+x)¹⁰ = (1‑x²)¹⁰ = Σ (-1)^k C(10,k) x²k we obtain
   the linear recurrence   a(n)=Σ_{k=1}^{10} (-1)^{k+1} C(10,k) a(n‑2k)  for n>20,
   and the same formula with the known coefficients p(n) for n≤20:
       a(n)=p(n)+Σ_{k=1}^{min(10,⌊n/2⌋)} (-1)^{k+1} C(10,k) a(n‑2k).   */
/* Note: a(0)=1 ; a(n)=0 for all odd n. */

#include <stdlib.h>
#include <string.h>
#include <assert.h>

/* Binomial coefficients C(10,k) */
static const unsigned int binom10[11] = {
    1, 10, 45, 120, 210, 252, 210, 120, 45, 10, 1
};

/* Coefficients of P(x) for n=0..20 */
static const seq_t pcoef[21] = {
    1ULL,       0ULL,       190ULL,      0ULL,      4845ULL,
    512ULL,     43880ULL,   23040ULL,   187410ULL, 107520ULL,
    313780ULL, 107520ULL, 187410ULL,   23040ULL,   43880ULL,
    512ULL,      4845ULL,     0ULL,       190ULL,      0ULL,
    1ULL
};

/* Cache for computed terms (dynamic). */
static seq_t *cache = NULL;
static size_t cache_cap = 0;     /* number of allocated slots */
static size_t cache_len = 0;     /* next unused index (i.e. cache_len-1 is the highest stored) */

/* Ensure cache has room for indices up to n (inclusive). */
static void ensure_capacity(idx_t n)
{
    if ((size_t)n + 1 <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    if (new_cap < (size_t)n + 1)
        new_cap = (size_t)n + 1;
    seq_t *tmp = (seq_t *)malloc(new_cap * sizeof(seq_t));
    if (!tmp)
        abort();
    if (cache) {
        memcpy(tmp, cache, cache_cap * sizeof(seq_t));
        free(cache);
    }
    cache = tmp;
    cache_cap = new_cap;
}

/* Initialize first 21 terms using the non‑homogeneous recurrence. */
static void init_first_terms(void)
{
    if (cache_len > 0 && cache_len >= 21)
        return;        /* already done */

    ensure_capacity(20);
    /* compute a(0)..a(20) */
    for (idx_t n = 0; n <= 20; ++n) {
        unsigned __int128 sum = 0;
        /* add the polynomial term p(n) */
        sum += (unsigned __int128)pcoef[n];
        /* add contributions from previous terms */
        for (idx_t k = 1; k <= 10 && 2 * k <= n; ++k) {
            seq_t prev = cache[n - 2 * k];
            unsigned __int128 term = (unsigned __int128)prev * binom10[k];
            if ((k & 1) == 0)   /* k even => subtract */
                sum -= term;
            else                /* k odd  => add */
                sum += term;
        }
        cache[n] = (seq_t)sum;
    }
    cache_len = 21;
}

/* Compute a(n) for any n. */
seq_t a(idx_t n)
{
    ensure_capacity(n);
    init_first_terms();

    if (n < cache_len)
        return cache[n];

    /* compute terms from cache_len up to n using homogeneous recurrence */
    for (idx_t idx = cache_len; idx <= n; ++idx) {
        unsigned __int128 sum = 0;
        for (idx_t k = 1; k <= 10; ++k) {
            seq_t prev = cache[idx - 2 * k];
            unsigned __int128 term = (unsigned __int128)prev * binom10[k];
            if ((k & 1) == 0)   /* k even => subtract */
                sum -= term;
            else                /* k odd  => add */
                sum += term;
        }
        cache[idx] = (seq_t)sum;
    }
    cache_len = n + 1;
    return cache[n];
}
