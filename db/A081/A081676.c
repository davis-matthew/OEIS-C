/*
 * A081676  Largest perfect power <= n.
 * https://oeis.org/A081676
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = largest perfect power ≤ n
 * Range:     u8 n <= 27, u16 n <= 3156, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) by checking each exponent k ≥ 2 up to ⌊log₂ n⌋ and selecting the
   largest baseᵏ that does not exceed n.  Uses the provided helper
   big_pow(seq_t base, seq_t exp) for exact power arithmetic. */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/big_pow.h"

/* Find the integer r such that r^exp ≤ limit < (r+1)^exp. */
static seq_t integer_nthroot(seq_t limit, idx_t exp)
{
    seq_t lo = 1, hi = limit;
    while (lo < hi)
    {
        seq_t mid = lo + (hi - lo + 1) / 2;
        seq_t p = big_pow(mid, (seq_t)exp);
        if (p <= limit)
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

seq_t a(idx_t n)
{
    seq_t limit = (seq_t)n;
    seq_t best = 1;                 /* 1 is the smallest perfect power */

    /* Compute the largest exponent k such that 2^k ≤ n. */
    uint64_t n64 = (uint64_t)n;
    int max_k = 0;
    while ((1ULL << max_k) <= n64)
        ++max_k;
    --max_k;                         /* last shift exceeded n */

    for (int k = 2; k <= max_k; ++k)
    {
        seq_t base  = integer_nthroot(limit, (idx_t)k);
        seq_t cand  = big_pow(base, (seq_t)k);
        if (cand > best)
            best = cand;
        if (cand == limit)            /* n itself is a perfect power */
            return n;
    }

    return best;
}
