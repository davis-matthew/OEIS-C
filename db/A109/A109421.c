/*
 * A109421  Numbers n such that tau(n)/bigomega(n) is an integer
 *          [tau(n)=number of divisors of n; bigomega(n)=number of prime
 *          divisors of n, counted with multiplicities].
 * https://oeis.org/A109421
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 200, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A109421: numbers n > 1 such that tau(n)/bigomega(n) is an integer */
#include <stdint.h>
#include <stdlib.h>

static uint32_t *spf = NULL;          /* smallest prime factors */
static uint32_t spf_limit = 0;        /* highest index for which spf is valid */

static void recompute_spf(uint32_t limit)
{
    /* Allocate and compute spf up to 'limit' using a simple sieve */
    size_t sz = (size_t)(limit + 1);
    uint32_t *new_spf = (uint32_t *)realloc(spf, sz * sizeof(uint32_t));
    for (size_t i = 0; i < sz; ++i) new_spf[i] = 0;
    for (uint32_t i = 2; i <= limit; ++i)
    {
        if (new_spf[i] == 0)      /* i is prime */
        {
            new_spf[i] = i;
            if ((uint64_t)i * i <= limit)
                for (uint64_t j = (uint64_t)i * i; j <= limit; j += i)
                    if (new_spf[(size_t)j] == 0) new_spf[(size_t)j] = i;
        }
    }
    spf = new_spf;
    spf_limit = limit;
}

static void ensure_spf(uint32_t limit)
{
    if (limit > spf_limit) recompute_spf(limit);
}

/* Compute tau(n) and bigomega(n) using spf */
static void tau_bigomega_uint32(uint32_t v, seq_t *tau_out, seq_t *bigomega_out)
{
    seq_t tau = 1;
    seq_t bigomega = 0;
    while (v > 1)
    {
        uint32_t p = spf[v];
        seq_t exp = 0;
        while (v % p == 0)
        {
            v /= p;
            exp++;
        }
        tau *= (exp + 1);
        bigomega += exp;
    }
    *tau_out = tau;
    *bigomega_out = bigomega;
}

seq_t a(idx_t n)
{
    /* static cache of sequence terms */
    static seq_t *vals = NULL;        /* array of good numbers */
    static size_t vals_cap = 0;       /* capacity of the array */
    static idx_t vals_size = 0;       /* number of terms stored */
    static uint32_t curr_test = 2;    /* next integer to test */

    if (n < 1) return 0;              /* undefined for offset zero */

    /* Generate terms until we have the n-th one */
    while (vals_size < n)
    {
        ensure_spf(curr_test);
        seq_t tau, bigomega;
        tau_bigomega_uint32((uint32_t)curr_test, &tau, &bigomega);
        if (bigomega != 0 && tau % bigomega == 0)
        {
            /* Store the term, expanding the array if necessary */
            if (vals_size == vals_cap)
            {
                size_t new_cap = vals_cap ? vals_cap * 2 : 64;
                seq_t *tmp = (seq_t *)realloc(vals, new_cap * sizeof(seq_t));
                vals = tmp;
                vals_cap = new_cap;
            }
            vals[vals_size++] = (seq_t)curr_test;
        }
        ++curr_test;
    }

    /* Return the n-th term (1‑based index) */
    return vals[n - 1];
}
