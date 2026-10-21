/*
 * A074053  Numbers n such that A001414(n) = sum of composites from the
 *          smallest prime factor of n to the largest prime factor of n.
 * https://oeis.org/A074053
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the nth integer n such that A001414(n) (sum of prime
 *            factors with multiplicity) equals the sum of all composite
 *            numbers between the smallest and largest prime factor of n
 * Range:     u8 n <= 2, u16 n <= 32, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence is computed by a sieve of smallest prime factors
   and a prefix sum of composite numbers.  It proceeds lazily and
   stores all found terms so that calls can come in arbitrary order. */

#ifdef __cplusplus
/* C++ implementation (supports arbitrary-precision seq_t) */
#include <cstdint>
#include <cstddef>
#include <new>

static uint64_t* spf = nullptr;                      /* smallest prime factor table */
static seq_t*    prefix = nullptr;                 /* prefix sums of composites */
static idx_t    sieve_limit = 0;                    /* current max spf index */
static idx_t    next_candidate = 2;                 /* next integer to test */

/* sequence terms storage */
static seq_t*   entries = nullptr;
static size_t   entries_len = 0, entries_cap = 0;

/* ensure capacity for entries array */
static void ensure_entries_capacity(size_t needed)
{
    if (entries_cap < needed) {
        size_t new_cap = entries_cap ? entries_cap << 1 : 16;
        while (new_cap < needed) new_cap <<= 1;
        seq_t* tmp = new seq_t[new_cap];
        for (size_t i = 0; i < entries_len; ++i) tmp[i] = entries[i];
        delete[] entries;
        entries = tmp;
        entries_cap = new_cap;
    }
}

/* Add a new term to the sequence */
static void add_entry(seq_t val)
{
    ensure_entries_capacity(entries_len + 1);
    entries[entries_len++] = val;
}

/* Extend the sieve and prefix arrays up to new_limit */
static void extend_sieve(idx_t new_limit)
{
    if (new_limit <= sieve_limit) return;

    /* allocate new tables, value‑initialized to zero */
    uint64_t* new_spf   = new uint64_t[new_limit + 1]();
    seq_t*    new_prefix = new seq_t[new_limit + 1]();

    /* compute smallest prime factors */
    for (idx_t i = 2; i <= new_limit; ++i) {
        if (!new_spf[i]) {                      /* i is prime */
            new_spf[i] = i;
            if (i <= new_limit / i) {           /* avoid overflow */
                for (idx_t j = i * i; j <= new_limit; j += i) {
                    if (!new_spf[j]) new_spf[j] = i;
                }
            }
        }
    }

    /* compute prefix sums of composite numbers */
    seq_t cumulative = 0;
    new_prefix[0] = new_prefix[1] = 0;
    for (idx_t i = 2; i <= new_limit; ++i) {
        if (new_spf[i] != i) {                  /* composite */
            cumulative += (seq_t)i;
        }
        new_prefix[i] = cumulative;
    }

    /* free old tables */
    delete[] spf;
    delete[] prefix;

    spf = new_spf;
    prefix = new_prefix;
    sieve_limit = new_limit;
}

/* initialise static data */
static void initialise(void)
{
    extend_sieve(1);
    add_entry((seq_t)1);         /* a(1) = 1 by convention */
    next_candidate = 2;
}

seq_t a(idx_t n)
{
    if (!entries) initialise();

    while (entries_len < n) {
        if (sieve_limit < next_candidate) extend_sieve(next_candidate);

        for (idx_t i = next_candidate; i <= sieve_limit; ++i) {
            if (i <= 1) continue;          /* skip 0,1 */

            /* factorisation using stored smallest prime factors */
            seq_t sum_pf = 0;
            idx_t  m = i;
            idx_t  lpf = 0, gpf = 0;
            while (m > 1) {
                idx_t p = spf[m];
                if (!lpf) lpf = p;
                if (p > gpf) gpf = p;
                sum_pf += (seq_t)p;
                m /= p;
            }

            /* sum of composites between lpf and gpf (inclusive) */
            seq_t comp_sum = prefix[gpf] - prefix[lpf - 1];
            if (sum_pf == comp_sum) add_entry((seq_t)i);
        }

        next_candidate = sieve_limit + 1;

        /* if still insufficient, extend the sieve further */
        if (entries_len < n) {
            idx_t new_limit = sieve_limit * 2 + 1000;
            extend_sieve(new_limit);
        }
    }

    return entries[n - 1];
}

