/*
 * A018859  Smallest power of 4 that begins with n.
 * https://oeis.org/A018859
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 2, u64 n <= 2, u128 n <= 8,
 *            big any n
 * Helpers:   mul_small, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function computes OEIS A018859: a(n) = the smallest power of 4
   whose decimal representation begins with the digits of n (n ≥ 1). */

#include "../utils/mul_small.h"   /* mul_small */
#include "../utils/seq_pow.h"     /* seq_pow */

static inline int digits_of(idx_t x)
/* Return the number of decimal digits of a positive integer x. */
{
    int d = 0;
    do { d++; x /= 10; } while (x > 0);
    return d;
}

seq_t a(idx_t n)
{
    /* Special case: n == 1 -> 4^0 = 1 */
    if (n == 1) return (seq_t)1;

    int len_n = digits_of(n);         /* number of digits of n */

    seq_t cur = (seq_t)1;             /* current power 4^k, start at 4^0 */
    int len_cur = 1;                  /* current number of digits of cur   */
    seq_t pow10_next = (seq_t)10;     /* next threshold 10^len_cur          */
    idx_t k = 0;                      /* exponent of cur (not used further) */

    for (;;) {
        /* advance to next power of 4 */
        cur = mul_small(cur, 4);
        k++;

        /* adjust digit counter */
        while (cur >= pow10_next) {
            pow10_next = mul_small(pow10_next, 10);
            len_cur++;
        }

        if (len_cur < len_n) continue;   /* not enough digits yet */

        /* compute divisor = 10^(len_cur - len_n) */
        idx_t shift = (idx_t)(len_cur - len_n);
        seq_t divisor = (shift == 0) ? (seq_t)1 : seq_pow((seq_t)10, shift);

        /* most significant len_n digits of cur */
        seq_t high = cur / divisor;

        if (high == (seq_t)n) return cur;
    }
}
