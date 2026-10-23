/*
 * A120837  a(n) = the smallest positive integer which does not occur earlier
 *          in the sequence and which is coprime to the number of positive
 *          divisors of n.
 * https://oeis.org/A120837
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the smallest positive integer not used previously in the
 *            sequence and coprime with the number of divisors of n
 * Range:     u8 n <= 138, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/gcd.h"

/* Static state for memoisation */
static idx_t   max_computed = 0;        /* highest n computed so far   */
static seq_t * perm         = NULL;     /* 0‑based sequence values    */
static size_t perm_cap     = 0;         /* capacity of perm array     */
static bool *  seen         = NULL;     /* flag array of used values  */
static size_t seen_cap     = 0;         /* capacity of seen array     */
static uint32_t * spf       = NULL;     /* smallest prime factor table*/
static size_t spf_cap       = 0;         /* capacity of spf array      */

/* Ensure the seen array can hold indices up to min_cap‑1 */
static void grow_seen (size_t min_cap)
{
    if (seen_cap >= min_cap) return;
    size_t new_cap = seen_cap ? seen_cap : 8;
    while (new_cap < min_cap) new_cap <<= 1;
    bool *new_seen = (bool *)realloc (seen, new_cap * sizeof (bool));
    if (!new_seen) abort ();
    memset (new_seen + seen_cap, 0, (new_cap - seen_cap) * sizeof (bool));
    seen      = new_seen;
    seen_cap  = new_cap;
}

/* Ensure the permutation array can hold n entries (0‑based) */
static void grow_perm (size_t n)
{
    if (perm_cap >= n) return;
    size_t new_cap = perm_cap ? perm_cap : 16;
    while (new_cap < n) new_cap <<= 1;
    seq_t *new_perm = (seq_t *)realloc (perm, new_cap * sizeof (seq_t));
    if (!new_perm) abort ();
    perm      = new_perm;
    perm_cap  = new_cap;
}

/* Recompute the SPF table up to `limit` (inclusive) */
static void recompute_spf (size_t limit)
{
    size_t new_cap = spf_cap ? spf_cap : 64;
    while (new_cap <= limit) new_cap <<= 1;
    uint32_t *new_spf = (uint32_t *)realloc (spf, new_cap * sizeof (uint32_t));
    if (!new_spf) abort ();
    memset (new_spf, 0, new_cap * sizeof (uint32_t));

    if (new_cap > 1) new_spf[1] = 1;
    for (size_t i = 2; i < new_cap; ++i) {
        if (!new_spf[i]) {                    /* i is prime   */
            new_spf[i] = (uint32_t)i;
            if (i * i < new_cap) {
                for (size_t j = i * i; j < new_cap; j += i)
                    if (!new_spf[j]) new_spf[j] = (uint32_t)i;
            }
        }
    }

    spf      = new_spf;
    spf_cap  = new_cap;
}

/* divisor count τ(k) using the SPF table (k <= spf_cap) */
static seq_t tau_of (idx_t k)
{
    uint32_t x   = (uint32_t)k;   /* k fits in 32 bits for the intended limits   */
    uint32_t tau = 1;
    while (x > 1) {
        uint32_t p   = spf[x];
        uint32_t cnt = 0;
        while (x % p == 0) { x /= p; ++cnt; }
        tau *= (cnt + 1);
    }
    return (seq_t)tau;
}

/* Main function required by the build system */
seq_t a (idx_t n)
{
    if (n == 0) return 0;                 /* undefined for n=0   */

    /* enlarge storage for new terms */
    grow_perm ((size_t)n);

    /* ensure SPF table supports up to n */
    if (spf_cap <= (size_t)n) recompute_spf ((size_t)n);

    /* produce terms up to n */
    for (idx_t k = max_computed + 1; k <= n; ++k) {
        seq_t tau = tau_of (k);

        seq_t cand = 1;
        for (;;) {
            size_t idx = (size_t)cand;
            grow_seen (idx + 1);                /* ensure we can index */
            if (!seen[idx] && gcd (cand, tau) == 1)
                break;                          /* suitable value found */
            ++cand;
        }

        perm[k - 1] = cand;                     /* store in sequence   */
        size_t idx  = (size_t)cand;
        grow_seen (idx + 1);
        seen[idx] = true;                       /* mark as used        */
    }

    max_computed = n;
    return perm[n - 1];
}
