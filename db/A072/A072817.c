/*
 * A072817  Accumulative sum of the greatest digit of n minus the least digit
 *          of n (A037904) <= 10^n.
 * https://oeis.org/A072817
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = 1 + Σ_{w=1..n} Σ_{k=1..9} (10-k)*k*((k+1)^w - 2*k^w +
 *            (k-1)^w) * - k*((k+1)^(w-1) - k^(w-1))
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 8, u64 n <= 18, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  a(n) is the cumulative sum of (greatest digit – least digit) for all integers
 *  ≤ 10^n, with a(1)=1 as the first term. */

#include <stdint.h>  /* may be provided by the build for seq_t and idx_t */

seq_t a(idx_t n)
{
    /* pow[b] holds b^(w-1) before the w‑th iteration.  Initially w=0. */
    seq_t pow[11] = {0};
    for (idx_t b = 0; b <= 10; ++b) {
        pow[b] = (seq_t)1;    /* b^0 = 1 for all bases */
    }

    seq_t sum = (seq_t)0;

    for (idx_t w = 1; w <= n; ++w) {
        seq_t term_w = (seq_t)0;

        for (idx_t k = 1; k <= 9; ++k) {
            seq_t pwk   = pow[k];        /* k^(w-1)   */
            seq_t pwk1  = pow[k+1];      /* (k+1)^(w-1) */
            seq_t pwk_1 = pow[k-1];      /* (k-1)^(w-1) */

            seq_t pwk_next   = pwk   * (seq_t)k;      /* k^w  */
            seq_t pwk1_next  = pwk1  * (seq_t)(k+1);  /* (k+1)^w */
            seq_t pwk_1_next = pwk_1 * (seq_t)(k-1);  /* (k-1)^w */

            seq_t part1 = pwk1_next;                   /* (k+1)^w */
            part1 -= (seq_t)2 * pwk_next;              /* - 2*k^w */
            part1 += pwk_1_next;                      /* + (k-1)^w */

            seq_t subTerm = (seq_t)(10 - k) * (seq_t)k * part1;

            seq_t part2 = pwk1 - pwk;                  /* (k+1)^(w-1) - k^(w-1) */
            seq_t subTerm2 = (seq_t)k * part2;          /* k * part2 */

            term_w += subTerm - subTerm2;
        }

        sum += term_w;

        /* Update pow for next iteration: pow[b] ← pow[b] * b  (now b^w) */
        for (idx_t b = 0; b <= 10; ++b) {
            pow[b] *= (seq_t)b;
        }
    }

    return (seq_t)1 + sum;  /* add the initial term a(1) = 1 */
}
