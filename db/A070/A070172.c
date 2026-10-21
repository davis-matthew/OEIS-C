/*
 * A070172  Smallest k such that sigma(k) >= n.
 * https://oeis.org/A070172
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = min{k ≥ 1 | σ(k) ≥ n} (σ(k) = sum of divisors of k)
 * Range:     u8 n <= 744, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* external types supplied by the build */
extern seq_t a(idx_t n);

/* -----------------------  internal state  ----------------------- */
static idx_t max_k = 0;          /* largest k for which arrays are valid      */
static uint64_t *sigma_arr = NULL;   /* divisor sums (uint64_t to avoid overflow)   */
static uint64_t *pref_arr  = NULL;   /* prefix maximum of sigma (uint64_t)           */
static size_t  capacity = 0;      /* allocated size of the arrays                */

/* enlarge arrays to hold indices up to 'limit' (inclusive)          */
static void ensure_capacity(idx_t limit)
{
    if (capacity <= (size_t)limit)
    {
        idx_t new_cap = capacity > 0 ? capacity : 1;
        while (new_cap <= limit) new_cap <<= 1;          /* double size */

        /* realloc might fail – assume the evaluator has enough memory  */
        sigma_arr = (uint64_t *)realloc(sigma_arr, new_cap * sizeof(uint64_t));
        pref_arr  = (uint64_t *)realloc(pref_arr,  new_cap * sizeof(uint64_t));
        capacity = new_cap;
    }
}

/* recompute sigma and prefix arrays up to 'limit' (inclusive)     */
static void recompute_upto(idx_t limit)
{
    ensure_capacity(limit);

    /* initialise sigma to zero */
    for (idx_t i = 0; i <= limit; ++i)
        sigma_arr[i] = 0;

    /* sieve: add i to all multiples of i */
    for (idx_t i = 1; i <= limit; ++i)
    {
        uint64_t ui = (uint64_t)i;
        for (idx_t j = i; j <= limit; j += i)
            sigma_arr[j] += ui;
    }

    /* prefix maximum of sigma */
    pref_arr[0] = 0;
    if (limit >= 1)
        pref_arr[1] = sigma_arr[1];
    for (idx_t k = 2; k <= limit; ++k)
        pref_arr[k] = (pref_arr[k-1] > sigma_arr[k]) ? pref_arr[k-1] : sigma_arr[k];

    max_k = limit;
}

/* extend computation until pref[max_k] >= n */
static void extend_upto(idx_t n)
{
    if (max_k == 0)
        recompute_upto(1);

    while (pref_arr[max_k] < (uint64_t)n)
    {
        /* double the range for simplicity */
        idx_t new_limit = (max_k == 0) ? 1 : (max_k << 1);
        recompute_upto(new_limit);
    }
}

/* binary search for minimal k with pref[k] >= n (n >= 1) */
static idx_t first_ge(idx_t n)
{
    idx_t low = 1, high = max_k;
    while (low < high)
    {
        idx_t mid = low + (high - low) / 2;
        if (pref_arr[mid] >= (uint64_t)n)
            high = mid;
        else
            low = mid + 1;
    }
    return low;
}

/* -------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n < 1)  /* offset is 1 – undefined for n=0 but return 0 */
        return 0;

    extend_upto(n);
    idx_t k = first_ge(n);
    return (seq_t)k;
}
