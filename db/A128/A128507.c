/*
 * A128507  Denominators of partial sums for a series for
 *          3*sqrt(2)*(Pi^3)/2^7.
 * https://oeis.org/A128507
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 8, u128 n <= 16,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A128507:
 *  Denominators of partial sums r(n) = Σ_{k=0..n} S(2k)/(2k+1)^3
 *  where S(2k) = [1,1,-1,-1] repeated.
 *  This function returns the reduced denominator of r(n).
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "../utils/gcd.h"

/* Signed number helper */
typedef struct {
    bool neg;   /* true if negative */
    seq_t abs;  /* absolute value (non‑negative) */
} SignedNum;

/* Add two signed numbers: res = a + b */
static void signed_add(const SignedNum *a, const SignedNum *b, SignedNum *res) {
    if (a->neg == b->neg) {                /* same sign */
        res->neg = a->neg;
        res->abs = a->abs + b->abs;
    } else {                                /* different signs */
        if (a->abs >= b->abs) {
            res->neg = a->neg;
            res->abs = a->abs - b->abs;
        } else {
            res->neg = b->neg;
            res->abs = b->abs - a->abs;
        }
    }
}

seq_t a(idx_t n) {
    /* pattern S(2k): [1,1,-1,-1] */
    const int pattern[4] = {1, 1, -1, -1};

    /* current fraction num/den, initially 0/1 */
    SignedNum cur_num = {false, 0};
    seq_t cur_den = 1;

    for (idx_t k = 0; k <= n; ++k) {
        /* denominator of the k-th term:
         * (2k+1)^3
         */
        seq_t base = (seq_t)(2 * k + 1);
        seq_t den_i = base * base * base;

        /* S(2k) value */
        int s = pattern[k & 3];  /* k % 4 */

        /* Compute product: cur_num * den_i (positive absolute) */
        SignedNum prod1 = {false, cur_num.abs * den_i};

        /* Term to add: s * cur_den, represented as SignedNum */
        SignedNum term = {(s < 0), cur_den};

        /* Add them to obtain new numerator */
        SignedNum new_num;
        signed_add(&prod1, &term, &new_num);

        /* New denominator: cur_den * den_i */
        seq_t new_den = cur_den * den_i;

        /* Reduce fraction */
        seq_t g = gcd(new_num.abs, new_den);
        if (g > 1) {
            new_num.abs /= g;
            new_den /= g;
        }

        /* Update state for next iteration */
        cur_num = new_num;
        cur_den = new_den;
    }

    return cur_den;
}
