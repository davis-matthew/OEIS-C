/*
 * A087406  Multiples of 2 with digits grouped in pairs and leading zeros
 *          omitted.
 * https://oeis.org/A087406
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) is the n-th two‑digit pair obtained from the concatenation
 *            of the positive even integers (2,4,6,8,10,12,…) when grouped in
 *            pairs and leading zeros omitted
 * Range:     u8 n <= 71, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>  /* for size_t, etc. */

/* compute 10^exp as an unsigned 128‑bit integer */
static __uint128_t pow10_u128(int exp) {
    __uint128_t r = 1;
    for (int i = 0; i < exp; ++i) r *= 10;
    return r;
}

/* return the decimal digit at 1‑based position pos in the infinite
   string “2468101214…”.  pos is guaranteed to be >= 1. */
static int digit_at_pos(idx_t pos) {
    idx_t remaining = pos;    /* how many digits still needed */
    int d = 1;                /* current digit length of even numbers */

    while (1) {
        /* number of even integers having exactly d digits */
        __uint128_t cnt;
        if (d == 1) {
            cnt = 4;                     /* 2,4,6,8 */
        } else {
            __uint128_t first = pow10_u128(d - 1);   /* 10^{d-1} */
            __uint128_t last  = pow10_u128(d) - 2;   /* largest even <10^d */
            cnt = (last - first) / 2 + 1;
        }
        __uint128_t group_digits = cnt * (unsigned)d;

        if ((__uint128_t)remaining <= group_digits) break;
        remaining -= (idx_t)group_digits;   /* subtract whole group */
        ++d;
    }

    /* which even number and which digit inside it */
    unsigned idx_in_group = (unsigned)((remaining - 1) / d);
    unsigned digit_index  = (unsigned)((remaining - 1) % d);

    /* value of the even number */
    __uint128_t val;
    if (d == 1) {
        val = (__uint128_t)2 + (__uint128_t)2 * idx_in_group;
    } else {
        val = pow10_u128(d - 1) + (__uint128_t)2 * idx_in_group;
    }

    /* divisor to isolate the desired digit */
    __uint128_t divisor = pow10_u128(d - digit_index - 1);
    unsigned digit = (unsigned)((val / divisor) % 10);
    return (int)digit;
}

seq_t a(idx_t n)
{
    /* n is 1‑based */
    idx_t pos1 = n << 1;        /* position of the second digit of the pair */
    idx_t pos0 = pos1 - 1;      /* position of the first digit of the pair */

    int d0 = digit_at_pos(pos0);
    int d1 = digit_at_pos(pos1);

    /* combine the two digits; leading zeros are omitted automatically */
    seq_t result = (seq_t)(d0 * 10 + d1);
    return result;
}
