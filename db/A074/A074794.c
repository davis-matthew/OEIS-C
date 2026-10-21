/*
 * A074794  Number of numbers k <= n such that tau(k) == 1 (mod 3) where
 *          tau(k) = A000005(k) is the number of divisors of k.
 * https://oeis.org/A074794
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = # { k ≤ n | τ(k) ≡ 1 (mod 3) } * τ(k) is the number of
 *            divisors of k. * The function builds a cumulative table up to
 *            the largest n requested * and reuses it for subsequent calls. No
 *            external helper headers are * required. The algorithm runs in
 *            O(n log n) for the largest n, * which is comfo...
 * Range:     u8 n <= 705, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* static storage for the prefix array and divisor counts */
static seq_t *pref      = NULL;   /* pref[i] = a(i) */
static uint32_t *divcnt = NULL;   /* divisor count for each i */
static idx_t cur_max    = 0;       /* current highest precomputed index */

/* Resize the storage to cover at least 'need' elements */
static void grow(idx_t need)
{
    idx_t new_max = need;
    /* enlarge only if necessary to avoid frequent reallocs */
    if (need <= cur_max) return;
    /* allocate new arrays */
    divcnt = (uint32_t *)realloc(divcnt, (new_max + 1) * sizeof(uint32_t));
    pref   = (seq_t   *)realloc(pref  , (new_max + 1) * sizeof(seq_t));
    /* clear new slots (they may contain garbage) */
    for (idx_t i = cur_max + 1; i <= new_max; ++i)
        divcnt[i] = 0, pref[i] = 0;
    cur_max = new_max;
}

/* Recompute divisor counts and prefix sums up to 'max_n' */
static void recompute(idx_t max_n)
{
    /* ensure arrays large enough */
    grow(max_n);

    /* Zero divisor counts up to max_n */
    for (idx_t i = 1; i <= max_n; ++i)
        divcnt[i] = 0;

    /* Simple sieve to count divisors */
    for (idx_t d = 1; d <= max_n; ++d) {
        for (idx_t k = d; k <= max_n; k += d)
            ++divcnt[k];
    }

    /* Build prefix sums for the desired congruence */
    pref[0] = 0;
    for (idx_t i = 1; i <= max_n; ++i) {
        pref[i] = pref[i - 1] + ((divcnt[i] % 3u) == 1u ? 1 : 0);
    }
}

/* Main exported function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* offset is 1; preserve for safety */

    /* On first call, or if a larger n is requested, recompute */
    if (cur_max < n) recompute(n);

    /* Return the already computed prefix value */
    return pref[n];
}
