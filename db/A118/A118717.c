/*
 * A118717  Sum of two consecutive semiprimes.
 * https://oeis.org/A118717
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A001358(n) + A001358(n+1) where A001358 is the list of
 *            semiprimes in increasing order
 * Range:     u8 n <= 42, u16 n <= 8109, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Needed headers */
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/*--------------------------------------------------------------------
  Dynamic prime sieve (stores primes as uint32_t)
--------------------------------------------------------------------*/
static uint32_t *primes = NULL;
static size_t    primes_cnt = 0;
static size_t    primes_cap = 0;

static void sieve_primes_upto(uint64_t limit)
{
    /* Ensure enough capacity for the primes array */
    if (primes_cap < limit) {
        primes_cap = (size_t)limit + 16;
        primes = (uint32_t *)realloc(primes, primes_cap * sizeof(uint32_t));
    }
    primes_cnt = 0;

    /* Simple sieve using a bool array */
    bool *is_comp = (bool *)calloc((size_t)limit + 1, sizeof(bool));
    if (!is_comp) {
        /* allocation failure → keep primes empty */
        return;
    }

    for (uint64_t i = 2; i <= limit; ++i) {
        if (!is_comp[(size_t)i]) {
            if (primes_cnt == primes_cap) {
                primes_cap = primes_cap * 2 + 16;
                primes = (uint32_t *)realloc(primes, primes_cap * sizeof(uint32_t));
            }
            primes[primes_cnt++] = (uint32_t)i;

            if (i * i <= limit) {
                for (uint64_t j = i * i; j <= limit; j += i)
                    is_comp[(size_t)j] = true;
            }
        }
    }
    free(is_comp);
}

/*--------------------------------------------------------------------
  Dynamic semiprime list
--------------------------------------------------------------------*/
static uint64_t *semiprimes = NULL;
static size_t    semiprimes_cnt = 0;
static size_t    semiprimes_cap = 0;
static uint64_t  semiprime_max_val = 0;      /* largest value generated */

/* comparison for qsort */
static int cmp_uint64(const void *a, const void *b)
{
    uint64_t va = *(const uint64_t *)a;
    uint64_t vb = *(const uint64_t *)b;
    return (va < vb) ? -1 : (va > vb);
}

/* Generate all semiprimes <= max_val and store them sorted, unique */
static void generate_semiprimes_up_to(uint64_t max_val)
{
    /* generate enough primes */
    sieve_primes_upto(max_val);

    /* reserve space for semiprimes (rough estimate) */
    if (semiprimes_cap < 256) {
        semiprimes_cap = 256;
        semiprimes = (uint64_t *)realloc(semiprimes, semiprimes_cap * sizeof(uint64_t));
    }
    semiprimes_cnt = 0;

    /* produce all pairs p <= q with p*q <= max_val */
    for (size_t i = 0; i < primes_cnt; ++i) {
        uint32_t p = primes[i];
        if ((uint64_t)p * p > max_val) break;
        for (size_t j = i; j < primes_cnt; ++j) {
            uint32_t q = primes[j];
            uint64_t prod = (uint64_t)p * (uint64_t)q;
            if (prod > max_val) break;
            if (semiprimes_cnt == semiprimes_cap) {
                semiprimes_cap = semiprimes_cap * 2 + 16;
                semiprimes = (uint64_t *)realloc(semiprimes, semiprimes_cap * sizeof(uint64_t));
            }
            semiprimes[semiprimes_cnt++] = prod;
        }
    }

    /* sort and deduplicate */
    if (semiprimes_cnt > 0) {
        qsort(semiprimes, semiprimes_cnt, sizeof(uint64_t), cmp_uint64);
        size_t uniq = 0;
        for (size_t k = 0; k < semiprimes_cnt; ++k) {
            if (uniq == 0 || semiprimes[k] != semiprimes[uniq - 1]) {
                semiprimes[uniq++] = semiprimes[k];
            }
        }
        semiprimes_cnt = uniq;
    }

    semiprime_max_val = max_val;
}

/* Ensure that we have at least n+1 semiprimes (0‑based index needed: n) */
static void ensure_semiprimes(idx_t n)
{
    if (semiprimes_cnt > n) return;  /* already have enough */

    /* initial bound: roughly n*log n, but use a safe overshoot */
    uint64_t bound = (uint64_t)(n + 2) * 20 + 100;   /* heuristic */
    if (bound < 100) bound = 100;

    /* regenerate until enough semiprimes are available */
    while (true) {
        generate_semiprimes_up_to(bound);
        if (semiprimes_cnt > n) break;
        bound *= 2;            /* double and try again */
    }
}

/*--------------------------------------------------------------------
  Public sequence function
--------------------------------------------------------------------*/
seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* behaviour for n < 1 is irrelevant */
    ensure_semiprimes(n);                 /* need semiprimes up to index n */
    seq_t sum = (seq_t)semiprimes[n - 1] + (seq_t)semiprimes[n];
    return sum;
}
