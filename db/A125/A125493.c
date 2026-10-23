/*
 * A125493  Composite deficient numbers.
 * https://oeis.org/A125493
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, changed
 * Formula:   not stated
 * Range:     u8 n <= 20, u16 n <= 65, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A125493 – Composite deficient numbers.
   a(n) is the nth composite integer whose sum of all divisors σ(n)
   is strictly less than 2·n.  The sequence starts 4, 8, 9, 10, ...
   This routine builds the sequence on demand by sieving smallest
   prime factors and computing σ(n) for each candidate. */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

/* static storage for the sequence and the sieve */
static seq_t *comp_def_seq = NULL;      /* array of found numbers */
static size_t comp_def_len = 0;          /* current length */
static size_t comp_def_cap = 0;          /* current capacity */

static uint32_t *spf = NULL;            /* smallest prime factor sieve */
static size_t spf_cap = 0;              /* current sieve size (indexable 0..spf_cap-1) */

static idx_t last_checked = 1;          /* highest number examined so far */

/* enlarge or initialise the spf array to cover all values up to 'limit' */
static void init_spf(size_t limit)
{
    if (limit + 1 <= spf_cap) return;   /* already large enough */

    uint32_t *new_spf = (uint32_t *)realloc(spf, (limit + 1) * sizeof(uint32_t));
    if (!new_spf) abort();               /* memory fail */

    for (size_t i = spf_cap; i <= limit; ++i)
        new_spf[i] = (uint32_t)i;

    for (size_t p = 2; p * p <= limit; ++p)
        if (new_spf[p] == (uint32_t)p)
            for (size_t m = p * p; m <= limit; m += p)
                if (new_spf[m] == (uint32_t)m)
                    new_spf[m] = (uint32_t)p;

    spf = new_spf;
    spf_cap = limit + 1;
}

/* ensure the sequence array can hold 'need' elements */
static void ensure_seq_capacity(size_t need)
{
    if (need <= comp_def_cap) return;

    size_t new_cap = (comp_def_cap == 0) ? 32 : comp_def_cap * 2;
    while (new_cap < need) new_cap *= 2;

    seq_t *new_seq = (seq_t *)realloc(comp_def_seq, new_cap * sizeof(seq_t));
    if (!new_seq) abort();

    comp_def_seq = new_seq;
    comp_def_cap = new_cap;
}

/* test whether x is composite (x ≥ 2 and not prime) */
static bool is_composite(uint32_t x)
{
    return x >= 2 && spf[x] != x;
}

/* compute σ(n) with an early‑out if it would exceed 'limit' */
static seq_t sigma_of(uint32_t n, seq_t limit)
{
    seq_t sigma = 1;
    uint32_t x = n;

    while (x > 1)
    {
        uint32_t p = spf[x];
        unsigned e = 0;
        while (x % p == 0) { x /= p; ++e; }

        seq_t pow = 1;
        for (unsigned k = 0; k < e + 1; ++k)
            pow *= (seq_t)p;

        seq_t term = (pow - 1) / ((seq_t)p - 1);

        if (sigma > limit / term)          /* would overflow limit */
            return limit + 1;              /* definitely not deficient */

        sigma *= term;
    }
    return sigma;
}

/* produce at least 'need' terms of the sequence */
static void generate_to(size_t need)
{
    ensure_seq_capacity(need);

    /* initialise sieve to a modest size; it will grow on demand */
    if (spf_cap == 0) init_spf(5000);

    while (comp_def_len < need)
    {
        ++last_checked;

        if (!is_composite((uint32_t)last_checked))
            continue;

        seq_t limit = ((seq_t)2 * (seq_t)last_checked);
        seq_t sig = sigma_of((uint32_t)last_checked, limit);

        if (sig < limit)
            comp_def_seq[comp_def_len++] = (seq_t)last_checked;

        /* enlarge sieve when we approach its boundary */
        if ((size_t)last_checked + 100 >= spf_cap)
            init_spf((size_t)last_checked * 2 + 100);
    }
}

/* entry point required by the benchmark framework */
seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* undefined; return 0 for safety */
    generate_to((size_t)n);
    return comp_def_seq[(size_t)n - 1];
}
