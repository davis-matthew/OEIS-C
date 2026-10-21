/*
 * A072910  a(n) = sigma(A023883(n)) / tau(A023883(n)).
 * https://oeis.org/A072910
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = sigma(A023883(n)) / tau(A023883(n)) where A023883 lists
 *            non‑prime arithmetic numbers (numbers m with sigma(m)/tau(m)
 *            integral). a(1)=1 is included. The routine builds A023883
 *            incrementally and caches the corresponding ratios
 * Range:     u8 n <= 11, u16 n <= 306, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* External types supplied by the build.              */
/* seq_t  – unsigned integer or bigint class          */
/* idx_t  – unsigned integer type (uint64_t)          */

static seq_t *a_vals   = NULL;   /* cached a(n) values                     */
static size_t a_size   = 0;      /* number of cached values                */
static size_t a_cap    = 0;      /* allocated capacity of a_vals           */

static idx_t spf_limit = 0;      /* current upper bound of SPF sieve         */
static idx_t *spf       = NULL;  /* array of smallest prime factors          */

static idx_t last_checked = 1;   /* last integer examined for arithmetic     */

/* Build SPF sieve up to 'limit'.  Reallocates the array if it currently
   cannot contain values up to 'limit'.  The sieve uses the classical
   O(n log log n) algorithm. */
static void build_spf(idx_t limit)
{
    if (spf_limit >= limit)
        return;

    /* allocate or reallocate */
    spf = (idx_t *)realloc(spf, (size_t)(limit + 1) * sizeof(idx_t));
    for (idx_t i = 0; i <= limit; ++i)
        spf[i] = 0;

    for (idx_t i = 2; i <= limit; ++i) {
        if (spf[i] == 0) {
            spf[i] = i;
            if (i > limit / i)
                continue;
            for (idx_t j = i * i; j <= limit; j += i)
                if (spf[j] == 0)
                    spf[j] = i;
        }
    }
    spf[1] = 1;          /* convention to keep 1 handled separately */
    spf_limit = limit;
}

/* Append a new value to the cached sequence, enlarging the array if
   necessary. */
static void cache_value(seq_t val)
{
    if (a_size == a_cap) {
        a_cap = (a_cap == 0) ? 16 : a_cap * 2;
        a_vals = (seq_t *)realloc(a_vals, a_cap * sizeof(seq_t));
    }
    a_vals[a_size++] = val;
}

/* Compute sigma(n) and tau(n) using the SPF table.  If sigma(n) is
   divisible by tau(n), set *ratio = sigma(n)/tau(n) and return true.
   Otherwise return false.                                */
static bool compute_ratio(idx_t n, seq_t *ratio)
{
    if (n == 1) {
        *ratio = (seq_t)1;
        return true;
    }

    idx_t temp = n;
    seq_t sigma = 1;
    seq_t tau   = 1;

    while (temp > 1) {
        idx_t p = spf[temp];
        idx_t e = 0;
        while (temp % p == 0) {
            temp /= p;
            ++e;
        }
        /* compute (p^(e+1)-1)/(p-1) */
        seq_t p_pow = (seq_t)1;
        for (idx_t i = 0; i < e + 1; ++i)
            p_pow *= (seq_t)p;
        seq_t numer   = p_pow - (seq_t)1;
        seq_t denom   = (seq_t)p - (seq_t)1;
        seq_t sigma_f = numer / denom;   /* exact division by construction */
        sigma *= sigma_f;
        tau   *= (seq_t)(e + 1);
    }

    if (sigma % tau != (seq_t)0)
        return false;

    *ratio = sigma / tau;
    return true;
}

/* Main sequence function.  Returns the nth value of A072910.  It builds
   the sequence on demand, tolerating calls in any order. */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;   /* undefined for n==0; behavior arbitrary */

    /* Initialisation: cache the first term (1). */
    if (a_size == 0) {
        build_spf(10);            /* small initial sieve */
        cache_value((seq_t)1);    /* A023883(1) = 1, ratio 1 */
        last_checked = 1;
    }

    /* Extend the cache until it contains the requested index. */
    while (a_size < n) {
        /* Ensure the sieve covers the next integer to test. */
        if (last_checked + 1 > spf_limit) {
            idx_t new_limit = spf_limit ? spf_limit * 2 : 64;
            if (new_limit <= last_checked + 1)
                new_limit = last_checked + 2;
            build_spf(new_limit);
        }
        ++last_checked;   /* next integer to examine */

        /* Skip primes (spf[x]==x) and the number 1 (already cached). */
        if (spf[last_checked] == last_checked || last_checked == 1)
            continue;

        seq_t ratio;
        if (compute_ratio(last_checked, &ratio))
            cache_value(ratio);
    }

    return a_vals[n - 1];
}
