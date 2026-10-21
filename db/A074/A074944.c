/*
 * A074944  Number of k with 1 <= k <= n such that gcd(n,k) = tau(k), where
 *          tau is A000005, number of divisors function.
 * https://oeis.org/A074944
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = |{ k : 1 ≤ k ≤ n AND gcd(n,k) = τ(k) }|. τ(k) is the
 *            number of divisors of k. This code builds the divisor‑count
 *            array up to n for each call, resetting it before recomputing. It
 *            works for any seq_t (arbitrary precision or unsigned integer)
 * Range:     u8 n <= 5687, u16 n <= 8827, u32 n <= 8827, u64 n <= 8827,
 *            u128 n <= 8827, big any n
 * Helpers:   gcd_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/gcd_uint64.h"

/* ---------- local gcd for uint64_t --------------------------------- */

/* ---------- divisor‑count array ------------------------------------- */
static uint64_t *tau_arr = NULL;   /* tau_arr[i] = τ(i) for 1 ≤ i ≤ tau_max */
static size_t tau_max  = 0;        /* current maximum i with valid values */

/* Build τ(k) for all 1 ≤ k ≤ n.  All entries up to n are set to 0
   before the recomputation, so previous values do not accumulate. */
static void compute_tau(size_t n)
{
    /* Allocate or grow the array if necessary. */
    if (tau_arr == NULL) {
        tau_arr = (uint64_t *)calloc(n + 1, sizeof(uint64_t));
        if (!tau_arr) abort();
        tau_max = 0;
    } else if (n > tau_max) {
        uint64_t *new_arr = (uint64_t *)realloc(tau_arr,
                                                (n + 1) * sizeof(uint64_t));
        if (!new_arr) abort();
        tau_arr = new_arr;
        /* Initialise new slots to 0. */
        for (size_t i = tau_max + 1; i <= n; ++i)
            tau_arr[i] = 0;
        tau_max = n;
    }

    /* Zero all entries up to n (including any that were already set). */
    for (size_t i = 1; i <= n; ++i)
        tau_arr[i] = 0;

    /* Recompute τ(k) by a divisor sieve. */
    for (size_t i = 1; i <= n; ++i)
        for (size_t j = i; j <= n; j += i)
            ++tau_arr[j];
}

/* ---------- OEIS function ------------------------------------------ */
seq_t a(idx_t n)
{
    if (n < (idx_t)1)                      /* undefined for n = 0 */
        return (seq_t)0;

    size_t sn = (size_t)n;
    compute_tau(sn);

    uint64_t count = 0;
    uint64_t nn = (uint64_t)n;

    for (size_t k = 1; k <= sn; ++k)
        if (gcd_uint64(nn, (uint64_t)k) == tau_arr[k])
            ++count;

    return (seq_t)count;
}
