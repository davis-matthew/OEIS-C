/*
 * A067003  Number of numbers <= n with same number of distinct prime factors
 *          as n.
 * https://oeis.org/A067003
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = #{m ≤ n : ω(m) = ω(n)} (ω = number of distinct prime
 *            factors)
 * Range:     u8 n <= 480, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* ---- static state --------------------------------------------------- */
static idx_t  max_computed = 0;           /* largest n already processed      */
static seq_t *a_cache      = NULL;        /* a[1..max_computed]               */
static unsigned char *omega_cache = NULL;/* ω[1..max_computed]                */
static idx_t  *spf_cache   = NULL;        /* smallest prime factor of i       */
static idx_t  spf_limit    = 0;           /* current size of spf_cache        */
static unsigned int *cnt_by_omega = NULL; /* counts of each ω seen so far    */
static idx_t  cnt_cap      = 0;           /* capacity of cnt_by_omega        */

/* ---- helper: build spf array up to 'limit' -------------------------- */
static void build_spf(idx_t limit)
{
    idx_t *new_spf = (idx_t *)malloc((limit + 1) * sizeof(idx_t));
    if (!new_spf) return;                  /* failure – unlikely in tests */

    for (idx_t i = 0; i <= limit; ++i) new_spf[i] = 0;
    if (limit >= 1) new_spf[1] = 1;

    for (idx_t i = 2; i <= limit; ++i) {
        if (new_spf[i] == 0) {          /* i is prime */
            for (idx_t j = i; j <= limit; j += i)
                if (new_spf[j] == 0) new_spf[j] = i;
        }
    }

    /* replace old table */
    free((void *)spf_cache);
    spf_cache = new_spf;
    spf_limit = limit;
}

/* ---- ensure that data up to 'n' is computed ------------------------- */
static void ensure_computed(idx_t n)
{
    if (n <= max_computed) return;

    /* grow spf if needed */
    if (n > spf_limit) build_spf(n);

    /* grow a[] and ω[] arrays */
    size_t new_cap = (size_t)(n + 1);
    a_cache      = (seq_t *)realloc(a_cache,      new_cap * sizeof(seq_t));
    omega_cache  = (unsigned char *)realloc(omega_cache,
                                            new_cap * sizeof(unsigned char));

    /* grow count array if needed */
    if (n > cnt_cap) {
        idx_t new_counts_cap = n + 1;          /* safe upper bound */
        cnt_by_omega = (unsigned int *)
                        realloc(cnt_by_omega, new_counts_cap * sizeof(unsigned int));
        /* zero new entries */
        for (idx_t i = cnt_cap; i < new_counts_cap; ++i) cnt_by_omega[i] = 0;
        cnt_cap = new_counts_cap;
    }

    /* compute new entries */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        idx_t x = i;
        unsigned int omega_val = 0;
        while (x > 1) {
            idx_t p = spf_cache[x];
            omega_val++;
            while (x % p == 0) x /= p;
        }
        omega_cache[i] = (unsigned char)omega_val;
        a_cache[i] = (seq_t)(cnt_by_omega[omega_val] + 1);
        cnt_by_omega[omega_val]++;            /* include current i */
    }

    max_computed = n;
}

/* ---- public function ------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                     /* safety for out‑of‑range */
    ensure_computed(n);
    return a_cache[n];
}
