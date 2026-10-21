/*
 * A077729  Smallest square beginning with n copies of n.
 * https://oeis.org/A077729
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 9, u128 n <= 9,
 *            big any n
 * Helpers:   digit_len, floor_sqrt, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Smallest square whose decimal representation begins with n copies of n,
   found by testing increasing numbers of trailing digits. */
#include <stdint.h>
#include <stdbool.h>
#include "../utils/floor_sqrt.h"
#include "../utils/pow10.h"
#include "../utils/digit_len.h"

static seq_t ceil_sqrt(seq_t x)
{
    seq_t y = floor_sqrt(x);
    if (y * y == x)
        return y;
    return y + 1;
}

seq_t a(idx_t n)
{
    if (n == 0)   /* undefined for n<1, keep offset 1 */
        return 0;

    seq_t n_seq = (seq_t)n;               /* n as seq_t */
    idx_t len_n  = digit_len(n_seq);      /* number of digits of n */
    seq_t pow10_n = pow10(len_n);         /* 10^len_n */

    /* P = n repeated n times */
    seq_t P = 0;
    for (idx_t i = 0; i < n; ++i)
        P = P * pow10_n + n_seq;

    seq_t P_plus1 = P + 1;

    seq_t pow10_t = 1;    /* 10^t for current t (number of trailing digits) */
    while (true)
    {
        seq_t lower = P * pow10_t;
        seq_t upper = (P_plus1 * pow10_t) - 1;

        seq_t k_min = ceil_sqrt(lower);
        seq_t k_max = floor_sqrt(upper);

        if (k_min <= k_max)
            return k_min * k_min;   /* smallest square found */

        pow10_t = pow10_t * 10;      /* increase t by one digit */
    }
}
