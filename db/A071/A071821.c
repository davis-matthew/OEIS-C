/*
 * A071821  Numbers whose largest prime factor is of the form 4k+1.
 * https://oeis.org/A071821
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 107, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Numbers whose largest prime factor is of the form 4k+1
   (OEIS A071821).  The function a(n) returns the n-th such number
   with the offset 1 (a(1) = 5).  */
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* ------------------------------------------------------------------ */
/*  Static auxiliary data for memoisation                             */
/* ------------------------------------------------------------------ */
static uint64_t *spf = NULL;          /* smallest prime factor table   */
static uint64_t spf_limit = 0;        /* current maximum value spf covers */

static seq_t *terms = NULL;           /* array of computed terms       */
static size_t terms_cap = 0;
static size_t terms_len = 0;

static uint64_t next_candidate = 2;   /* next integer to test */

/* ------------------------------------------------------------------ */
/*  Smallest prime factor sieve (rebuilds whole table if needed)       */
/* ------------------------------------------------------------------ */
static void ensure_spf_limit(uint64_t limit)
{
    if (limit <= spf_limit)
        return;

    /* Allocate new table: use explicit cast for C++ compatibility */
    uint64_t *new_spf = (uint64_t *)calloc((size_t)limit + 1, sizeof(uint64_t));
    if (!new_spf)
        abort();

    /* Sieve: set spf[i] = smallest prime factor of i */
    for (uint64_t i = 2; i <= limit; ++i) {
        if (new_spf[i] == 0) {
            new_spf[i] = i;
            if (i > limit / i)            /* avoid overflow */
                continue;
            for (uint64_t j = i * i; j <= limit; j += i) {
                if (new_spf[j] == 0)
                    new_spf[j] = i;
            }
        }
    }

    free(spf);
    spf = new_spf;
    spf_limit = limit;
}

/* ------------------------------------------------------------------ */
/*  Append a new term to the terms buffer                              */
/* ------------------------------------------------------------------ */
static void push_term(seq_t val)
{
    if (terms_len == terms_cap) {
        size_t new_cap = terms_cap ? terms_cap * 2 : 8;
        seq_t *tmp = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
        if (!tmp)
            abort();
        terms = tmp;
        terms_cap = new_cap;
    }
    terms[terms_len++] = val;
}

/* ------------------------------------------------------------------ */
/*  Ensure that the terms array contains at least n entries           */
/* ------------------------------------------------------------------ */
static void ensure_terms(idx_t n)
{
    while (terms_len < n) {
        /* enlarge SPF table if necessary */
        if (next_candidate > spf_limit)
            ensure_spf_limit(next_candidate * 2 + 100);

        /* compute largest prime factor of next_candidate */
        uint64_t num = next_candidate;
        uint64_t largest = 0;
        while (num > 1) {
            uint64_t p = spf[num];
            if (p > largest)
                largest = p;
            do { num /= p; } while (num % p == 0);
        }

        /* check condition */
        if (largest % 4U == 1U)
            push_term((seq_t)next_candidate);

        ++next_candidate;
    }
}

/* ------------------------------------------------------------------ */
/*  Public function required by the harness                            */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0)                /* offset starts at 1, but caller may not ask for 0 */
        return 0;
    ensure_terms(n);
    return terms[n - 1];
}
