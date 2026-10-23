/*
 * A020057  a(n) = floor(Gamma(n+3/11)/Gamma(3/11)).
 * https://oeis.org/A020057
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor( Gamma(n+3/11) / Gamma(3/11) ) * = floor(
 *            Π_{k=0}^{n‑1} (11 k+3) / 11ⁿ )
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 14, u64 n <= 21, u128 n <= 35,
 *            big any n
 * Helpers:   seq_from_str
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "../utils/seq_from_str.h"

#define BASE 1000000000u   /* 1e9 */
#define MAX_DIGITS 6500    /* enough for n up to 10000 */

seq_t a(idx_t n)
{
    if (n == 0u)
        return (seq_t)1;

    /* big integer stored little‑endian in base BASE */
    struct BigInt {
        uint32_t d[MAX_DIGITS];
        size_t  len;
    } num;

    num.len = 1;
    num.d[0] = 1;

    /* multiply by (11*k + 3) for k = 0 .. n‑1 */
    for (idx_t k = 0; k < n; ++k) {
        uint32_t factor = (uint32_t)(11 * k + 3);
        uint64_t carry = 0;
        for (size_t i = 0; i < num.len; ++i) {
            uint64_t prod = (uint64_t)num.d[i] * factor + carry;
            num.d[i] = (uint32_t)(prod % BASE);
            carry    = prod / BASE;
        }
        while (carry) {
            if (num.len >= MAX_DIGITS) {      /* safety, should not happen */
                return (seq_t)0;
            }
            num.d[num.len++] = (uint32_t)(carry % BASE);
            carry /= BASE;
        }
    }

    /* divide by 11, n times */
    for (idx_t div = 0; div < n; ++div) {
        uint64_t rem = 0;
        for (size_t i = num.len; i-- > 0; ) {
            uint64_t cur = num.d[i] + rem * BASE;
            num.d[i]    = (uint32_t)(cur / 11);
            rem         = cur % 11;
        }
        /* trim leading zeros */
        while (num.len > 1 && num.d[num.len - 1] == 0)
            --num.len;
    }

    /* convert big integer to decimal string */
    uint32_t ms = num.d[num.len - 1];
    size_t ms_digits = 0;
    uint32_t tmp = ms;
    do {
        ++ms_digits;
        tmp /= 10;
    } while (tmp > 0);

    size_t dec_len = (num.len - 1) * 9 + ms_digits;
    char *buf = (char *)malloc(dec_len + 1);
    if (!buf)
        return (seq_t)0;

    /* write most significant block without leading zeros */
    int pos = sprintf(buf, "%u", ms);
    /* write remaining blocks with leading zeros */
    for (size_t i = num.len; i-- > 1; ) {
        pos += sprintf(buf + pos, "%09u", num.d[i - 1]);
    }
    buf[dec_len] = '\0';

    seq_t result = seq_from_str(buf);
    free(buf);
    return result;
}
