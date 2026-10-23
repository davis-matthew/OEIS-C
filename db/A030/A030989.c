/*
 * A030989  6-automorphic numbers: final digits of 6n^2 agree with n.
 * https://oeis.org/A030989
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 10, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Helpers:   mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: a(n) is the unique positive integer <10^n such that
   a(n) is divisible by 2^n and 6*a(n)-1 is divisible by 5^n.
   The algorithm builds the number digit by digit, ensuring the conditions
   hold at each step. */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/mul_small.h"

seq_t a(idx_t n)
{
    if (n <= 1) /* a(1) = 6 */
        return (seq_t)6;

    seq_t A = (seq_t)6;      /* current a(i-1)   */
    seq_t pow10 = (seq_t)10; /* 10^(i-1)         */
    seq_t pow5 = (seq_t)5;   /* 5^(i-1)          */
    seq_t pow2 = (seq_t)2;   /* 2^(i-1)          */

    for (idx_t i = 2; i <= n; ++i) {
        /* parity of l = A / 2^(i-1)   (l is odd or even) */
        seq_t l = A / pow2;
        unsigned int parity = (unsigned int)(l % 2U);

        seq_t target_mod = pow5 * (seq_t)5; /* 5^i */

        bool found = false;
        seq_t candidate = A;
        for (int k = parity; k < 10; k += 2) { /* candidates of required parity */
            seq_t mult = mul_small(pow10, (unsigned int)k);
            candidate = A + mult;               /* a = a + k*10^(i-1) */

            /* Check 6*a - 1 ≡ 0 (mod 5^i)   ⇔   6*a mod 5^i == 1   */
            seq_t cand_mod = candidate % target_mod;
            seq_t six_mod = (cand_mod * (seq_t)6) % target_mod;
            if (six_mod == 1U) {
                A = candidate;
                found = true;
                break;
            }
        }
        /* in the (very unlikely) event no k satisfies the condition,
           the sequence definition guarantees a solution exists. */

        /* prepare powers for next iteration */
        pow10 = mul_small(pow10, (unsigned int)10); /* 10^i */
        pow5  = mul_small(pow5,  (unsigned int)5);  /* 5^i  */
        pow2  = mul_small(pow2,  (unsigned int)2);  /* 2^i  */
    }
    return A;
}
