/*
 * A071173  Nonprimes which terminate in their sum of prime factors.
 * https://oeis.org/A071173
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 42, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: the nth non‑prime number whose decimal representation ends with
 * the sum of its prime factors (with multiplicity).  The first few terms are
 * 1,4,18,220,...  */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* ------------------------------------------------------------------ */
/*  Smallest prime factor sieve, dynamic, for numbers up to 32‑bit.    */
/* ------------------------------------------------------------------ */
static uint32_t *spf = NULL;      /* spf[i] = smallest prime factor of i */
static size_t  spf_size = 0;      /* length of the spf array */

/* Expand the sieve so that indices < limit are valid. */
static void ensure_spf(size_t limit)
{
    if (limit <= spf_size)
        return;

    size_t new_size = spf_size ? spf_size * 2 : 256;
    while (new_size <= limit)
        new_size <<= 1;

    uint32_t *new_spf = (uint32_t *)realloc(spf, new_size * sizeof(uint32_t));
    if (!new_spf) {
        /* allocation failed – terminate */
        exit(EXIT_FAILURE);
    }
    spf = new_spf;

    /* initialise new entries */
    for (size_t i = spf_size; i < new_size; ++i)
        spf[i] = (uint32_t)i;

    /* sieve up to new_size – only need to start at 2 */
    for (size_t i = 2; (uint64_t)i * i < new_size; ++i) {
        if (spf[i] == (uint32_t)i) {           /* i is prime */
            uint64_t ii = (uint64_t)i * i;
            for (uint64_t j = ii; j < new_size; j += i) {
                if (spf[j] == (uint32_t)j)
                    spf[j] = (uint32_t)i;
            }
        }
    }

    spf_size = new_size;
}

/* ------------------------------------------------------------------ */
/*  Sum of prime factors with multiplicity for a 64‑bit integer.       */
/* ------------------------------------------------------------------ */
static uint64_t sopfr_u64(uint64_t n)
{
    if (n < 2)
        return 0;                 /* 1 has no prime factors */

    ensure_spf((size_t)n + 1);
    uint64_t sum = 0;
    while (n > 1) {
        uint32_t p = spf[(size_t)n];
        sum += p;
        n /= p;
    }
    return sum;
}

/* ------------------------------------------------------------------ */
/*  Helper: compute 10^d where d is the number of decimal digits of x.    */
static uint64_t pow10_digits(uint64_t x)
{
    if (x == 0)
        return 1;                 /* 10^0 */

    uint64_t pow = 1;
    while (x > 0) {
        pow *= 10;
        x /= 10;
    }
    return pow;
}

/* ------------------------------------------------------------------ */
/*  The required sequence function.                                   */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    /* Static state to make calls independent of order. */
    static bool initialized = false;
    static seq_t *terms      = NULL;   /* array of found terms */
    static size_t terms_cap  = 0;
    static size_t terms_cnt  = 0;
    static uint64_t current   = 1;      /* number being examined */

    if (!initialized) {
        initialized = true;
        terms_cap = 128;
        terms = (seq_t *)malloc(terms_cap * sizeof(seq_t));
        if (!terms) exit(EXIT_FAILURE);
        terms_cnt = 0;
        current = 1;
    }

    /* Generate terms until we have at least n of them. */
    while (terms_cnt < (size_t)n) {
        uint64_t x = current;

        /* 1 is always a term (special case). */
        if (x == 1) {
            if (terms_cnt == 0 || terms[terms_cnt - 1] != (seq_t)1)
                terms[terms_cnt++] = (seq_t)1;
            ++current;
            continue;
        }

        /* Ensure we have SPF data for this number. */
        ensure_spf((size_t)x + 1);

        /* Skip primes. */
        if (spf[(size_t)x] == (uint32_t)x) {
            ++current;
            continue;
        }

        /* Compute sum of prime factors. */
        uint64_t sum = sopfr_u64(x);

        /* Check the termination condition. */
        uint64_t mod = pow10_digits(sum);
        if (x % mod == sum) {
            if (terms_cnt == terms_cap) {
                terms_cap <<= 1;
                seq_t *tmp = (seq_t *)realloc(terms, terms_cap * sizeof(seq_t));
                if (!tmp) exit(EXIT_FAILURE);
                terms = tmp;
            }
            terms[terms_cnt++] = (seq_t)x;
        }

        ++current;
    }

    return terms[n - 1];
}
