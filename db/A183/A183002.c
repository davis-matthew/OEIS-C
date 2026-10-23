/*
 * A183002  a(n) is the total number of noncentral divisors in all positive
 *          integers <= n.
 * https://oeis.org/A183002
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=1..n} (tau(k)-2 + (tau(k) mod 2)), * where tau(k)
 *            is the divisor-count function A000005
 * Range:     u8 n <= 63, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   divisor_count_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>          /* malloc, free */
#include "../utils/divisor_count_u64.h"  /* uint64_t divisor_count_u64(uint64_t) */

/* cached prefix sums of the non‑central divisor counts.
 * pref[0] is always 0; pref[i] equals a(i). */
static seq_t *pref = NULL;   /* static cache for arbitrary‑precision types */
static idx_t cur = 0;        /* maximum index for which pref has been filled */

/* extend the cache so that pref[n] is valid for all n <= target. */
static void extend_cache(idx_t target)
{
    if (target <= cur)          /* already computed */
        return;

    /* allocate a new array large enough for target+1 elements */
    size_t new_len = (size_t)(target + 1);
    seq_t *new_pref = (seq_t *)malloc(new_len * sizeof(seq_t));
    if (!new_pref)
        exit(EXIT_FAILURE);    /* memory allocation failure */

    /* copy existing data if any, else just initialize index 0 */
    if (pref) {
        size_t copied = (size_t)(cur + 1);
        for (size_t i = 0; i < copied; ++i)
            new_pref[i] = pref[i];
    }
    new_pref[0] = (seq_t)0;   /* sentinel */

    /* compute missing values from cur+1 up to target */
    for (idx_t k = cur + 1; k <= target; ++k) {
        /* compute tau(k) using the provided routine */
        uint64_t tau64 = divisor_count_u64(k);
        seq_t tau_k = (seq_t)tau64;

        /* contribution: tau_k-2 if tau_k even, tau_k-1 if tau_k odd */
        seq_t contrib;
        if (tau_k & (seq_t)1)
            contrib = tau_k - (seq_t)1;  /* tau_k odd */
        else
            contrib = tau_k - (seq_t)2;  /* tau_k even */

        new_pref[k] = new_pref[k - 1] + contrib;
    }

    /* replace old array and update current index */
    free(pref);
    pref = new_pref;
    cur = target;
}

/* public function required by the exercise */
seq_t a(idx_t n)
{
    if (n < 1)        /* undefined for n=0, return 0 safely */
        return (seq_t)0;

    extend_cache(n);
    return pref[n];
}
