/*
 * A183997  Number of (n+1) X 4 0..2 arrays with all 2 X 2 subblock sums the
 *          same.
 * https://oeis.org/A183997
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of (n+1) x 4 0..2 arrays with all 2x2 subblock
 *            sums the same. Empirical recurrence of order 9 (valid for all n
 *            >= 10): a(n) = 6*a(n-1) - 60*a(n-3) + 85*a(n-4) + 150*a(n-5) -
 *            360*a(n-6) + 396*a(n-8) - 216*a(n-9). Initial values (offset =
 *            1): a(1)=361, a(2)=579, a(3)=977, a(4)=1887, a...
 * Range:     u8 none, u16 n <= 8, u32 n <= 19, u64 n <= 39, u128 n <= 79,
 *            big any n
 * Helpers:   seq_from_str
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include "../utils/seq_from_str.h"

typedef struct {
    int len;                 /* number of used limbs */
    unsigned int limb[600];  /* 32‑bit limbs, little endian */
} BigInt;

/* set to zero */
static void bi_set_zero(BigInt *bi) { bi->len = 0; }

/* initialize from unsigned long long */
static void bi_set_uint(BigInt *bi, unsigned long long val) {
    if (val == 0) { bi->len = 0; return; }
    bi->len = 0;
    while (val) {
        bi->limb[bi->len++] = (unsigned)(val & 0xFFFFFFFFULL);
        val >>= 32;
    }
}

/* copy a to b (b = a) */
static void bi_copy(BigInt *b, const BigInt *a) {
    b->len = a->len;
    memcpy(b->limb, a->limb, a->len * sizeof(unsigned));
}

/* dest += src  (in place) */
static void bi_add(BigInt *dest, const BigInt *src) {
    unsigned long long carry = 0;
    int dlen = dest->len;
    int slen = src->len;
    int maxlen = dlen > slen ? dlen : slen;
    int i;
    for (i = 0; i < maxlen || carry; ++i) {
        unsigned long long sum = carry;
        if (i < dlen) sum += dest->limb[i];
        if (i < slen) sum += src->limb[i];
        dest->limb[i] = (unsigned)(sum & 0xFFFFFFFFULL);
        carry = sum >> 32;
    }
    dest->len = i;
}

/* dest = a - b   (assumes a >= b) */
static void bi_sub(BigInt *dest, const BigInt *a, const BigInt *b) {
    int borrow = 0;
    int i;
    for (i = 0; i < a->len; ++i) {
        long long diff = (long long)a->limb[i] - (i < b->len ? b->limb[i] : 0) - borrow;
        if (diff < 0) {
            diff += ((long long)1 << 32);
            borrow = 1;
        } else
            borrow = 0;
        dest->limb[i] = (unsigned)diff;
    }
    dest->len = a->len;
    while (dest->len > 0 && dest->limb[dest->len - 1] == 0)
        dest->len--;
}

/* dest = src * m  (small multiplier) */
static void bi_mul_small(BigInt *dest, const BigInt *src, unsigned int m) {
    unsigned long long carry = 0;
    int i;
    for (i = 0; i < src->len || carry; ++i) {
        unsigned long long prod = carry;
        if (i < src->len) prod += (unsigned long long)src->limb[i] * m;
        dest->limb[i] = (unsigned)(prod & 0xFFFFFFFFULL);
        carry = prod >> 32;
    }
    dest->len = i;
}

/* convert BigInt to decimal string; buffer must be big enough */
static void bi_to_decimal(const BigInt *bi, char *buf) {
    /* copy bi so we can modify */
    BigInt temp;
    bi_copy(&temp, bi);
    unsigned rems[1000];
    int rcnt = 0;
    while (temp.len > 0) {
        unsigned long long rem = 0;
        for (int i = temp.len - 1; i >= 0; --i) {
            unsigned long long cur = (rem << 32) | temp.limb[i];
            temp.limb[i] = (unsigned)(cur / 1000000000ULL);
            rem = cur % 1000000000ULL;
        }
        while (temp.len > 0 && temp.limb[temp.len - 1] == 0) temp.len--;
        rems[rcnt++] = (unsigned)rem;
    }
    if (rcnt == 0) { buf[0] = '0'; buf[1] = '\0'; return; }
    int pos = 0;
    pos += sprintf(buf + pos, "%u", rems[rcnt - 1]);
    for (int k = rcnt - 2; k >= 0; --k)
        pos += sprintf(buf + pos, "%09u", rems[k]);
    buf[pos] = '\0';
}

/* Main sequence function */
seq_t a(idx_t n) {
    static const unsigned long long init[9] = {
        361ULL, 579ULL, 977ULL, 1887ULL, 3793ULL,
        8415ULL, 19217ULL, 47031ULL, 117601ULL
    };
    if (n <= 9) return (seq_t)init[n - 1];

    BigInt arr[9];          /* circular buffer for last 9 terms */
    for (int i = 0; i < 9; ++i) bi_set_uint(&arr[i], init[i]);

    BigInt pos, neg, temp, cur;
    for (idx_t k = 10; k <= n; ++k) {
        bi_set_zero(&pos);
        bi_set_zero(&neg);

        /* positive contributions */
        bi_copy(&temp, &arr[8]);        /* a(k-1) */
        bi_mul_small(&temp, &temp, 6);
        bi_add(&pos, &temp);

        bi_copy(&temp, &arr[5]);        /* a(k-4) */
        bi_mul_small(&temp, &temp, 85);
        bi_add(&pos, &temp);

        bi_copy(&temp, &arr[4]);        /* a(k-5) */
        bi_mul_small(&temp, &temp, 150);
        bi_add(&pos, &temp);

        bi_copy(&temp, &arr[1]);        /* a(k-8) */
        bi_mul_small(&temp, &temp, 396);
        bi_add(&pos, &temp);

        /* negative contributions */
        bi_copy(&temp, &arr[6]);        /* a(k-3) */
        bi_mul_small(&temp, &temp, 60);
        bi_add(&neg, &temp);

        bi_copy(&temp, &arr[3]);        /* a(k-6) */
        bi_mul_small(&temp, &temp, 360);
        bi_add(&neg, &temp);

        bi_copy(&temp, &arr[0]);        /* a(k-9) */
        bi_mul_small(&temp, &temp, 216);
        bi_add(&neg, &temp);

        /* cur = pos - neg */
        bi_sub(&cur, &pos, &neg);

        /* shift buffer */
        for (int i = 0; i < 8; ++i) bi_copy(&arr[i], &arr[i + 1]);
        bi_copy(&arr[8], &cur);
    }

    /* Convert result to seq_t via decimal string */
    char dec_buf[20000];
    bi_to_decimal(&arr[8], dec_buf);
    return seq_from_str(dec_buf);
}
