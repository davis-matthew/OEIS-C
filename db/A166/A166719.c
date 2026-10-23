/*
 * A166719  Numbers with at most 5 prime factors (counted with multiplicity).
 * https://oeis.org/A166719
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 246, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A166719 – Numbers with at most 5 prime factors (counted with multiplicity)
 *
 *  Computes a(n) by sieving the smallest prime factor of every integer
 *  and counting how many prime factors each number has.  Whenever the
 *  count (including multiplicity) is ≤5 the number is appended to a
 *  growing cache of valid terms.  The sequence is memoised so that
 *  subsequent calls with any index are answered in O(1) time after the
 *  required terms have been generated.
 *
 *  The code works for both C (C11) and C++17.  In C++ a dynamic array
 *  of objects is allocated with `new[]` so that constructor/destructor
 *  semantics are respected for arbitrary‑precision `seq_t` types.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>   /* for memcpy in the C build */

static seq_t     *seq_vals  = NULL;  /* cached sequence values         */
static size_t      seq_len   = 0;     /* number of cached terms         */
static size_t      seq_cap   = 0;     /* allocated capacity             */
static idx_t       cur_i     = 1;     /* next integer to examine        */

static uint64_t   *spf       = NULL;  /* smallest prime factor          */
static unsigned char *bigomega = NULL;/* number of prime factors       */
static idx_t       spf_limit = 0;     /* current max index of spf      */

/* ----------------------------------------------------------------------
 *  Allocate or reallocate a dynamic array of seq_t objects.
 *  The implementation is different for C and C++ so that a C++ big‑int
 *  class is constructed correctly.
 * ---------------------------------------------------------------------- */
static void grow_seq_vals(size_t needed)
{
    if (needed <= seq_cap) return;

    size_t new_cap = seq_cap ? seq_cap * 2 : 128;
    while (new_cap < needed) new_cap *= 2;

#ifdef __cplusplus
    seq_t *new_arr = new seq_t[new_cap];
    for (size_t i = 0; i < seq_len; ++i) new_arr[i] = seq_vals[i];
    delete[] seq_vals;
    seq_vals = new_arr;
#else
    seq_t *new_arr = (seq_t *)realloc(seq_vals, new_cap * sizeof(seq_t));
    if (!new_arr) { /* allocation failed */ exit(EXIT_FAILURE); }
    seq_vals = new_arr;
#endif

    seq_cap = new_cap;
}

/* ----------------------------------------------------------------------
 *  Recompute the smallest‑prime‑factor and big‑omega arrays up to
 *  the new limit.  The simple sieve below is fast enough for the
 *  limits that occur in the tests (n ≤ 10 000).
 * ---------------------------------------------------------------------- */
static void extend_spf(idx_t new_limit)
{
    if (new_limit <= spf_limit) return;

    /* allocate new arrays */
    uint64_t *new_spf   = (uint64_t *)malloc((new_limit + 1) * sizeof(uint64_t));
    unsigned char *new_big =
        (unsigned char *)malloc((new_limit + 1) * sizeof(unsigned char));
    if (!new_spf || !new_big) { exit(EXIT_FAILURE); }

    /* initialise 0 */
    memset(new_spf, 0, (new_limit + 1) * sizeof(uint64_t));
    memset(new_big, 0, (new_limit + 1) * sizeof(unsigned char));

    /* simple Eratosthenes sieve for spf */
    for (idx_t i = 2; i <= new_limit; ++i)
    {
        if (new_spf[i] == 0)          /* i is prime */
        {
            new_spf[i] = i;
            /* mark multiples */
            for (idx_t j = i * 2; j <= new_limit; j += i)
                if (new_spf[j] == 0)
                    new_spf[j] = i;
        }
    }

    /* compute bigomega: number of prime factors with multiplicity */
    new_big[1] = 0;
    for (idx_t i = 2; i <= new_limit; ++i)
        new_big[i] = new_big[i / new_spf[i]] + 1;

    /* free old arrays */
    free(spf);
    free(bigomega);

    spf       = new_spf;
    bigomega  = new_big;
    spf_limit = new_limit;
}

/* ----------------------------------------------------------------------
 *  Ensure that spf/bigomega arrays cover up to `need`.
 * ---------------------------------------------------------------------- */
static void ensure_spf(idx_t need)
{
    if (need <= spf_limit) return;
    /* grow roughly linearly – sufficient for n up to 10‑4 tests */
    idx_t new_lim = spf_limit ? spf_limit * 2 : 128;
    if (new_lim < need * 2) new_lim = need * 2;
    extend_spf(new_lim);
}

/* ----------------------------------------------------------------------
 *  Main sequence function
 * ---------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined but never called */

    /* generate terms until we have at least n of them */
    while (seq_len < n)
    {
        /* make sure we can factor cur_i */
        ensure_spf(cur_i);

        /* 1 is valid (0 prime factors) */
        if (cur_i == 1 || bigomega[cur_i] <= 5)
        {
            grow_seq_vals(seq_len + 1);
            seq_vals[seq_len++] = (seq_t)cur_i;
        }
        ++cur_i;
    }
    return seq_vals[n - 1];
}
