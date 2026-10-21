/*
 * A088707  Semiprimes + 1.
 * https://oeis.org/A088707
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A001358(n) + 1 (semiprime + 1)
 * Range:     u8 n <= 82, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdint.h>

/* types supplied by the build */
extern seq_t a(idx_t n);

/* ------------------------------------------------------------------------- */
/* static storage for the sieve and semiprime list                           */
static uint32_t *spf = NULL;          /* smallest prime factor array */
static size_t   spf_size = 0;         /* size of spf array */

static seq_t   *semiprimes = NULL;    /* array of semiprimes found */
static size_t   semiprime_cnt = 0;    /* number of semiprimes stored */
static size_t   semiprime_cap = 0;    /* current capacity of semiprimes array */

static idx_t   sieve_limit = 0;       /* current upper bound of the sieve */

/* ------------------------------------------------------------------------- */
/* rebuild the sieve up to new_limit and regenerate the semiprime list      */
static void rebuild_sieve(idx_t new_limit)
{
    /* free old spf storage */
    if (spf != NULL) {
        free(spf);
    }

    /* allocate new spf */
    spf = (uint32_t *)malloc((size_t)(new_limit + 1) * sizeof(uint32_t));
    spf_size = (size_t)(new_limit + 1);
    memset(spf, 0, spf_size * sizeof(uint32_t));

    /* linear sieve for smallest prime factor */
    for (idx_t i = 2; i <= new_limit; ++i) {
        if (spf[i] == 0) {
            spf[i] = (uint32_t)i;
            if (i <= new_limit / i) {
                for (idx_t j = i * i; j <= new_limit; j += i) {
                    if (spf[j] == 0) {
                        spf[j] = (uint32_t)i;
                    }
                }
            }
        }
    }

    /* free old semiprime array */
    if (semiprimes != NULL) {
        free(semiprimes);
    }

    semiprime_cnt = 0;
    semiprime_cap = new_limit / 2 + 1;   /* initial rough capacity */
    semiprimes = (seq_t *)malloc(semiprime_cap * sizeof(seq_t));

    /* generate list of semiprimes up to new_limit */
    for (idx_t num = 4; num <= new_limit; ++num) {
        uint32_t p = spf[num];
        if (p == 0) {
            continue;
        }
        idx_t q = num / p;
        /* if q is prime, num is semiprime */
        if (spf[q] == (uint32_t)q) {
            if (semiprime_cnt == semiprime_cap) {
                semiprime_cap <<= 1;
                semiprimes = (seq_t *)realloc(semiprimes, semiprime_cap * sizeof(seq_t));
            }
            semiprimes[semiprime_cnt++] = (seq_t)num;
        }
    }

    sieve_limit = new_limit;
}

/* ------------------------------------------------------------------------- */
/* ensure that at least n_needed semiprimes have been generated               */
static void ensure_n(idx_t n_needed)
{
    if (semiprime_cnt >= n_needed) {
        return;
    }
    /* start with a reasonable limit if none yet */
    idx_t limit = (sieve_limit > 0) ? sieve_limit * 2 : 64;
    while (semiprime_cnt < n_needed) {
        limit <<= 1;
        rebuild_sieve(limit);
    }
}

/* ------------------------------------------------------------------------- */
/* user visible sequence function                                           */
seq_t a(idx_t n)
{
    /* n is 1‑based */
    if (n == 0) {
        return (seq_t)0;  /* undefined for n=0, but return 0 */
    }
    ensure_n(n);
    /* careful cast: n-1 fits into size_t for our intended range */
    size_t idx = (size_t)(n - 1);
    /* semiprimes[idx] + 1 */
    return semiprimes[idx] + (seq_t)1;
}
