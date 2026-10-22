/*
 * A001127  Trajectory of 1 under map x->x + (x-with-digits-reversed).
 * https://oeis.org/A001127
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy, nice
 * Formula:   a(0) = 1, a(n) = a(n-1) + reverse(a(n-1)), where reverse()
 *            reverses the decimal digits
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 24, u64 n <= 44, u128 n <= 89,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

static unsigned char *a001127_dig = NULL;  /* little-endian decimal digits */
static unsigned char *a001127_tmp = NULL;
static size_t a001127_len = 0;
static size_t a001127_cap = 0;
static idx_t a001127_cur = 0;

static void a001127_reset(void) {
    if (a001127_cap < 16) {
        a001127_cap = 16;
        a001127_dig = (unsigned char *)realloc(a001127_dig, a001127_cap);
        a001127_tmp = (unsigned char *)realloc(a001127_tmp, a001127_cap);
    }
    a001127_dig[0] = 1;
    a001127_len = 1;
    a001127_cur = 0;
}

static void a001127_step(void) {
    size_t i, L = a001127_len;
    unsigned carry = 0;
    if (L + 2 > a001127_cap) {
        a001127_cap = 2 * (L + 2);
        a001127_dig = (unsigned char *)realloc(a001127_dig, a001127_cap);
        a001127_tmp = (unsigned char *)realloc(a001127_tmp, a001127_cap);
    }
    for (i = 0; i < L; i++) {
        unsigned s = (unsigned)a001127_dig[i] + (unsigned)a001127_dig[L - 1 - i] + carry;
        a001127_tmp[i] = (unsigned char)(s % 10);
        carry = s / 10;
    }
    if (carry) a001127_tmp[L++] = (unsigned char)carry;
    {
        unsigned char *t = a001127_dig;
        a001127_dig = a001127_tmp;
        a001127_tmp = t;
    }
    a001127_len = L;
    a001127_cur++;
}

seq_t a(idx_t n) {
    seq_t r = 0;
    size_t i;
    if (a001127_dig == NULL || n < a001127_cur) a001127_reset();
    while (a001127_cur < n) a001127_step();
    for (i = a001127_len; i-- > 0;) {
        r = r * 10 + (seq_t)a001127_dig[i];
    }
    return r;
}
