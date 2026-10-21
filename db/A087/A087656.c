/*
 * A087656  Let f be defined on the rationals by f(p/q)
 *          =(p+1)/(q+1)=p_{1}/q_{1} where (p_{1},q_{1})=1. Let
 *          f^k(p/q)=p_{k}/q_{k} where (p_{k},q_{k})=1. Sequence gives least k
 *          such that p_{k}-q_{k} = 1 starting at n.
 * https://oeis.org/A087656
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of iterations of f(p/q) = (p+1)/(q+1) (reduced)
 *            needed to reach a fraction with numerator – denominator = 1,
 *            starting from n/1. Recurrence: for n ≥ 3 let k = n‑1 and p =
 *            least prime factor of k; then a(n) = a(k/p + 1) + (p‑1). Base
 *            case: a(3) = 1. The algorithm iteratively applies t...
 * Range:     u8 n <= 84, u16 n <= 84, u32 n <= 84, u64 n <= 84, u128 n <= 84,
 *            big any n
 * Helpers:   least_prime_factor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/least_prime_factor.h"

seq_t a(idx_t n)
{
    if (n < 3) return 0;   /* out of defined range – undefined by OEIS */
    seq_t steps = 0;
    while (n > 2) {
        uint64_t k = (uint64_t)n - 1;          /* n >= 3, so k >= 2 */
        uint64_t p = least_prime_factor(k);   /* least prime factor of k */
        steps += (seq_t)(p - 1);               /* add the p‑1 intermediate steps */
        n = (uint64_t)(k / p) + 1;             /* move to the next reduced fraction */
    }
    return steps;
}
