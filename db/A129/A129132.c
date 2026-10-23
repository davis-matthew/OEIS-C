/*
 * A129132  Partial sums of A051903.
 * https://oeis.org/A129132
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = - Σ_{i=2..n} μ(i) · Σ_{j≥1} ⌊ n / i^j ⌋ where μ is the
 *            Möbius function
 * Range:     u8 n <= 154, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static idx_t mu_max = 0;          /* largest n for which μ has been computed */
static int8_t *mu_tbl = NULL;     /* μ values: -1, 0, or +1 for 0<=i<=mu_max */
static uint32_t *spf_tbl = NULL;  /* smallest prime factor for each i */

/* Ensure that μ(i) and spf(i) are known for all 0 <= i <= n  */
static void ensure_mu(idx_t n)
{
    if (n <= mu_max) return;

    /* Allocate or grow the tables */
    size_t new_sz = (size_t)(n + 1);
    mu_tbl  = (int8_t*)realloc(mu_tbl,  new_sz * sizeof(int8_t));
    spf_tbl = (uint32_t*)realloc(spf_tbl, new_sz * sizeof(uint32_t));

    /* Initialise new entries */
    for (idx_t i = mu_max; i <= n; ++i) {
        mu_tbl[i]  = 0;
        spf_tbl[i] = (uint32_t)i;
    }

    /* Sieve smallest prime factors up to n */
    for (idx_t i = 2; i * i <= n; ++i) {
        if (spf_tbl[i] == (uint32_t)i) {           /* i is prime */
            for (idx_t j = i * i; j <= n; j += i) {
                if (spf_tbl[j] == (uint32_t)j)
                    spf_tbl[j] = (uint32_t)i;
            }
        }
    }

    /* Compute Möbius values using the spf table */
    mu_tbl[1] = 1;
    for (idx_t i = 2; i <= n; ++i) {
        if (spf_tbl[i] == (uint32_t)i) {       /* i is prime */
            mu_tbl[i] = -1;
        } else {
            idx_t div = i / spf_tbl[i];
            if (div % spf_tbl[i] == 0)
                mu_tbl[i] = 0;                  /* squared prime factor */
            else
                mu_tbl[i] = -(mu_tbl[div]);    /* square‑free, flip sign */
        }
    }

    mu_max = n;
}

seq_t a(idx_t n)
{
    if (n <= 1) return 0;        /* a(1)=0, a(0) not required */

    ensure_mu(n);

    seq_t sum_pos = 0;          /* Σ when μ(i) = -1  (addend) */
    seq_t sum_neg = 0;          /* Σ when μ(i) = +1  (subtrahend) */

    for (idx_t i = 2; i <= n; ++i) {
        int8_t mu = mu_tbl[i];
        if (mu == 0) continue;          /* skip squares of primes */

        idx_t power = i;
        while (power <= n) {
            seq_t term = (seq_t)(n / power);   /* floor(n / i^j) */
            if (mu == -1)
                sum_pos += term;
            else /* mu == 1 */
                sum_neg += term;

            /* Prevent overflow in power multiplication */
            if (power > n / i) break;
            power *= i;
        }
    }

    seq_t result = sum_pos;
    result -= sum_neg;                /* sum_pos >= sum_neg for all n */
    return result;
}
