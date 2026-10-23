/*
 * A111228  Numbers where the digits are alternatively even and odd and no
 *          digit is 0.
 * https://oeis.org/A111228
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 65, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A111228: numbers whose digits alternate even/odd, no digit is 0. */
/* a(n) is the (n+1)-th admissible integer, starting with a(0)=1. */
#include <stdint.h>

static inline idx_t pow_uint(idx_t base, idx_t exp)
{
    idx_t r = 1;
    while (exp) {
        r *= base;
        --exp;
    }
    return r;
}

/* number of admissible strings of length len, starting with a digit
   of parity `parity` (0=even, 1=odd).  For len==0 the count is 1. */
static inline idx_t count_start(int parity, idx_t len)
{
    if (len == 0) return 1;
    if (parity == 0) /* start with even */
        return 4 * pow_uint(5, len - 1);   /* 4 even choices, then 5 odd choices */
    else               /* start with odd */
        return 5 * pow_uint(4, len - 1);   /* 5 odd choices, then 4 even choices */
}

/* total number of admissible numbers of a given length */
static inline idx_t count_len(idx_t len)
{
    return count_start(0, len) + count_start(1, len);
}

seq_t a(idx_t n)
{
    /* 1. find the length of the n-th number */
    idx_t cum = 0;
    idx_t len = 1;
    for (;;) {
        idx_t cnt = count_len(len);
        if (cum + cnt > n) break;
        cum += cnt;
        ++len;
    }

    /* 2. index within numbers of this length */
    idx_t k = n - cum;   /* 0‑based index */

    /* 3. construct the number digit by digit */
    seq_t result = 0;
    int prev_parity = -1;   /* no previous digit yet */

    static const int odd_digits[5]   = {1,3,5,7,9};
    static const int even_digits[4]  = {2,4,6,8};

    for (idx_t pos = 0; pos < len; ++pos) {
        if (pos == 0) {
            /* first digit: 1..9 (no zero) */
            for (int i = 0; i < 9; ++i) {
                int d = 1 + i;                /* 1..9 */
                int parity = d & 1;           /* 1=odd, 0=even */
                idx_t rem = count_start(1 - parity, len - pos - 1);
                if (k < rem) {
                    result = result * 10 + d;
                    prev_parity = parity;
                    break;
                } else {
                    k -= rem;
                }
            }
        } else {
            int need_parity = 1 - prev_parity;   /* alternating parity */
            const int *digits = (need_parity == 0) ? even_digits : odd_digits;
            int sz = (need_parity == 0) ? 4 : 5;
            for (int i = 0; i < sz; ++i) {
                int d = digits[i];
                idx_t rem = count_start(1 - need_parity, len - pos - 1);
                if (k < rem) {
                    result = result * 10 + d;
                    prev_parity = need_parity;
                    break;
                } else {
                    k -= rem;
                }
            }
        }
    }

    return result;
}
