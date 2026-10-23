/*
 * A110087  Numbers k such that sigma(k) - phi(k) > tau(k)^omega(k), where
 *          sigma = A000203, phi = A000010, tau = A000005 and omega = A001221.
 * https://oeis.org/A110087
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   A110087 = numbers k such that sigma(k) - phi(k) >
 *            tau(k)^omega(k) * (sigma = A000203, phi = A000010, tau =
 *            A000005, omega = A001221)
 * Range:     u8 n <= 5, u16 n <= 144, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   seq_pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/seq_pow_seq.h"

/* smallest‑prime‑factor table ------------------------------------------------*/
static uint32_t *spf = NULL;      /* array of smallest prime factors   */
static size_t spf_limit = 0;      /* largest index with a filled value */

/* ensure spf[1..limit] is available, rebuilding if necessary */
static void ensure_spf(size_t limit)
{
    if (limit <= spf_limit) return;
    size_t new_limit = spf_limit ? spf_limit : 1;
    while (new_limit < limit) new_limit <<= 1;

    uint32_t *new_spf = (uint32_t *)calloc(new_limit + 1u, sizeof(uint32_t));
    if (!new_spf) exit(EXIT_FAILURE);

    for (uint32_t i = 2; (uint64_t)i * i <= new_limit; ++i)
    {
        if (new_spf[i] == 0)
        {
            new_spf[i] = i;
            for (uint64_t j = (uint64_t)i * i; j <= new_limit; j += i)
                if (new_spf[j] == 0) new_spf[j] = i;
        }
    }
    for (uint32_t i = 2; i <= new_limit; ++i)
        if (new_spf[i] == 0) new_spf[i] = i;

    free(spf);
    spf = new_spf;
    spf_limit = new_limit;
}

/* factorisation and required arithmetic ------------------------------------*/
static void factorise(uint64_t n, seq_t *sigma, idx_t *tau, idx_t *omega, seq_t *phi)
{
    uint64_t tmp   = n;
    seq_t   s_val  = 1;
    idx_t   t_val  = 1;
    idx_t   o_val  = 0;
    uint64_t p_val = n;

    while (tmp > 1)
    {
        uint32_t p = spf[tmp];
        idx_t exp = 0;
        while (tmp % p == 0) { tmp /= p; ++exp; }

        /* (p^{exp+1}-1)/(p-1) */
        seq_t term = seq_pow_seq((seq_t)p, (idx_t)(exp + 1)) - 1;
        term /= (seq_t)(p - 1);
        s_val *= term;

        t_val *= (exp + 1);
        ++o_val;

        p_val = (p_val / p) * (p - 1);
    }

    *sigma = s_val;
    *tau   = t_val;
    *omega = o_val;
    *phi   = (seq_t)p_val;
}

/* ---------------------------------------------------------------------------*/

static seq_t *a_seq    = NULL;   /* cached sequence values              */
static size_t a_cap    = 0;      /* allocated capacity                   */
static size_t a_size   = 0;      /* number of values stored              */
static uint64_t next_k = 1;      /* next candidate to test (persistent)  */

/* main sequence access -------------------------------------------------------*/
seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* offset is 1; value for n<1 is undefined */

    if (!a_seq)  /* first call: initialise storage */
    {
        a_cap = 16;
        a_seq = (seq_t *)malloc(a_cap * sizeof(seq_t));
        a_size = 0;
        next_k  = 1;
    }

    /* prepare the SPF table up to the range we need */
    ensure_spf(64);

    /* generate terms until we have at least n of them */
    while (a_size < n)
    {
        uint64_t k = next_k;
        if (k > spf_limit) ensure_spf((size_t)k * 2);

        seq_t sigma, phi;
        idx_t tau, omega;
        factorise(k, &sigma, &tau, &omega, &phi);

        seq_t left  = sigma - phi;                 /* sigma - phi   */
        seq_t right = seq_pow_seq((seq_t)tau, omega); /* tau^omega    */

        if (left > right)                         /* condition true */
        {
            if (a_size == a_cap)                  /* grow array    */
            {
                a_cap <<= 1;
                a_seq = (seq_t *)realloc(a_seq, a_cap * sizeof(seq_t));
            }
            a_seq[a_size++] = (seq_t)k;
        }
        ++next_k;
    }

    return a_seq[n - 1];
}