#else
/* C implementation (seq_t is a typedef to an unsigned builtin type) */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* static data */
static uint64_t* spf = NULL;              /* smallest prime factor table */
static seq_t*    prefix = NULL;          /* prefix sums of composites */
static idx_t    sieve_limit = 0;          /* current max spf index */
static idx_t    next_candidate = 2;       /* next integer to test */

static seq_t*   entries = NULL;
static size_t   entries_len = 0, entries_cap = 0;

/* ensure capacity for entries array */
static void ensure_entries_capacity(size_t needed)
{
    if (entries_cap < needed) {
        size_t new_cap = entries_cap ? entries_cap << 1 : 16;
        while (new_cap < needed) new_cap <<= 1;
        seq_t* tmp = (seq_t*)realloc(entries, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        entries = tmp;
        entries_cap = new_cap;
    }
}

/* Add a new term to the sequence */
static void add_entry(seq_t val)
{
    ensure_entries_capacity(entries_len + 1);
    entries[entries_len++] = val;
}

/* Extend the sieve and prefix arrays up to new_limit */
static void extend_sieve(idx_t new_limit)
{
    if (new_limit <= sieve_limit) return;

    uint64_t* new_spf   = (uint64_t*)calloc((size_t)new_limit + 1, sizeof(uint64_t));
    seq_t*    new_prefix = (seq_t*)calloc((size_t)new_limit + 1, sizeof(seq_t));
    if (!new_spf || !new_prefix) abort();

    /* compute smallest prime factors */
    for (idx_t i = 2; i <= new_limit; ++i) {
        if (!new_spf[i]) {                      /* i is prime */
            new_spf[i] = i;
            if (i <= new_limit / i) {           /* avoid overflow */
                for (idx_t j = i * i; j <= new_limit; j += i) {
                    if (!new_spf[j]) new_spf[j] = i;
                }
            }
        }
    }

    /* compute prefix sums of composite numbers */
    seq_t cumulative = 0;
    new_prefix[0] = new_prefix[1] = 0;
    for (idx_t i = 2; i <= new_limit; ++i) {
        if (new_spf[i] != i) {                  /* composite */
            cumulative += (seq_t)i;
        }
        new_prefix[i] = cumulative;
    }

    /* free old tables */
    free(spf);
    free(prefix);

    spf = new_spf;
    prefix = new_prefix;
    sieve_limit = new_limit;
}

/* initialise static data */
static void initialise(void)
{
    extend_sieve(1);
    add_entry((seq_t)1);         /* a(1) = 1 by convention */
    next_candidate = 2;
}

seq_t a(idx_t n)
{
    if (!entries) initialise();

    while (entries_len < n) {
        if (sieve_limit < next_candidate) extend_sieve(next_candidate);

        for (idx_t i = next_candidate; i <= sieve_limit; ++i) {
            if (i <= 1) continue;          /* skip 0,1 */

            /* factorisation using stored smallest prime factors */
            seq_t sum_pf = 0;
            idx_t  m = i;
            idx_t  lpf = 0, gpf = 0;
            while (m > 1) {
                idx_t p = spf[m];
                if (!lpf) lpf = p;
                if (p > gpf) gpf = p;
                sum_pf += (seq_t)p;
                m /= p;
            }

            /* sum of composites between lpf and gpf (inclusive) */
            seq_t comp_sum = prefix[gpf] - prefix[lpf - 1];
            if (sum_pf == comp_sum) add_entry((seq_t)i);
        }

        next_candidate = sieve_limit + 1;

        /* if still insufficient, extend the sieve further */
        if (entries_len < n) {
            idx_t new_limit = sieve_limit * 2 + 1000;
            extend_sieve(new_limit);
        }
    }

    return entries[n - 1];
}
#endif
