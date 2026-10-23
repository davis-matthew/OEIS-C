/*
 * A196941  a(n) is the minimum prime (or 1) needed to write integer n into
 *          the form n = a + b such that all prime factors of a and b are
 *          smaller or equal to a(n).
 * https://oeis.org/A196941
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS A196941: 
   For n≥2, a(n) is the smallest integer k such that
   n can be written as a+b with all prime factors of a and b
   ≤ k.  This implementation computes it by
   • generating the largest prime factor of every integer up to n
   • scanning all splits a+b=n (a≤b) and taking the minimum
   of the maximal prime factor in the split. */
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static uint64_t *lpf_arr = NULL;    /* largest prime factor table */
static size_t lpf_cap  = 0;          /* current array capacity */

static void ensure_lpf(size_t n)
{
    if (n < 2) return;

    /* enlarge storage if required */
    if (lpf_arr == NULL || n + 1 > lpf_cap) {
        size_t new_cap = n + 1;
        uint64_t *tmp = (uint64_t *)realloc(lpf_arr, new_cap * sizeof(uint64_t));
        if (!tmp) abort();               /* out of memory */
        lpf_arr = tmp;
        lpf_cap = new_cap;
    }

    /* initialise with zeros and set lpf[1] = 1 */
    memset(lpf_arr, 0, lpf_cap * sizeof(uint64_t));
    lpf_arr[1] = 1;

    /* sieve for largest prime factor: */
    for (size_t i = 2; i < lpf_cap; ++i) {
        if (lpf_arr[i] == 0) {          /* i is prime */
            for (size_t j = i; j < lpf_cap; j += i)
                lpf_arr[j] = i;          /* overwritten later by larger primes */
        }
    }
}

seq_t a(idx_t n)
{
    if (n < 2) return 0;               /* behaviour below offset is irrelevant */

    ensure_lpf((size_t)n);

    uint64_t best = UINT64_MAX;
    for (uint64_t a1 = 1; a1 <= n / 2; ++a1) {
        uint64_t b1 = n - a1;
        uint64_t la = lpf_arr[a1];
        uint64_t lb = lpf_arr[b1];
        uint64_t cand = la > lb ? la : lb;
        if (cand < best) {
            best = cand;
            if (best == 1) break;      /* cannot get better than 1 */
        }
    }

    return (seq_t)best;
}
