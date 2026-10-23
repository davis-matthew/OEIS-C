/*
 * A161845  a(n) = A002808(n)^A141468(n).
 * https://oeis.org/A161845
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A002808(n)^A141468(n) (n ≥ 1)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 10, u128 n <= 14,
 *            big any n
 * Helpers:   big_pow, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime.h"
#include "../utils/big_pow.h"

/* --------- cache for nth composite numbers (A002808) --------- */
static uint64_t *composites  = NULL;   /* 1‑based index: composites[0] = 4, composites[1] = 6, … */
static size_t   composites_cnt = 0;
static uint64_t comp_last     = 3;      /* last tested candidate */

/* --------- cache for nth nonprime numbers (A141468) --------- */
static uint64_t *nonprimes   = NULL;   /* 1‑based index: nonprimes[0] = 0, nonprimes[1] = 1, … */
static size_t   nonprimes_cnt = 0;
static uint64_t np_last       = 0;      /* last tested candidate */

/* -------------------------------------------------------------------------- */
/* Ensure at least `need` composite numbers are stored in `composites`. */
/* -------------------------------------------------------------------------- */
static void ensure_composites(size_t need)
{
    if (composites_cnt >= need)
        return;

    size_t new_cap = composites_cnt ? composites_cnt * 2 : 64;
    if (new_cap < need) new_cap = need;

    uint64_t *tmp = (uint64_t *)realloc(composites, new_cap * sizeof(uint64_t));
    if (!tmp) abort();            /* allocation failed */
    composites = tmp;

    while (composites_cnt < need) {
        comp_last++;
        if (comp_last < 4)          /* 2 and 3 are primes */
            continue;
        if (!is_prime(comp_last))
            composites[composites_cnt++] = comp_last;
    }
}

/* -------------------------------------------------------------------------- */
/* Ensure at least `need` nonprime numbers are stored in `nonprimes`. */
/* -------------------------------------------------------------------------- */
static void ensure_nonprimes(size_t need)
{
    if (nonprimes_cnt >= need)
        return;

    size_t new_cap = nonprimes_cnt ? nonprimes_cnt * 2 : 64;
    if (new_cap < need) new_cap = need;

    uint64_t *tmp = (uint64_t *)realloc(nonprimes, new_cap * sizeof(uint64_t));
    if (!tmp) abort();
    nonprimes = tmp;

    while (nonprimes_cnt < need) {
        if (!is_prime(np_last))
            nonprimes[nonprimes_cnt++] = np_last;
        np_last++;
    }
}

/* -------------------------------------------------------------------------- */
/* Compute the n‑th term of the sequence.                                    */
/* -------------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* n ≥ 1 by OEIS offset */
    size_t sz = (size_t)n;          /* safe: n is small for normal usage */
    ensure_composites(sz);
    ensure_nonprimes(sz);

    seq_t base = (seq_t)composites[sz - 1];
    seq_t exp  = (seq_t)nonprimes[sz - 1];
    return big_pow(base, exp);
}
