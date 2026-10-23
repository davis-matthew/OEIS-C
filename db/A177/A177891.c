/*
 * A177891  Numbers n such that sum of proper (or aliquot) divisors of n is a
 *          semiprime.
 * https://oeis.org/A177891
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 40, u16 n <= 5791, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A177891 – Numbers n such that the sum of proper divisors of n is a
    semiprime.  The function below builds this sequence on demand, using
    a small sieve of smallest prime factors and a table of primes.  It
    works for any seq_t type (fixed‑size integer or arbitrary precision)
    and for idx_t = uint64_t.  The algorithm searches sequentially;
    a new term is appended when n is found to satisfy the definition.*/

#include <stdlib.h>   /* malloc, realloc, calloc, abort */
#include <stdbool.h>
#include <stdint.h>   /* for uint32_t */

/* -------------------------------------------------------------------------- */
/*  Static state:  sieve, primes, and the accumulated sequence                */
/* -------------------------------------------------------------------------- */
static uint32_t   *spf      = NULL;   /* smallest prime factor of i */
static idx_t      spf_limit = 0;      /* current size of spf table          */
/* primes stored in an array (realloced when new primes are found)          */
static uint32_t   *primes   = NULL;
static size_t     primes_cnt = 0;
/* accumulated sequence of valid n's                                    */
static seq_t     *seq      = NULL;
static size_t     seq_len  = 0;
static size_t     seq_cap  = 0;
/* next integer to test                                                    */
static idx_t     next_n    = 1;

/* -------------------------------------------------------------------------- */
/*  Rebuild the sieve and primes arrays up to 'new_limit'                    */
/* -------------------------------------------------------------------------- */
static void grow_spf(idx_t new_limit)
{
    /* Allocate new table (C++ needs explicit cast) */
    uint32_t *new_spf = (uint32_t*)calloc((size_t)(new_limit + 1), sizeof(uint32_t));
    if (!new_spf) abort();

    /* Build smallest‑prime‑factor sieve from scratch */
    for (idx_t i = 2; i <= new_limit; ++i) {
        if (!new_spf[i]) {
            new_spf[i] = (uint32_t)i;               /* i is prime   */
            if (i * i <= new_limit) {
                for (idx_t j = i * i; j <= new_limit; j += i)
                    if (!new_spf[j]) new_spf[j] = (uint32_t)i;
            }
        }
    }

    /* Replace old table */
    free(spf);
    spf = new_spf;
    spf_limit = new_limit;

    /* Count primes */
    size_t cnt = 0;
    for (idx_t i = 2; i <= spf_limit; ++i)
        if (new_spf[i] == (uint32_t)i) ++cnt;

    /* Reallocate primes array */
    uint32_t *new_pr = (uint32_t*)realloc(primes, cnt * sizeof(uint32_t));
    if (!new_pr) abort();
    primes = new_pr;
    primes_cnt = cnt;

    size_t k = 0;
    for (idx_t i = 2; i <= spf_limit; ++i)
        if (new_spf[i] == (uint32_t)i) primes[k++] = (uint32_t)i;
}

/* Ensure the sieve covers at least up to 'limit' */
static void ensure_spf(idx_t limit)
{
    if (spf_limit < limit) {
        /* grow roughly twice the needed limit to limit recomputations */
        idx_t new_lim = limit * 2 + 10;
        grow_spf(new_lim);
    }
}

/* -------------------------------------------------------------------------- */
/*  Sum of proper divisors:  σ(n) – n                                   */
/* -------------------------------------------------------------------------- */
static seq_t sum_proper_divisors(idx_t n)
{
    seq_t sigma = (seq_t)1;
    idx_t tmp = n;
    while (tmp > 1) {
        uint32_t p = spf[(idx_t)tmp];
        seq_t term_sum = (seq_t)1;
        seq_t p_power  = (seq_t)1;
        do {
            tmp /= p;
            p_power *= (seq_t)p;
            term_sum += p_power;
        } while (tmp % p == 0);
        sigma *= term_sum;
    }
    return sigma - (seq_t)n;          /* proper sum */
}

/* -------------------------------------------------------------------------- */
/*  Test whether x is a semiprime (exactly two prime factors, counted with multiplicity) */
/* -------------------------------------------------------------------------- */
static bool is_semiprime(seq_t x)
{
    if (x < (seq_t)4) return false;   /* 0,1,2,3 cannot be semiprime */

    int factor_cnt = 0;
    seq_t y = x;

    /* First use spf table while possible */
    while (y > 1 && factor_cnt <= 2) {
        if (y <= (seq_t)spf_limit) {
            uint32_t p = spf[(idx_t)y];
            while (y % p == 0) {
                y /= p;
                ++factor_cnt;
                if (factor_cnt > 2) return false;
            }
        } else {
            /* trial division using prime list */
            bool divided = false;
            for (size_t i = 0; i < primes_cnt; ++i) {
                uint32_t p = primes[i];
                seq_t p2 = (seq_t)p * (seq_t)p;
                if (p2 > y) break;           /* p > sqrt(y) */
                if (y % p == 0) {
                    while (y % p == 0) {
                        y /= p;
                        ++factor_cnt;
                        if (factor_cnt > 2) return false;
                    }
                    divided = true;
                    break;
                }
            }
            if (!divided) {                   /* remaining y is prime */
                ++factor_cnt;
                break;
            }
        }
    }
    return factor_cnt == 2;
}

/* -------------------------------------------------------------------------- */
/*  Append a value to the sequence array                                     */
/* -------------------------------------------------------------------------- */
static void append_value(seq_t v)
{
    if (seq_len == seq_cap) {
        size_t new_cap = (seq_cap == 0) ? 64 : seq_cap * 2;
        seq_t *tmp = (seq_t*)realloc(seq, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        seq = tmp;
        seq_cap = new_cap;
    }
    seq[seq_len++] = v;
}

/* -------------------------------------------------------------------------- */
/*  Build the sequence up to a given index (1‑based)                        */
/* -------------------------------------------------------------------------- */
static void build_upto(idx_t target)
{
    if (seq_len >= target) return;

    if (!spf) grow_spf(1000);   /* initial sieve size */

    while (seq_len < target) {
        ensure_spf(next_n);               /* guarantee spf table covers next_n */
        seq_t proper = sum_proper_divisors(next_n);
        if (is_semiprime(proper))
            append_value((seq_t)next_n);
        ++next_n;
    }
}

/* -------------------------------------------------------------------------- */
/*  Public function:  a(n) – n‑th element of the sequence                     */
/* -------------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* out of range, return 0 */
    build_upto(n);
    return seq[n - 1];
}
