/*
 * A062975  Half-totient function of n is not divisible by the number of
 *          divisors of n.
 * https://oeis.org/A062975
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the n-th integer k ≥ 3 such that (phi(k)/2) is not
 *            divisible by the number of divisors of k. The function computes
 *            this list by incrementally searching for qualifying numbers
 * Range:     u8 n <= 71, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   divisor_count_u64, phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/phi.h"
#include "../utils/divisor_count_u64.h"

/* seq_t and idx_t are supplied by the build system. */
seq_t a(idx_t n)
{
    /* The sequence is defined only for n ≥ 3.  For smaller n we simply
       return 0; the caller may ignore such cases. */
    if (n < 3)
        return (seq_t)0;

    /* target_index is zero‑based, because a(3) is the first element. */
    const idx_t target_index = n - 3;

    /* Static cache of already computed sequence values. */
    static seq_t *seq = NULL;
    static size_t seq_len = 0;   /* number of elements stored   */
    static size_t seq_cap = 0;   /* capacity of allocated array */

    /* Static variable holding the candidate number to test next. */
    static uint64_t cur = 3;

    /* Grow the cache until it contains the desired element. */
    while (seq_len <= target_index)
    {
        /* Find the next integer cur that satisfies the property. */
        for (;; ++cur)
        {
            /* Compute phi(cur) and its half.  phi(cur) is even for cur≥3. */
            seq_t phi_val   = phi((idx_t)cur);
            seq_t phi_half  = phi_val / 2;

            /* Count divisors of cur. */
            uint64_t tau_u64 = divisor_count_u64(cur);
            seq_t   tau_seq  = (seq_t)tau_u64;

            /* Check the conditions. */
            if ((phi_half % tau_seq) != 0)
                break;   /* cur is the next sequence element */
        }

        /* Ensure enough space in the cache. */
        if (seq_len == seq_cap)
        {
            seq_cap = seq_cap ? seq_cap * 2 : 8;
            seq     = (seq_t *)realloc(seq, seq_cap * sizeof(seq_t));
        }

        /* Store the discovered value. */
        seq[seq_len++] = (seq_t)cur;

        /* Advance to the next candidate for future searches. */
        ++cur;
    }

    return seq[target_index];
}
