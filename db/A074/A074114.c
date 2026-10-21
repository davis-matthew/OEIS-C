/*
 * A074114  Largest n-digit number of the form p^a*q^b... with the maximum
 *          value of a+b+.... where p, q etc. are primes.
 * https://oeis.org/A074114
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = 2^a * 3^b (b ∈ {0,1}) where a is chosen so that 2^a ≤
 *            10^n − 1 and 2^a * 3 ≤ 10^n − 1 whenever possible. This is
 *            equivalent to: Let a0 = ⌊log₂(10ⁿ)⌋. If 2^a0 < (2/3)·10ⁿ then
 *            a(n) = 2^a0 · 3, else a(n) = 2^a0
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 20,
 *            big any n
 * Helpers:   max_pow2_leq, pow10_ui, pow2_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/max_pow2_leq.h"
#include "../utils/pow10_ui.h"
#include "../utils/pow2_seq.h"

/* Main sequence function.  */
seq_t a(idx_t n)
{
    /* Compute 10^n - 1 as the upper bound for the n‑digit number. */
    seq_t limit = pow10_ui(n);
    limit -= 1;

    /* Largest power of 2 not exceeding limit. */
    idx_t a0 = max_pow2_leq(&limit);

    /* Largest power of 2 not exceeding floor(limit/3), i.e. 2^a for 2^a * 3 ≤ limit. */
    seq_t limit_div3 = limit / 3;
    idx_t a1 = max_pow2_leq(&limit_div3);

    /* Construct the two candidate values. */
    seq_t cand1 = pow2_seq(a0);          /* 2^a0 */
    seq_t cand2 = pow2_seq(a1) * 3;      /* 2^a1 * 3 */

    idx_t sum1 = a0;              /* a + b for cand1  (b=0) */
    idx_t sum2 = a1 + 1;          /* a + b for cand2  (b=1) */

    if (sum2 > sum1)
        return cand2;
    else if (sum2 < sum1)
        return cand1;
    else
        return (cand2 > cand1) ? cand2 : cand1;   /* tie: choose the larger value */
}
