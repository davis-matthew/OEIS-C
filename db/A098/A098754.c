/*
 * A098754  Slowest increasing sequence of integers that alternately begin and
 *          end with 1.
 * https://oeis.org/A098754
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 26, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   digit_len, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A098754: Slowest increasing sequence of integers that alternately begin
       and end with 1.
   The sequence is defined by:
       a(1) = 1
   For n > 1:
       if n is even:  a(n) = smallest integer > a(n-1) that ends with digit 1
       if n is odd :  a(n) = smallest integer > a(n-1) that begins with digit 1
   Each term is the minimal number satisfying the alternating condition
   and larger than its predecessor, giving the slowest possible growth.
   This implementation computes the terms iteratively in O(n) time. */

#include "../utils/pow10.h"
#include "../utils/digit_len.h"

/* compute a(n) */
seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;

    seq_t prev = (seq_t)1;
    seq_t curr = prev;

    for (idx_t i = 2; i <= n; ++i) {
        if ((i & 1) == 0) {           /* even index: end with 1 */
            seq_t mod10 = prev % (seq_t)10;
            unsigned long long last = (unsigned long long)mod10;
            int inc = (1 - last + 10) % 10;
            if (inc == 0) inc = 10;
            curr = prev + (seq_t)inc;
        } else {                      /* odd index: begin with 1 */
            int digits = digit_len(prev);           /* number of decimal digits */
            seq_t base   = pow10((idx_t)digits - 1);/* 10^(digits-1) */
            seq_t rest   = prev - base;             /* something < base   */
            seq_t restp  = rest + (seq_t)1;          /* rest+1            */

            if (restp < base)                      /* same length fits   */
                curr = base + restp;
            else                                   /* need one more digit */
                curr = base * (seq_t)10;            /* 10*base == 10^digits */
        }
        prev = curr;
    }
    return curr;
}
