/*
 * A084766  Sum of numbers of prime-factors {2*n-p: 2<p<2*n, p prime}, with
 *          repetitions.
 * https://oeis.org/A084766
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{p prime, 2 < p < 2*n} Ω(2*n - p) where Ω(k) is the
 *            number of prime factors of k counted with multiplicity
 * Range:     u8 n <= 312, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

/* -------------------------------------------------------------------- */
/*  Sieve data structures                                              */
/* -------------------------------------------------------------------- */
static uint32_t *spf = NULL;          /* smallest prime factor, indexed 0..limit */
static uint8_t  *omega = NULL;        /* Ω(i) for i = 0..limit                  */
static uint32_t *prime_list = NULL;   /* list of primes up to limit             */
static size_t   prime_cnt = 0;        /* number of primes stored               */
static uint32_t sieve_limit = 0;      /* maximum n already sieved (2*n value)  */

/* -------------------------------------------------------------------- */
/*  Build or extend the sieve up to at least 2*n                         */
/* -------------------------------------------------------------------- */
static void ensure_sieve(idx_t n)
{
    idx_t new_limit = 2 * n;                     /* need to cover 2*n          */
    if (new_limit <= sieve_limit) return;       /* already enough             */

    /* free previous arrays if any */
    free(spf);
    free(omega);
    free(prime_list);

    /* allocate new arrays; use a generous buffer for the prime list */
    spf        = (uint32_t *)malloc((new_limit + 1) * sizeof(uint32_t));
    omega      = (uint8_t  *)malloc((new_limit + 1) * sizeof(uint8_t));
    /* ~limit / ln(limit) ≈ 0.2*limit for small limits, so allocate 0.25*limit */
    prime_list = (uint32_t *)malloc((new_limit / 4 + 100) * sizeof(uint32_t));
    if (!spf || !omega || !prime_list) {
        /* Allocation failed – abort gracefully */
        free(spf); free(omega); free(prime_list);
        exit(1);
    }

    /* Linear (Euler) sieve up to new_limit */
    prime_cnt = 0;
    for (idx_t i = 0; i <= new_limit; ++i) spf[i] = 0;
    for (idx_t i = 2; i <= new_limit; ++i) {
        if (spf[i] == 0) {
            spf[i] = (uint32_t)i;
            prime_list[prime_cnt++] = (uint32_t)i;
        }
        for (size_t j = 0; j < prime_cnt; ++j) {
            uint32_t p = prime_list[j];
            if (p > spf[i] || (idx_t)i * p > new_limit) break;
            spf[i * p] = p;
        }
    }

    /* Pre‑compute Ω(k) using the spf array */
    omega[0] = 0;
    omega[1] = 0;
    for (idx_t i = 2; i <= new_limit; ++i) {
        uint32_t p = spf[i];
        omega[i] = omega[i / p] + 1;
    }

    sieve_limit = (uint32_t)new_limit;
}

/* -------------------------------------------------------------------- */
/*  Main function                                                      */
/* -------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* out of OEIS range, but safe */

    ensure_sieve(n);               /* make sure 2*n is covered      */

    idx_t two_n = 2 * n;
    seq_t sum   = 0;

    /* iterate over all primes p with 2 < p < 2*n */
    for (size_t i = 0; i < prime_cnt && prime_list[i] < two_n; ++i) {
        uint32_t p = prime_list[i];
        if (p == 2) continue;          /* exclude p = 2             */
        idx_t m = two_n - p;            /* m > 0                    */
        sum += (seq_t)omega[m];
    }
    return sum;
}
