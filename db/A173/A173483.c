/*
 * A173483  a(n) = the largest (n+1)-digit number ending in n zeros that is
 *          divisible by n, else 0.
 * https://oeis.org/A173483
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 19, u128 n <= 20,
 *            big any n
 * Helpers:   mod_pow, mul_small, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computed algorithm: For a given n ≥ 1, find the largest digit d (1 ≤ d ≤ 9)
 * such that d*10ⁿ is divisible by n.  The resulting (n+1)-digit number is
 * d*10ⁿ; if no such d exists, return 0. */
#include <stdbool.h>
#include <stdint.h>
#include "../utils/mod_pow.h"
#include "../utils/mul_small.h"
#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    /* OEIS offset starts at 1; no definition below 1 */
    if (n < 1ULL)
        return (seq_t)0;

    /* Compute 10^n mod n once; thereafter the number is small. */
    seq_t ten_mod = mod_pow((seq_t)10, n, (seq_t)n);   /* 10ⁿ mod n */

    /* Search for the largest suitable digit d in 9..1. */
    for (unsigned int d = 9; d >= 1; --d)
    {
        seq_t prod = mul_small(ten_mod, d);            /* (10ⁿ mod n) * d */
        if ((prod % (seq_t)n) == (seq_t)0)
        {
            /* Full number is d * 10ⁿ. */
            seq_t pow10n = pow10(n);                   /* 10ⁿ itself */
            return mul_small(pow10n, d);
        }
    }

    /* No digit found → sequence entry is 0. */
    return (seq_t)0;
}
