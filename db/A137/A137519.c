/*
 * A137519  a(1)=1. a(2)=2. For n>=3, a(n) = the smallest integer > a(n-1)
 *          that is coprime to (a(n-1)+1)*(a(n-2)+1).
 * https://oeis.org/A137519
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 73, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A137519:
   a(1) = 1, a(2) = 2;
   for n ≥ 3, a(n) is the smallest integer > a(n‑1) that is coprime
   to (a(n‑1)+1)*(a(n‑2)+1). */

#include <stdbool.h>

/* Helper: Euclidean gcd for arbitrary‑precision seq_t */
static seq_t gcd_seq_t(seq_t a, seq_t b)
{
    while (b != (seq_t)0) {
        seq_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* Public sequence generator */
seq_t a(idx_t n)
{
    if (n == (idx_t)0)
        return (seq_t)0;          /* undefined, return 0 as safe sentinel */

    /* Handle the two defined initial terms */
    if (n == (idx_t)1) return (seq_t)1;
    if (n == (idx_t)2) return (seq_t)2;

    /* Iteratively build the sequence up to n */
    seq_t prev2 = (seq_t)1;          /* a(n-2) */
    seq_t prev1 = (seq_t)2;          /* a(n-1) */
    seq_t cand;
    for (idx_t i = (idx_t)3; i <= n; ++i) {
        /* Find smallest integer > prev1 coprime to (prev1+1)*(prev2+1) */
        cand = prev1 + (seq_t)1;
        while (gcd_seq_t(cand, prev1 + (seq_t)1) != (seq_t)1 ||
               gcd_seq_t(cand, prev2 + (seq_t)1) != (seq_t)1) {
            ++cand;
        }
        /* Shift values for next iteration */
        prev2 = prev1;
        prev1 = cand;
    }
    return prev1;
}
