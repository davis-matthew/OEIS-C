/*
 * A015819  Numbers k such that phi(k + 3) | sigma(k).
 * https://oeis.org/A015819
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 32, u16 n <= 642, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A015819:  a(n) = k such that φ(k + 3) | σ(k)  */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* typedefs supplied by the build */
/* seq_t  : unsigned integer type for sequence values   */
/* idx_t  : unsigned integer type for indices          */

static uint32_t *spf = NULL;          /* smallest prime factor table   */
static size_t   spf_size = 0;         /* size = current limit + 1      */

static seq_t *terms = NULL;           /* array of found k values        */
static size_t terms_cnt = 0;          /* number of terms stored         */
static size_t terms_cap = 0;          /* capacity of terms array       */

static idx_t last_k = 0;              /* last k processed               */
static bool initialized = false;     /* one‑time initialization flag   */

/* --------------------------------------------------------------------- */
/* Compute smallest prime factor table up to 'limit'                      */
static void compute_spf(size_t limit)
{
    uint32_t *new_spf = (uint32_t *)malloc((limit + 1) * sizeof(uint32_t));
    for (size_t i = 0; i <= limit; ++i) new_spf[i] = (uint32_t)i;
    for (size_t i = 2; i * i <= limit; ++i) {
        if (new_spf[i] == (uint32_t)i) {
            for (size_t j = i * i; j <= limit; j += i) {
                if (new_spf[j] == (uint32_t)j) new_spf[j] = (uint32_t)i;
            }
        }
    }
    if (spf) free(spf);
    spf = new_spf;
    spf_size = limit + 1;
}

/* --------------------------------------------------------------------- */
/* Ensure that the SPF table covers at least 'needed' indices             */
static void ensure_spf_limit(idx_t needed)
{
    if ((size_t)needed < spf_size) return;
    size_t new_limit = spf_size > 0 ? spf_size * 2 : 100;
    while (new_limit <= (size_t)needed) new_limit *= 2;
    compute_spf(new_limit);
}

/* --------------------------------------------------------------------- */
/* Factorial sum sigma using the SPF table                                */
static seq_t sigma_by_factor(idx_t n)
{
    seq_t sum = 1;
    while (n > 1) {
        uint32_t p = spf[n];
        seq_t pp = 1;
        while (n % p == 0) {
            n /= p;
            pp *= p;
        }
        /* term = (p^(e+1) - 1) / (p - 1) = (pp * p - 1) / (p - 1) */
        sum *= (pp * (seq_t)p - 1) / ((seq_t)p - 1);
    }
    return sum;
}

/* --------------------------------------------------------------------- */
/* Euler's totient phi using the SPF table                                */
static seq_t phi_by_factor(idx_t n)
{
    seq_t res = n;
    while (n > 1) {
        uint32_t p = spf[n];
        res = res / p * ((seq_t)p - 1);
        while (n % p == 0) n /= p;
    }
    return res;
}

/* --------------------------------------------------------------------- */
/* One‑time initialization of data structures                            */
static void init(void)
{
    compute_spf(5000000);          /* initial limit large enough for n ≤ 10000 */
    terms_cap = 16;
    terms = (seq_t *)malloc(terms_cap * sizeof(seq_t));
    terms_cnt = 0;
    last_k = 0;
    initialized = true;
}

/* --------------------------------------------------------------------- */
/* Main function requested by the judge                                    */
seq_t a(idx_t n)
{
    if (!initialized) init();

    while (terms_cnt < n) {
        ++last_k;
        /* make sure SPF table covers k+3 for phi calculation          */
        ensure_spf_limit(last_k + 3);

        seq_t sigma_k  = sigma_by_factor(last_k);
        seq_t phi_kp3  = phi_by_factor(last_k + 3);
        if (phi_kp3 != 0 && sigma_k % phi_kp3 == 0) {
            if (terms_cnt >= terms_cap) {
                terms_cap = terms_cap * 2;
                terms = (seq_t *)realloc(terms, terms_cap * sizeof(seq_t));
            }
            terms[terms_cnt++] = (seq_t)last_k;
        }
    }

    return terms[n - 1];
}
