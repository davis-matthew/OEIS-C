/*
 * A177934  Decimal expansion of sqrt(71216963807).
 * https://oeis.org/A177934
 *
 * Offset:    6    (a(6) is the first term)
 * Keywords:  cons, nonn
 * Formula:   not stated
 * Range:     u8 n <= 110, u16 n <= 110, u32 n <= 110, u64 n <= 110,
 *            u128 n <= 110, big any n
 * Helpers:   _floor_isqrt_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A177934 – Decimal expansion of sqrt(71216963807).
 *  a(n) = the (n‑5)th decimal digit of sqrt(71216963807),
 *  counting the first digit of the integer part as position 6.
 *  The function computes digits by the digit‑by‑digit algorithm
 *  for square roots using a simple big‑integer implementation in
 *  base 1 000 000 000.  It is memoised and works for any n ≥ 6.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/_floor_isqrt_u64.h"

/* Constant to be square‑rooted */
static const uint64_t N_CONST = 71216963807ULL;

/* Working precision: enough for 10 000 decimal digits
 * (≈ 1114 limbs base 1e9). */
#define BIG_MAX_LEN 2500
#define BASE 1000000000U  /* 1e9 */

static uint32_t root_arr[BIG_MAX_LEN], rem_arr[BIG_MAX_LEN],
                cand_arr[BIG_MAX_LEN], cand_plus_arr[BIG_MAX_LEN],
                temp_arr[BIG_MAX_LEN];
static size_t root_len = 0, rem_len = 0,
             cand_len = 0, cand_plus_len = 0,
             temp_len = 0;

static uint8_t *digit_buf = NULL;
static size_t digit_len = 0, digit_cap = 0;
static bool init_done = false;

/*--------- Big integer helpers (little‑endian, base BASE) ---------*/
static inline void big_set_uint64(uint32_t *a, size_t *len, uint64_t v)
{
    size_t i = 0;
    do {
        a[i++] = (uint32_t)(v % BASE);
        v /= BASE;
    } while (v);
    *len = i;
}

static inline void big_copy(uint32_t *dest, size_t *dlen,
                           const uint32_t *src, size_t slen)
{
    memmove(dest, src, slen * sizeof(uint32_t));
    *dlen = slen;
}

static inline void big_mul_uint32(uint32_t *a, size_t *len, uint32_t m)
{
    uint64_t carry = 0;
    for (size_t i = 0; i < *len; ++i) {
        uint64_t prod = (uint64_t)a[i] * m + carry;
        a[i] = (uint32_t)(prod % BASE);
        carry = prod / BASE;
    }
    if (carry) {
        a[(*len)++] = (uint32_t)carry;
    }
}

static inline void big_add_uint32(uint32_t *a, size_t *len, uint32_t add)
{
    uint64_t carry = add;
    for (size_t i = 0; i < *len && carry; ++i) {
        uint64_t sum = (uint64_t)a[i] + carry;
        a[i] = (uint32_t)(sum % BASE);
        carry = sum / BASE;
    }
    if (carry) {
        a[(*len)++] = (uint32_t)carry;
    }
}

static inline int big_compare(const uint32_t *a, size_t la,
                              const uint32_t *b, size_t lb)
{
    while (la > 1 && a[la - 1] == 0) --la;
    while (lb > 1 && b[lb - 1] == 0) --lb;
    if (la > lb) return 1;
    if (la < lb) return -1;
    for (int64_t i = (int64_t)la - 1; i >= 0; --i) {
        if (a[i] > b[i]) return 1;
        if (a[i] < b[i]) return -1;
    }
    return 0;
}

static inline void big_sub_equals(uint32_t *a, size_t *alen,
                                  const uint32_t *b, size_t blen)
{
    uint64_t borrow = 0;
    for (size_t i = 0; i < *alen; ++i) {
        uint64_t bi = (i < blen) ? b[i] : 0;
        uint64_t sub = bi + borrow;
        if (a[i] >= sub) {
            a[i] -= sub;
            borrow = 0;
        } else {
            a[i] += BASE - sub;
            borrow = 1;
        }
    }
    while (*alen > 1 && a[*alen - 1] == 0) --(*alen);
}
/*--------------------------------------------------------------*/

seq_t a(idx_t n)
{
    const idx_t OFFSET = 6u;
    if (n < OFFSET) {
        return (seq_t)0;   /* undefined for n < 6 */
    }

    /* Index of the desired digit (0‑based) */
    size_t target = (size_t)(n - OFFSET);

    /* Initialise once */
    if (!init_done) {
        /* Integer part of sqrt */
        uint64_t sqrt_int = _floor_isqrt_u64(N_CONST);

        /* Remainder after integer part */
        uint64_t rem_u = N_CONST - sqrt_int * sqrt_int;
        big_set_uint64(rem_arr, &rem_len, rem_u);

        /* Root so far (integer part) */
        big_set_uint64(root_arr, &root_len, sqrt_int);

        /* Store integer part digits into digit_buf */
        uint8_t tmp_digits[20];
        size_t tmp_len = 0;
        uint64_t tmp = sqrt_int;
        do {
            tmp_digits[tmp_len++] = (uint8_t)(tmp % 10);
            tmp /= 10;
        } while (tmp);

        /* Reverse to most significant first */
        for (size_t i = 0; i < tmp_len / 2; ++i) {
            uint8_t t = tmp_digits[i];
            tmp_digits[i] = tmp_digits[tmp_len - 1 - i];
            tmp_digits[tmp_len - 1 - i] = t;
        }

        digit_cap = tmp_len;
        digit_buf = (uint8_t*)malloc(digit_cap);
        if (!digit_buf) return (seq_t)0; /* allocation failure */
        memcpy(digit_buf, tmp_digits, tmp_len);
        digit_len = tmp_len;

        init_done = true;
    }

    /* Generate more digits if needed */
    while (target >= digit_len) {
        /* rem *= 100 */
        big_mul_uint32(rem_arr, &rem_len, 100);

        /* cand = root * 20 */
        big_copy(cand_arr, &cand_len, root_arr, root_len);
        big_mul_uint32(cand_arr, &cand_len, 20);

        int chosen = 0;
        for (int d = 9; d >= 0; --d) {
            /* cand_plus = cand + d */
            big_copy(cand_plus_arr, &cand_plus_len, cand_arr, cand_len);
            big_add_uint32(cand_plus_arr, &cand_plus_len, (uint32_t)d);

            /* temp = cand_plus * d */
            big_copy(temp_arr, &temp_len, cand_plus_arr, cand_plus_len);
            big_mul_uint32(temp_arr, &temp_len, (uint32_t)d);

            if (big_compare(temp_arr, temp_len, rem_arr, rem_len) <= 0) {
                chosen = d;
                big_sub_equals(rem_arr, &rem_len, temp_arr, temp_len);

                /* Update root: root = root * 10 + d */
                big_mul_uint32(root_arr, &root_len, 10);
                big_add_uint32(root_arr, &root_len, (uint32_t)d);
                break;
            }
        }

        if (digit_len + 1 > digit_cap) {
            size_t new_cap = digit_cap ? digit_cap * 2 : 16;
            uint8_t *tmp = (uint8_t*)realloc(digit_buf, new_cap);
            if (!tmp) return (seq_t)0;
            digit_buf = tmp;
            digit_cap = new_cap;
        }
        digit_buf[digit_len++] = (uint8_t)chosen;
    }

    return (seq_t)digit_buf[target];
}
