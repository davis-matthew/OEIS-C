/*
 * A236856  Partial sums of A003418 starting summing from A003418(1), with
 *          a(0) = 0.
 * https://oeis.org/A236856
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=1..n} lcm(1,2,…,k)
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 22, u64 n <= 43, u128 n <= 88,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
/* Utility for computing gcd on seq_t values */
#include "../utils/gcd.h"

/* Compute the n-th term of A236856.
 *  a(0) = 0
 *  a(n) = a(n‑1) + LCM(1,2,…,n)  for n ≥ 1
 *
 *  seq_t is an arbitrary integer type (possibly big‑int).
 *  idx_t is an unsigned integer type (e.g., uint64_t).
 */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;

    seq_t sum      = (seq_t)0;   /* running sum of LCMs   */
    seq_t lcm_cur  = (seq_t)1;   /* current LCM(1..k)    */

    for (idx_t k = 1; k <= n; ++k) {
        seq_t k_seq = (seq_t)k;          /* cast index to seq_t   */
        seq_t g     = gcd(lcm_cur, k_seq);    /* g = gcd(lcm_cur, k)  */
        lcm_cur = (lcm_cur / g) * k_seq;     /* lcm_cur = lcm(lcm_cur, k) */
        sum = sum + lcm_cur;                     /* accumulate sum      */
    }

    return sum;
}
