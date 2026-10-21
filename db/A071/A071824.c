/*
 * A071824  Number of integers <= n whose largest prime factor is of the form
 *          4*k+1.
 * https://oeis.org/A071824
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = the number of integers 1 ≤ k ≤ n whose largest prime
 *            factor is congruent to 1 modulo 4
 * Range:     u8 n <= 578, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The types idx_t and seq_t are supplied by the build.
   They should not be defined here. */

#include <stdlib.h>
#include <stdint.h>

static uint32_t   *lpf   = NULL;   /* largest prime factor of each integer */
static seq_t     *pref  = NULL;   /* prefix counts up to each n */
static idx_t      limit = 0;     /* the largest n for which the tables are valid */

/* Ensure that tables are valid up to n.  Rebuild everything if n exceeds the
   current limit.  Clearing and reallocating is inexpensive for the small
   limits (≈10 000) requested. */
static void
ensure_filled(idx_t n)
{
    if (n <= limit)
        return;

    /* Free previous allocations if any.  It is safe to free(NULL). */
    free(lpf);
    free(pref);

    /* Allocate new tables with indices 0 … n inclusive. */
    size_t sz = (size_t)(n + 1);
    lpf  = (uint32_t *)calloc(sz, sizeof(uint32_t));
    pref = (seq_t   *)calloc(sz, sizeof(seq_t));
    if ((lpf == NULL) || (pref == NULL))
        abort();                       /* out of memory */

    /* Sieve to fill largest prime factor table. */
    for (idx_t i = 2; i <= n; ++i) {
        if (lpf[i] == 0) {             /* i is prime */
            for (idx_t m = i; m <= n; m += i)
                lpf[m] = (uint32_t)i;  /* overwrite – largest prime wins */
        }
    }

    /* Prefix sums: count 1 when lpf[i] ≡ 1 (mod 4) and i ≥ 2. */
    pref[0] = 0;
    for (idx_t i = 1; i <= n; ++i) {
        seq_t inc = 0;
        if (i >= 2 && (lpf[i] & 3U) == 1U)
            inc = 1;
        pref[i] = pref[i-1] + inc;
    }

    limit = n;
}

/* OEIS sequence A071824.  Undefined behaviour for n < 1 is allowed. */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;           /* arbitrary for out‑of‑range indices */

    ensure_filled(n);
    return pref[n];
}
