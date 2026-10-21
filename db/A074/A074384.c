/*
 * A074384  Solutions to mod(sigma(x), 6) = 5.
 * https://oeis.org/A074384
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, changed
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 6, u32 n <= 1117, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A074384
 * Squares k² such that sigma(k²) ≡ 5 (mod 6).
 * Algorithm:
 *   • Pre-compute smallest prime factors up to a fixed limit.
 *   • For k = 1, 2, …, compute sigma(k²) modulo 6 using its prime
 *     factorisation (multiplicative property).
 *   • Store every k² for which sigma(k²) ≡ 5 (mod 6).
 *   • a(n) returns the n‑th stored term.  The table is built lazily on
 *     the first call and then reused.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* sequence entry type (supplied by the build) */
extern seq_t a(idx_t n);

/* ----------------------------------------------------------------- */
/*  Constants and static storage                                     */
/* ----------------------------------------------------------------- */

#define LIMIT 2000000U                     /* maximum k to test */

static unsigned int *spf = NULL;           /* smallest prime factor array */
static seq_t *terms = NULL;                /* array of sequence terms   */
static size_t terms_len = 0;               /* number of terms stored   */
static size_t terms_cap = 0;               /* current capacity of array*/
static bool initialized = false;

/* ----------------------------------------------------------------- */
/*  Helper: build the SPF sieve                                      */
/* ----------------------------------------------------------------- */
static void build_spf(void)
{
    if (spf != NULL)
        return;

    spf = (unsigned int *)malloc((LIMIT + 1) * sizeof(unsigned int));
    if (!spf)
        abort();

    for (size_t i = 0; i <= LIMIT; ++i)
        spf[i] = 0;

    for (size_t i = 2; i <= LIMIT; ++i) {
        if (spf[i] == 0) {              /* i is prime */
            spf[i] = (unsigned int)i;
            if (i * i <= LIMIT) {
                for (size_t j = i * i; j <= LIMIT; j += i)
                    if (spf[j] == 0)
                        spf[j] = (unsigned int)i;
            }
        }
    }
}

/* ----------------------------------------------------------------- */
/*  Helper: generate the sequence terms                               */
/* ----------------------------------------------------------------- */
static void build_terms(void)
{
    if (terms != NULL)
        return;

    terms_cap = 1024;
    terms = (seq_t *)malloc(terms_cap * sizeof(seq_t));
    if (!terms)
        abort();

    for (size_t k = 1; k <= LIMIT; ++k) {
        /* compute sigma(k^2) modulo 6 */
        unsigned int prod_mod6 = 1;
        size_t tmp = k;

        while (tmp > 1) {
            unsigned int p = spf[tmp];
            unsigned int cnt = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++cnt;
            }
            unsigned int exp = cnt * 2;  /* exponent in k^2 */

            /* sum of geometric series 1 + p + ... + p^exp (mod 6) */
            unsigned int sum_mod6 = 0;
            unsigned int term_mod = 1 % 6;
            for (unsigned int i = 0; i <= exp; ++i) {
                sum_mod6 += term_mod;
                if (sum_mod6 >= 6) sum_mod6 -= 6;
                term_mod = (unsigned int)((term_mod * (p % 6)) % 6);
            }

            prod_mod6 = (unsigned int)((prod_mod6 * sum_mod6) % 6);
        }

        if (prod_mod6 == 5) {                  /* qualifies */
            seq_t val = (seq_t)k * (seq_t)k;   /* k^2 */
            if (terms_len == terms_cap) {
                terms_cap *= 2;
                seq_t *tmpbuf = (seq_t *)realloc(terms, terms_cap * sizeof(seq_t));
                if (!tmpbuf)
                    abort();
                terms = tmpbuf;
            }
            terms[terms_len++] = val;
        }
    }
}

/* ----------------------------------------------------------------- */
/*  Initialization helper                                             */
/* ----------------------------------------------------------------- */
static void init(void)
{
    if (initialized)
        return;
    build_spf();
    build_terms();
    initialized = true;
}

/* ----------------------------------------------------------------- */
/*  Required function                                                */
/* ----------------------------------------------------------------- */
seq_t a(idx_t n)
{
    init();

    if (n == 0)          /* sequence offset is 1, undefined below */
        return 0;

    if ((idx_t)(n - 1) >= terms_len)
        return 0;        /* out of range – behaviour defined as 0 */

    return terms[n - 1];
}
