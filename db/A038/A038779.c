/*
 * A038779  An intermediate sequence for nonisomorphic circulant directed
 *          p^2-graphs, indexed by odd primes p.
 * https://oeis.org/A038779
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 5, u64 n <= 10, u128 n <= 15,
 *            big any n
 * Helpers:   build_primes_upto, factor_u64, phi, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A038779
    Formula: a(n) = (1/(p-1)) * Σ_{d | (p-1)} φ(d) * 2^(2*(p-1)/d),
    where p is the nth odd prime (p=3,5,7,11,…).  This function
    accepts the index n (≥1) and returns a(n) in type seq_t.  The
    implementation caches primes and computed values; it uses only
    integer arithmetic and the helper routines supplied by the OEIS
    build environment. */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

/* Helper function headers -------------------------------------------------*/
#include "../utils/phi.h"                 /* seq_t phi(idx_t m); */
#include "../utils/pow2.h"                /* seq_t pow2(idx_t exp); */
#include "../utils/factor_u64.h"          /* void factor_u64(uint64_t n, uint64_t *pr, uint64_t *ex, size_t *cnt); */
#include "../utils/build_primes_upto.h"   /* void build_primes_upto(idx_t limit, uint64_t **primes_out, size_t *cnt_out); */

/*  Cached data -----------------------------------------------------------*/
static idx_t    *prime_list = NULL;  /* primes[0] = 2, primes[1] = 3, ...   */
static size_t    prime_count = 0;    /* number of primes stored            */
static seq_t   *value_cache = NULL; /* cache[0] unused; cache[n] holds a(n) */
static size_t   cache_size  = 0;    /* allocated size of value_cache        */

/*  Ensure at least n+1 primes in prime_list (including 2) ----------------*/
static void ensure_primes(idx_t n)
{
    if (prime_count > (size_t)n)
        return; /* already have enough */

    idx_t limit = (n < 6) ? 200 : n * 20; /* rough upper bound on p_n */

    while (prime_count <= (size_t)n) {
        uint64_t *new_primes = NULL;
        size_t    new_cnt    = 0;

        /* build_primes_upto allocates a uint64_t array of primes ≤ limit  */
        build_primes_upto(limit, &new_primes, &new_cnt);

        /* Replace old list */
        if (prime_list != NULL)
            free(prime_list);
        prime_list  = (idx_t *)new_primes;
        prime_count = new_cnt;

        /* If still not enough, increase the bound */
        if (prime_count <= (size_t)n)
            limit *= 2;
    }
}

/*  Recursive generation of all divisors of a number from its factorisation */
static void gen_divisors(const uint64_t *pr, const uint64_t *ex, size_t fcnt,
                         size_t idx, uint64_t cur, uint64_t *out, size_t *out_cnt)
{
    if (idx == fcnt) {
        out[(*out_cnt)++] = cur;
        return;
    }

    uint64_t p   = pr[idx];
    uint64_t mul = 1;
    for (uint64_t e = 0; e <= ex[idx]; ++e) {
        gen_divisors(pr, ex, fcnt, idx + 1, cur * mul, out, out_cnt);
        mul *= p;
    }
}

/*  Main sequence function -------------------------------------------------*/
seq_t a(idx_t n)
{
    if (n == 0)           /* undefined for n=0, return 0 */
        return (seq_t)0;

    /* Expand cache if necessary */
    if (n >= cache_size) {
        size_t new_sz = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(value_cache, new_sz * sizeof(seq_t));
        if (!tmp)   /* allocation failure */
            return (seq_t)0;

        /* Initialise newly allocated slots to zero */
        for (size_t i = cache_size; i < new_sz; ++i)
            tmp[i] = (seq_t)0;

        value_cache = tmp;
        cache_size  = new_sz;
    }

    if (value_cache[n] != (seq_t)0)
        return value_cache[n];    /* already computed */

    /* --- compute a(n) ----------------------------------------------- */
    ensure_primes(n);                 /* need at least the n‑th odd prime */

    /* nth odd prime: primes[0] = 2, primes[1] = 3, ... */
    uint64_t p          = (uint64_t)prime_list[n];
    uint64_t p_minus1   = p - 1;

    /* Factor p-1 to obtain its divisors */
    uint64_t fac_pr[30];
    uint64_t fac_ex[30];
    size_t   fac_cnt = 0;
    factor_u64(p_minus1, fac_pr, fac_ex, &fac_cnt);

    /* Enumerate all divisors of p-1 */
    uint64_t divisors[2000];
    size_t   div_cnt = 0;
    gen_divisors(fac_pr, fac_ex, fac_cnt, 0, 1, divisors, &div_cnt);

    /* Sum over divisors */
    seq_t sum = (seq_t)0;
    for (size_t i = 0; i < div_cnt; ++i) {
        uint64_t r   = divisors[i];
        seq_t    phi_r   = phi((idx_t)r);
        idx_t    exp     = (idx_t)((2ULL * p_minus1) / r);
        seq_t    pow2_exp= pow2(exp);
        sum += phi_r * pow2_exp;
    }

    seq_t result = sum / (seq_t)p_minus1;  /* divide by (p-1) */
    value_cache[n] = result;
    return result;
}
