/*
 * A082249  Reverse concatenation of 7 numbers that are multiples of 7.
 * https://oeis.org/A082249
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) is the reverse concatenation of the 7 consecutive integers
 *            s, s-1, …, s-6, where s is the nth integer greater than 15 such
 *            that the concatenation is divisible by 7 and all 7 numbers have
 *            the same number of decimal digits
 * Range:     u8 none, u16 none, u32 none, u64 n <= 12, u128 n <= 13,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"

static const idx_t inv_mod7[7] = {0, 1, 4, 5, 2, 3, 6};

seq_t a(idx_t n)
{
    idx_t remaining = n;
    idx_t d = 2;                          /* current digit length */

    while (1) {
        /* bounds for start in this digit length */
        seq_t pow10_d_minus1 = pow10(d - 1);
        seq_t pow10_d       = pow10(d);
        idx_t low  = (idx_t)pow10_d_minus1 + 6;   /* smallest start with same digit length */
        idx_t high = (idx_t)pow10_d - 1;          /* largest start with this digit length */

        /* compute A = sum_{i=0..6} r^i  and  B = sum_{i=0..6} i*r^i ,  r = 10^d mod 7 */
        idx_t r = (idx_t)pow10_d % 7;
        idx_t p = 1 % 7;
        idx_t A = 0, B = 0;
        for (int i = 0; i < 7; ++i) {
            A = (A + p) % 7;
            B = (B + i * p) % 7;
            p = (p * r) % 7;
        }

        /* s must satisfy s ≡ C  (mod 7) where C = (6*A - B) * A^{-1}  (mod 7) */
        idx_t temp = (6 * A) % 7;
        temp = (temp + 7 - B) % 7;         /* (6*A - B) mod 7 */
        idx_t invA   = inv_mod7[A];       /* inverse of A mod 7 (A != 0) */
        idx_t C      = (temp * invA) % 7;  /* required congruence for start */

        /* first start in this range that satisfies the congruence */
        idx_t rem_low = low % 7;
        idx_t offset  = (C + 7 - rem_low) % 7;
        idx_t first_start = low + offset;

        /* number of valid starts in this digit length */
        idx_t count = 0;
        if (first_start <= high) {
            count = (high - first_start) / 7 + 1;
        }

        if (remaining > count) {
            remaining -= count;    /* skip this whole block of digit length d */
            d++;
            continue;
        }

        /* the desired block lies in this digit length */
        idx_t block_index = remaining - 1;          /* 0‑based offset within this digit length */
        idx_t start = first_start + 7 * block_index;

        /* reverse concatenation of the 7 numbers */
        seq_t result = 0;
        seq_t pow10_d_value = pow10_d;              /* 10^d as seq_t */
        for (int i = 0; i < 7; ++i) {
            seq_t num = (seq_t)start - i;
            result = result * pow10_d_value + num;
        }
        return result;
    }
}
