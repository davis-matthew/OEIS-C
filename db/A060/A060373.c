/*
 * A060373  q(n), negative part of n when n=p(n)-q(n) with p(n), q(n),
 *          p(n)+q(n) in A005836, integers written without 2 in base 3.
 * https://oeis.org/A060373
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nice, nonn, look
 * Formula:   not stated
 * Range:     u8 n <= 364, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Computation: For each n the function returns the smallest
 * number q (in the set of base‑3 numbers with digits only 0 or 1)
 * such that n + q is also in that set.  This q is the "negative
 * part" of n in the definition of OEIS A060373.
 *
 * The algorithm scans the ternary digits of n from least significant
 * to most significant, keeping a carry that propagates when a digit
 * sum would otherwise become 2.  Whenever the digit plus carry equals
 * 2 we must set the corresponding q‑digit to 1 (to cause a carry),
 * otherwise the q‑digit stays 0.  This greedy choice yields the
 * minimal q in value.
 */
#include <stdint.h>

seq_t a(idx_t n)
{
    idx_t m = n;            /* local copy of the index          */
    seq_t q = 0;            /* result                           */
    seq_t power = 1;        /* 3^i for the current digit position */
    int carry = 0;          /* carry from lower digit            */

    while (m != 0 || carry) {
        int d = m % 3;      /* current ternary digit of n        */
        m /= 3;

        if (d + carry == 2) {
            /* need a q‑digit 1 to keep the sum digit 0 and produce a carry */
            q += power;
            carry = 1;
        } else {
            /* q‑digit 0 is enough; compute carry from the sum */
            carry = (d + carry) / 3;
        }

        power *= 3;
    }

    return q;
}
