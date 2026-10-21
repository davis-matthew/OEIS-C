/*
 * A070999  Numbers n such that the denominator of Sum_{k=1..n} 1/gcd(n,k) is
 *          not equal to n.
 * https://oeis.org/A070999
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = the n‑th integer k such that * gcd(k, Σ_{d|k} d·φ(d)) > 1
 *            * equivalently, the denominator of * Σ_{k=1..k} 1/gcd(k,k) is
 *            not equal to k. * * The implementation uses a simple sieve for
 *            smallest prime * factors (spf) to factor integers on demand and
 *            * computes Σ_{d|k} d·φ(d) modulo k to avoid...
 * Range:     u8 n <= 65, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/gcd_u64.h"

/* ------------------------------------------------------------------ */
/* Basic 64‑bit GCD used for the modulo computation                    */

/* ------------------------------------------------------------------ */
/* Sieve for smallest prime factor (spf) and list of primes            */
static uint32_t *spf = NULL;       /* spf[i] = smallest prime factor of i   */
static size_t   spf_size = 0;      /* number of elements in spf (max+1)    */

static uint32_t *prime_arr = NULL; /* array of primes found by sieve       */
static size_t   prime_cnt   = 0;    /* current number of primes             */

static void ensure_spf(size_t limit)
{
    if (spf_size >= limit + 1) return;   /* already large enough */

    /* reallocate arrays big enough for indices 0..limit */
    free(spf);
    spf = (uint32_t *)calloc(limit + 1, sizeof(uint32_t));

    free(prime_arr);
    prime_arr = (uint32_t *)malloc((limit + 1) * sizeof(uint32_t));
    prime_cnt = 0;

    /* linear sieve */
    for (uint32_t i = 2; i <= limit; ++i) {
        if (spf[i] == 0) {                /* i is prime */
            spf[i] = i;
            prime_arr[prime_cnt++] = i;
        }
        for (size_t j = 0; j < prime_cnt; ++j) {
            uint32_t p = prime_arr[j];
            uint64_t prod = (uint64_t)i * p;
            if (p > spf[i] || prod > limit) break;
            spf[(size_t)prod] = p;
        }
    }
    spf_size = limit + 1;
}

/* ------------------------------------------------------------------ */
/* Compute Σ_{d|n} d·φ(d) modulo n.  All work is done in 64‑bit to
 * avoid overflow on small seq_t types.                                        */
static uint64_t sum_div_phi_mod(uint32_t n, uint64_t mod)
{
    if (n == 1) return 1 % mod;   /* φ(1)=1 and d=1 only */

    /* factorise n */
    uint32_t pr[32];
    uint32_t exp[32];
    size_t cnt = 0;
    uint32_t temp = n;
    while (temp > 1) {
        uint32_t p = spf[temp];
        uint32_t e = 0;
        while (temp % p == 0) {
            temp /= p;
            ++e;
        }
        pr[cnt]   = p;
        exp[cnt]  = e;
        ++cnt;
    }

    uint64_t total = 1 % mod;    /* product over prime powers */

    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p      = pr[i];
        uint32_t a      = exp[i];
        uint64_t ga     = 1 % mod;      /* result for p^a  */
        uint64_t pow    = p % mod;      /* current p^{2k-1}  */
        uint64_t p_m1   = (p - 1) % mod;

        for (uint32_t k = 1; k <= a; ++k) {
            uint64_t term = (pow * p_m1) % mod;
            ga += term;
            if (ga >= mod) ga -= mod;

            /* advance to next odd exponent: p^{2k+1} = pow * p^2 */
            pow = ((pow * p) % mod * p) % mod;
        }

        total = (total * ga) % mod;
    }

    return total % mod;
}

/* ------------------------------------------------------------------ */
/* Test whether integer k satisfies the condition                     */
static int in_sequence(uint32_t k)
{
    if (k < 2) return 0;

    ensure_spf(k);

    uint64_t rem = sum_div_phi_mod(k, (uint64_t)k);   /* Σ ≡ Σ mod k */
    uint64_t g   = gcd_u64(rem, (uint64_t)k);

    return (g > 1);
}

/* ------------------------------------------------------------------ */
/* Caching of the sequence terms.  seq_arr stores the values,  not
 * the indices.  The array grows as needed. */
static seq_t *seq_arr = NULL;
static size_t seq_len = 0;
static size_t seq_cap = 0;
static uint32_t n_checked = 1;   /* last integer that has been examined */

/* ------------------------------------------------------------------ */
/* Return the n‑th term of the sequence (1‑based index)                */
seq_t a(idx_t n)
{
    /* Evaluate the sequence lazily until we have n terms */
    while (seq_len < n) {
        ++n_checked;                     /* next integer to test   */
        if (in_sequence(n_checked)) {
            if (seq_len == seq_cap) {    /* grow the storage      */
                seq_cap = seq_cap ? seq_cap * 2 : 4;
                seq_arr = (seq_t *)realloc(seq_arr,
                                            seq_cap * sizeof(seq_t));
            }
            seq_arr[seq_len++] = (seq_t)n_checked;  /* store the term  */
        }
    }
    return seq_arr[n - 1];                 /* 0‑based array, 1‑based index */
}
