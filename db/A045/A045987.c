/*
 * A045987  Describe 2^n.
 * https://oeis.org/A045987
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   run‑length encode the decimal digits of 2^n. a(n) is the
 *            concatenation of the counts and the corresponding digits
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 16, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Validated: partial, pass, 23 terms through n = 22 (2026-11-01)
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>                 /* size_t */
#include <stdbool.h>
#include <stdint.h>
#include "../utils/pow2.h"

/* convert a seq_t value to a decimal string */
static void seq_to_str(seq_t v, char *buf, size_t *len)
{
    if (v == 0) {
        buf[0] = '0';
        *len = 1;
        return;
    }
    char tmp[64];
    int pos = 0;
    seq_t ten = (seq_t)10;
    while (v != 0) {
        tmp[pos++] = '0' + (int)(v % ten);
        v /= ten;
    }
    for (int i = 0; i < pos; ++i)
        buf[i] = tmp[pos - 1 - i];
    *len = pos;
}

/* Compute 10^e for a non‑negative integer exponent e */
static seq_t pow10(idx_t e) {
    seq_t res = 1;
    while (e) {
        res *= 10;
        e--;
    }
    return res;
}

static char digit_buf[4000];   /* large enough for 2^10000 -> 3011 digits + safety */
static size_t digit_len;

seq_t a(idx_t n)
{
    /* 1. Compute 2^n */
    seq_t val = pow2(n);

    /* 2. Convert to base‑10 string (most significant first) */
    seq_to_str(val, digit_buf, &digit_len);

    if (digit_len == 0) {
        return 0; /* technically impossible for n >= 0 */
    }

    /* 3. Run‑length encode the digits */
    seq_t result = 0;
    unsigned char current = digit_buf[0] - '0';
    int count = 1;

    for (size_t i = 1; i < digit_len; ++i) {
        unsigned char d = digit_buf[i] - '0';
        if (d == current) {
            ++count;
        } else {
            /* number of decimal digits in count */
            int len_count = 0;
            int temp = count;
            do {
                ++len_count;
                temp /= 10;
            } while (temp);

            /* append count followed by the digit */
            seq_t factor = pow10((idx_t)(len_count + 1));
            result = result * factor + ((seq_t)count * 10 + current);

            /* start new run */
            current = d;
            count = 1;
        }
    }

    /* append the final run */
    int len_count = 0;
    int temp = count;
    do {
        ++len_count;
        temp /= 10;
    } while (temp);

    seq_t factor = pow10((idx_t)(len_count + 1));
    result = result * factor + ((seq_t)count * 10 + current);

    return result;
}
