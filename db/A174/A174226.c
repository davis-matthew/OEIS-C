/*
 * A174226  Partial sums of A034897.
 * https://oeis.org/A174226
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{i=1..n} h_i where h_i is the i-th hyperperfect
 *            number (numbers m such that ∃k>0: k·σ(m) = (k+1)m + k – 1). The
 *            sequence is the partial sums of the hyperperfect numbers (OEIS
 *            A034897)
 * Range:     u8 n <= 3, u16 n <= 14, u32 n <= 33, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* --- Static state shared among calls ----------------------------------- */
static seq_t *partial_sums   = NULL;   /* a(1)..a(n) as they are found          */
static size_t   partial_cap   = 0;      /* allocated size for partial_sums        */
static idx_t    counted       = 0;      /* how many hyperperfect numbers found   */
static idx_t    processed_m   = 0;      /* last m that was examined              */
static idx_t    sieve_limit   = 0;      /* size of current spf array              */
static uint32_t *spf          = NULL;   /* smallest prime factor array            */

/* Ensure capacity for partial_sums up to new_cap entries                 */
static void ensure_partial_cap(size_t new_cap)
{
    if (partial_cap >= new_cap) return;
    seq_t *tmp = (seq_t*)malloc(new_cap * sizeof(seq_t));
    if (!tmp) abort(); /* cannot happen on sane systems */
    if (partial_sums)
    {
        memcpy(tmp, partial_sums, partial_cap * sizeof(seq_t));
        free(partial_sums);
    }
    partial_sums = tmp;
    partial_cap  = new_cap;
}

/* Build spf array up to "limit" (inclusive).  Replaces any existing array. */
static void build_spf_custom(idx_t limit)
{
    if (sieve_limit > 0) free(spf);
    spf = (uint32_t*)malloc((limit + 1) * sizeof(uint32_t));
    if (!spf) abort();
    memset(spf, 0, (limit + 1) * sizeof(uint32_t));
    sieve_limit = limit;

    if (limit >= 2) spf[0] = spf[1] = 0;  /* not used */

    /* Mark evens */
    for (idx_t i = 4; i <= limit; i += 2) spf[i] = 2;

    /* Process odd numbers */
    for (idx_t i = 3; i * i <= limit; i += 2)
    {
        if (spf[i] == 0)                         /* i is prime   */
        {
            spf[i] = (uint32_t)i;
            for (idx_t j = i * i; j <= limit; j += i << 1)
                if (spf[j] == 0) spf[j] = (uint32_t)i;
        }
    }

    /* Remaining odds that are primes */
    for (idx_t i = 3; i <= limit; i += 2)
        if (spf[i] == 0) spf[i] = (uint32_t)i;
}

/* Compute sigma(m) using the spf array.  Works for all supported seq_t. */
static seq_t sigma_using_spf(uint32_t m)
{
    seq_t result = 1;
    uint32_t n = m;
    while (n > 1)
    {
        uint32_t p = spf[n];
        if (p == 0) p = n;          /* safety: should not happen */
        uint32_t exp = 0;
        while (n % p == 0) { n /= p; ++exp; }

        seq_t pe1 = 1;
        for (uint32_t i = 0; i < exp + 1; ++i) pe1 *= (seq_t)p;
        seq_t part = (pe1 - 1) / ((seq_t)p - 1);
        result *= part;
    }
    return result;
}

/* Main function: returns a(n) for any n >= 1 -------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined for offset 1, but guard */

    ensure_partial_cap((size_t)n);

    /* Ensure we have a working sieve at the start. */
    if (sieve_limit == 0)
        build_spf_custom(200000);   /* start with a reasonable limit */

    /* Keep searching until we have found n hyper‑perfect numbers. */
    while (counted < n)
    {
        /* Process numbers up to the current sieve limit. */
        while (processed_m < sieve_limit && counted < n)
        {
            ++processed_m;
            uint32_t m = (uint32_t)processed_m;
            seq_t sigma_m = sigma_using_spf(m);
            if (sigma_m > (seq_t)m + 1)   /* ensures dm > 0 */
            {
                seq_t dm = sigma_m - (seq_t)m - 1;          /* d = sigma(m)-m-1 = s-1 */
                if ( ((seq_t)m - 1) % dm == 0 )            /* k exists */
                {
                    ++counted;
                    seq_t new_sum;
                    if (counted == 1)
                        new_sum = (seq_t)m;
                    else
                        new_sum = partial_sums[counted - 2] + (seq_t)m;
                    partial_sums[counted - 1] = new_sum;
                }
            }
        }

        /* If we still lack terms, enlarge the sieve and continue. */
        if (counted < n)
        {
            idx_t new_limit = sieve_limit * 2;
            if (new_limit <= sieve_limit) new_limit = sieve_limit + 1; /* overflow guard */
            build_spf_custom(new_limit);
        }
    }

    return partial_sums[n - 1];
}
