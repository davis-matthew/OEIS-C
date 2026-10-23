/*
 * A109939  Largest k-digit prime == 1 (mod n) where k is the number of digits
 *          in n, or 0 if no such prime exists.
 * https://oeis.org/A109939
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   for each n ≥ 1, let k be the number of decimal digits of n. a(n)
 *            is the largest k‑digit prime p such that p ≡ 1 (mod n). If no
 *            such prime exists, a(n) = 0
 * Range:     u8 n <= 99, u16 n <= 9999, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   count_digits, is_prime, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/count_digits.h"
#include "../utils/pow10.h"
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    /* n ≥ 1 by OEIS offset.  */
    if (n <= 1)                /* 1 gives no prime ≡ 1 (mod 1) */
        return (seq_t)0;

    /* number of decimal digits of n */
    int k = count_digits(n);

    /* bounds for k‑digit numbers: 10^(k-1) ≤ p < 10^k */
    seq_t low_seq  = pow10(k - 1);   /* 1 for k=1 */
    seq_t high_seq = pow10(k);      /* 10 for k=1 */

    /* Convert to uint64_t.  The values are tiny for the test range,
       so the cast is safe. */
    uint64_t low  = (uint64_t)low_seq;
    uint64_t high = (uint64_t)high_seq;
    if (high == 0)            /* overflow guard for very large k */
        return (seq_t)0;
    high -= 1;                /* inclusive upper bound for k‑digit numbers */

    uint64_t n64 = (uint64_t)n;

    /* t such that candidate = 1 + t*n is k‑digit.
       t_min = ceil((low – 1) / n)   → (low + n – 2)/n
       t_max = floor((high – 1) / n) → (high – 1)/n   */
    uint64_t t_min = (low + n64 - 2) / n64;
    uint64_t t_max = (high - 1) / n64;

    if (t_min > t_max)
        return (seq_t)0;    /* no candidate at all */

    for (uint64_t t = t_max; ; --t) {
        uint64_t candidate = 1 + t * n64;
        if (is_prime(candidate))
            return (seq_t)candidate;
        if (t == t_min)
            break;          /* reached the lower bound */
    }
    return (seq_t)0;            /* no prime found */
}
