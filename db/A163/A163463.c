/*
 * A163463  a(1)=1. For n >= 2: If a(n-1) is coprime to n, then a(n) = the
 *          smallest integer > a(n-1) that is coprime to n. If a(n-1) is not
 *          coprime to n, then a(n) = the smallest integer > a(n-1) that is
 *          not coprime to n.
 * https://oeis.org/A163463
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 141, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function implements the recurrence from OEIS A163463
   a(1) = 1
   for n ≥ 2:
       if gcd(a(n‑1), n) == 1  then a(n) = the smallest integer > a(n‑1) coprime to n
       else                      a(n) = the smallest integer > a(n‑1) not coprime to n   */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/gcd.h"   /* provides: seq_t gcd(seq_t a, seq_t b) */

seq_t a(idx_t n)
{
    if (n == 0)                 /* n must be ≥ 1, but guard against 0 */
        return (seq_t)0;

    seq_t prev = (seq_t)1;      /* a(1) = 1 */
    if (n == 1)
        return prev;

    for (idx_t i = 2; i <= n; ++i) {
        seq_t g = gcd(prev, (seq_t)i);
        seq_t cand = prev + 1;
        if (g == (seq_t)1) {
            while (gcd(cand, (seq_t)i) != (seq_t)1)
                ++cand;
        } else {
            while (gcd(cand, (seq_t)i) == (seq_t)1)
                ++cand;
        }
        prev = cand;
    }
    return prev;
}
