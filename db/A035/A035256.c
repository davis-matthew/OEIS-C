/*
 * A035256  Positive integers of the form x^2+3xy-y^2.
 * https://oeis.org/A035256
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A035256: Positive integers representable by the
   indefinite binary quadratic form x² + 3xy – y²
   (discriminant 13).  An integer n is representable iff
   every prime factor p ≠ 13 for which (13/p) = –1 appears with
   an even exponent.  Prime 13 is ramified and may occur with
   arbitrary exponent. */


#include <stdlib.h>
#include <stdbool.h>

/* ---------- Least‑prime‐factor sieve ------------------------------------- */
static idx_t *spf = NULL;      /* least prime factor for every integer ≥2 */
static idx_t  spf_cap = 0;     /* current size of the sieve */

/* Ensure that the sieve covers all numbers ≤ limit.  We rebuild the
   sieve from scratch each time it is enlarged; this keeps the code
   simple and guarantees that spf[x] is non‑zero for all 2 ≤ x ≤ limit. */
static void
prepare_sieve(idx_t limit)
{
    if (limit <= spf_cap)
        return;

    /* Grow the sieve to at least `limit`.  If the new limit is
       much larger than the old one, we take twice the old size +
       a safety margin to reduce reallocations. */
    if (limit <= spf_cap * 2)
        limit = spf_cap * 2 + 10;

    idx_t *new_spf = (idx_t *)calloc((size_t)(limit + 1), sizeof(idx_t));
    if (!new_spf)
        exit(EXIT_FAILURE);

    for (idx_t i = 2; i <= limit; ++i) {
        if (!new_spf[i]) {                 /* `i` is prime */
            new_spf[i] = i;
            if (i <= limit / i) {
                for (idx_t j = i * i; j <= limit; j += i) {
                    if (!new_spf[j])
                        new_spf[j] = i;
                }
            }
        }
    }

    free(spf);
    spf      = new_spf;
    spf_cap  = limit;
}

/* ---------- Test whether a positive integer can be written as
               x² + 3xy – y². --------------------------------------------- */
static bool
is_representable(idx_t val)
{
    if (val == 0)
        return false;          /* 0 is not positive */
    if (val == 1)
        return true;           /* 1 = 1² + 3·1·0 – 0² */

    /* Ensure the sieve is large enough to factor `val`. */
    if (val > spf_cap)
        prepare_sieve(val);

    idx_t x = val;
    while (x > 1) {
        idx_t p = spf[x];
        if (p == 0)          /* should not happen, but keep safe */
            p = x;

        /* Count whether the exponent of `p` in `x` is odd or even. */
        int parity = 0;
        while (x % p == 0) {
            x /= p;
            parity ^= 1;          /* toggle parity */
        }

        /* Prime 13 is ramified: no restriction on its exponent. */
        if (p == 13)
            continue;

        int r = p % 13;           /* compute p mod 13 */
        if (r == 0)               /* ignoring the (p==13) case above */
            continue;

        /* Non‑residue primes mod 13: 2,5,6,7,8,11.
           Their exponents must be even (parity == 0). */
        if (r == 2 || r == 5 || r == 6 ||
            r == 7 || r == 8 || r == 11) {
            if (parity)           /* odd exponent → not representable */
                return false;
        }
        /* For residue primes (1,3,4,9,10,12) there is no restriction. */
    }
    return true;
}

/* ---------- Cache of computed sequence values --------------------------- */
static seq_t *term_cache = NULL;   /* array of stored terms          */
static size_t cache_size   = 0;     /* number of terms stored         */
static size_t cache_cap    = 0;     /* allocated capacity             */
static idx_t  next_check   = 1;     /* next integer to test           */

/* Grow the term cache to at least `new_cap` elements. */
static void
grow_cache(size_t new_cap)
{
    seq_t *new_cache = (seq_t *)realloc(term_cache,
                                        new_cap * sizeof(seq_t));
    if (!new_cache)
        exit(EXIT_FAILURE);
    term_cache = new_cache;
    cache_cap  = new_cap;
}

/* ---------- Main public routine ----------------------------------------- */
seq_t a(idx_t n)
{
    /* Build terms until the requested index is available. */
    while (cache_size < n) {
        /* Ensure the sieve covers the candidate integer. */
        if (next_check > spf_cap)
            prepare_sieve(next_check);

        /* Test representability of `next_check`.  If positive,
           store it in the cache. */
        if (is_representable(next_check)) {
            if (cache_size == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap << 1 : 16;
                grow_cache(new_cap);
            }
            term_cache[cache_size] = (seq_t)next_check;
            ++cache_size;
        }
        ++next_check;
    }
    return term_cache[n - 1];
}
