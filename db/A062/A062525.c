/*
 * A062525  8^a(n) is smallest power of 8 containing the string 'n'.
 * https://oeis.org/A062525
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) is the smallest exponent e such that 8^e (in decimal)
 *            contains the decimal representation of n
 * Range:     u8 n <= 1008, u16 n <= 9999, u32 n <= 9999, u64 n <= 9999,
 *            u128 n <= 9999, big any n
 * Helpers:   build_pi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/build_pi.h"

static void u64_to_str(idx_t n, char *buf, size_t *len) {
    size_t pos = 0;
    idx_t tmp = n;
    /* handle 0 specially */
    if (tmp == 0) {
        buf[pos++] = '0';
    } else {
        while (tmp > 0) {
            buf[pos++] = '0' + (tmp % 10);
            tmp /= 10;
        }
    }
    /* reverse */
    for (size_t i = 0; i < pos / 2; ++i) {
        char c = buf[i];
        buf[i] = buf[pos - 1 - i];
        buf[pos - 1 - i] = c;
    }
    *len = pos;
}

seq_t a(idx_t n) {
    /* Convert n to decimal string */
    static char patbuf[32];
    size_t patlen;
    u64_to_str(n, patbuf, &patlen);

    /* Build KMP prefix function */
    size_t *pi = (size_t *)malloc(patlen * sizeof(size_t));
    build_pi(patbuf, patlen, pi);

    /* Big integer: decimal digits lsb-first */
    size_t cap = 128;
    uint8_t *digits = (uint8_t *)malloc(cap);
    digits[0] = 1;   /* 8^0 = 1 */
    size_t len = 1;  /* number of digits */

    seq_t ans = 0;
    while (1) {
        /* search within current 8^e */
        size_t state = 0;
        for (size_t i = len; i-- > 0; ) {
            char d = '0' + digits[i];
            while (state > 0 && d != patbuf[state]) state = pi[state - 1];
            if (d == patbuf[state]) ++state;
            if (state == patlen) {
                ans = (seq_t)ans;  /* current exponent */
                free(digits);
                free(pi);
                return ans;
            }
        }

        /* multiply big integer by 8 to get next power */
        uint16_t carry = 0;
        for (size_t i = 0; i < len; ++i) {
            uint16_t prod = digits[i] * 8 + carry;
            digits[i] = prod % 10;
            carry = prod / 10;
        }
        while (carry > 0) {
            if (len == cap) {
                cap <<= 1;
                digits = (uint8_t *)realloc(digits, cap);
            }
            digits[len++] = carry % 10;
            carry /= 10;
        }

        ++ans;
    }
}
