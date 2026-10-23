/*
 * A143189  Numbers k such that at least one of the following equations holds:
 *          phi(2*k-1) = phi(2*k), sigma(2*k-1) = sigma(2*k), tau(2*k-1) =
 *          tau(2*k).
 * https://oeis.org/A143189
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 38, u16 n <= 6939, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A143189: a(n) is the nth integer k such that  
   φ(2k‑1)=φ(2k) or σ(2k‑1)=σ(2k) or τ(2k‑1)=τ(2k).  
   Computed by incremental search with on‑the‑fly prime
   factorisation. */

#include <stdint.h>
#include <stdlib.h>

static seq_t *cache = NULL;          /* stored k values */
static size_t cache_len = 0;         /* number of cached elements */
static idx_t next_k = 0;             /* next k to test */

/* compute φ, σ, τ for a uint64_t value */
static void factor_stats(uint64_t n,
                         uint64_t *phi_out,
                         uint64_t *sigma_out,
                         uint64_t *tau_out)
{
    uint64_t temp = n;
    *phi_out   = n;
    *sigma_out = 1;
    *tau_out   = 1;

    for (uint64_t p = 2; p * p <= temp; ++p)
    {
        if (temp % p == 0)
        {
            int exp = 0;
            uint64_t p_pow = 1;
            while (temp % p == 0)
            {
                temp   /= p;
                exp    ++;
                p_pow *= p;
            }

            /* φ update: φ(n) = φ(n) – φ(n)/p  */
            *phi_out -= *phi_out / p;

            /* Σ update: product of (p^(e+1)-1)/(p-1) */
            uint64_t sum = (p_pow * p - 1) / (p - 1);
            *sigma_out *= sum;

            /* τ update: (e+1) multiplicative factor */
            *tau_out *= (uint64_t)(exp + 1);
        }
    }

    if (temp > 1)
    {
        /* remaining prime factor */
        *phi_out   -= *phi_out / temp;
        *sigma_out *= (1 + temp);
        *tau_out   *= 2;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined for n < 1, but harmless */

    /* increase cache until it contains the nth element */
    while (cache_len < (size_t)n)
    {
        ++next_k;                     /* test next k */
        uint64_t a_val = 2 * next_k - 1;
        uint64_t b_val = 2 * next_k;

        uint64_t phi_a, sigma_a, tau_a;
        uint64_t phi_b, sigma_b, tau_b;

        factor_stats(a_val, &phi_a, &sigma_a, &tau_a);
        factor_stats(b_val, &phi_b, &sigma_b, &tau_b);

        /* check for any equality */
        if (phi_a == phi_b || sigma_a == sigma_b || tau_a == tau_b)
        {
            /* grow cache if necessary */
            seq_t *tmp = (seq_t *)realloc(cache,
                            (cache_len + 1) * sizeof(seq_t));
            if (!tmp) abort();  /* allocation failure */
            cache = tmp;
            cache[cache_len] = (seq_t)next_k;
            ++cache_len;
        }
    }

    return cache[n - 1];
}
