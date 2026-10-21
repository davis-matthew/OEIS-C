/*
 * A059934  Third step in Goodstein sequences, i.e., g(5) if g(2)=n: write
 *          g(4)=A057650(n) in hereditary representation base 4, bump to base
 *          5, then subtract 1 to produce g(5).
 * https://oeis.org/A059934
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 15, u32 n <= 15, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A059934 – Third step in Goodstein sequences
    a(n) is g(5) where g(2) = n.
    Computation:
      g(3) = step(n, 2)   // change base 2 → 3, subtract 1
      g(4) = step(g(3), 3)
      g(5) = step(g(4), 4)
    Each step replaces the hereditary base‑b representation of the
    argument by the same expression evaluated in base b+1 and then
    subtracts 1.  All operations are performed on the type seq_t
    (arbitrary‑precision or fixed‑width unsigned integer).  The
    implementation is fully recursive and works for any n ≥ 2.
*/

#include <stdint.h>

static seq_t pow_seq_t(seq_t base, idx_t exp)
{
    seq_t result = 1;
    for (idx_t i = 0; i < exp; ++i)
        result *= base;
    return result;
}

/*  Evaluate `n` which is written in hereditary base `basePrev`,
    replacing the base by `baseNew`.  The result is a seq_t.  */
static seq_t eval_step(seq_t n, idx_t basePrev, idx_t baseNew)
{
    /* Base case: when n < basePrev the representation is just n. */
    if (n < (seq_t)basePrev) return n;

    /* Find largest exponent e such that basePrev^e <= n. */
    idx_t e = 0;
    seq_t powPrev = 1;
    seq_t nextPow = basePrev;
    while (nextPow <= n) {
        powPrev = nextPow;
        ++e;
        nextPow = powPrev * (seq_t)basePrev;  /* avoid overflow when n==max */
    }

    /* Coefficient of this term. */
    seq_t coeff = n / powPrev;
    seq_t rem   = n - coeff * powPrev;

    /* Evaluate exponent e recursively. */
    seq_t eVal = eval_step((seq_t)e, basePrev, baseNew);
    /* Convert eVal to idx_t; it is tiny (≤ log_basePrev(n)). */
    idx_t eNewIdx = (idx_t)eVal;

    /* Compute this term in the new base. */
    seq_t term = coeff * pow_seq_t((seq_t)baseNew, eNewIdx);

    /* Continue with the remainder. */
    return term + eval_step(rem, basePrev, baseNew);
}

/*  One Goodstein step: from base `b` to base `b+1`, then subtract 1.  */
static seq_t goodstein_step(seq_t n, idx_t basePrev)
{
    seq_t val = eval_step(n, basePrev, basePrev + 1);
    return val - 1;
}

seq_t a(idx_t n)
{
    /* Preconditions: n >= 2.  Behaviour for n < 2 is undefined. */
    seq_t g3 = goodstein_step((seq_t)n,      /* base 2 → 3 */
                              2);
    seq_t g4 = goodstein_step(g3,            /* base 3 → 4 */
                              3);
    seq_t g5 = goodstein_step(g4,            /* base 4 → 5 */
                              4);
    return g5;
}
