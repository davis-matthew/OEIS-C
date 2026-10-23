/*
 * A177345  Decimal expansion of sqrt(2805).
 * https://oeis.org/A177345
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  cons, nonn
 * Formula:   not stated
 * Range:     u8 n <= 106, u16 n <= 106, u32 n <= 106, u64 n <= 106,
 *            u128 n <= 106, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Decimal expansion of sqrt(2805).  a(2) = first digit of the integer part (5),
 * a(3) = second digit (2).  Subsequent terms are digits after the decimal point.
 * The implementation uses a digit‑by‑digit algorithm for square roots, keeping
 * only the remainder (r) and the value 20·root (C) as big integers
 * represented in base 1 000 000 000.  This works for both fixed‑width
 * (e.g., uint64_t) and arbitrary‑precision seq_t builds. */

#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

/* Symbolic types supplied by the build */
extern seq_t a(idx_t n);

/* Base for internal big integer representation: 10^9 */
#define BIG_BASE 1000000000U
#define BIG_BASE_DIGITS 9

/* ------------------------------------------------------------------------- */
/* Big integer helpers (little‑endian arrays, base 10^9)                     */
/* ------------------------------------------------------------------------- */
static inline void big_mul_small(uint32_t *digits, size_t *len, uint32_t m)
{
    uint64_t carry = 0;
    for (size_t i = 0; i < *len; ++i) {
        uint64_t t = (uint64_t)digits[i] * m + carry;
        digits[i] = (uint32_t)(t % BIG_BASE);
        carry = t / BIG_BASE;
    }
    if (carry) {
        digits[*len] = (uint32_t)carry;
        ++*len;
    }
}

static inline void big_add_small(uint32_t *digits, size_t *len, uint32_t a)
{
    uint64_t t = (uint64_t)digits[0] + a;
    digits[0] = (uint32_t)(t % BIG_BASE);
    uint64_t carry = t / BIG_BASE;
    size_t i = 1;
    while (carry) {
        if (i == *len) {
            digits[i] = (uint32_t)carry;
            ++*len;
            break;
        }
        t = (uint64_t)digits[i] + carry;
        digits[i] = (uint32_t)(t % BIG_BASE);
        carry = t / BIG_BASE;
        ++i;
    }
}

/* r = a - b,  a >= b  */
static inline void big_sub(uint32_t *r, const uint32_t *a, size_t alen,
                           const uint32_t *b, size_t blen)
{
    int64_t borrow = 0;
    for (size_t i = 0; i < alen; ++i) {
        int64_t ai = a[i];
        int64_t bi = (i < blen) ? b[i] : 0;
        int64_t diff = ai - bi - borrow;
        if (diff < 0) {
            diff += BIG_BASE;
            borrow = 1;
        } else
            borrow = 0;
        r[i] = (uint32_t)diff;
    }
    /* trim leading zeros */
    size_t i = alen;
    while (i > 1 && r[i - 1] == 0)
        --i;
    /* copy back */
    memcpy(r, r, i * sizeof(uint32_t));
}

static inline int big_cmp(const uint32_t *a, size_t alen,
                          const uint32_t *b, size_t blen)
{
    if (alen != blen)
        return (alen < blen) ? -1 : 1;
    for (size_t i = alen; i-- > 0; ) {
        if (a[i] != b[i])
            return (a[i] < b[i]) ? -1 : 1;
    }
    return 0;
}

/* ------------------------------------------------------------------------- */
/* State of the digit generation process                                     */
/* ------------------------------------------------------------------------- */
#define MAX_DIGITS 20000          /* enough for n up to ~20000 */

static unsigned char digit_cache[MAX_DIGITS];
static size_t digit_cache_len = 0;  /* number of digits stored */

/* Big integers used by the algorithm */
#define BIG_BUF_SIZE 2048
static uint32_t C_digits[BIG_BUF_SIZE]; /* 20 * root */
static size_t C_len;
static uint32_t R_digits[BIG_BUF_SIZE]; /* remainder */
static size_t R_len;

/* Temporary buffer for intermediate calculations */
static uint32_t temp_digits[BIG_BUF_SIZE];
static size_t temp_len;

/* ------------------------------------------------------------------------- */
/* Initialization of the sequence                                             */
/* ------------------------------------------------------------------------- */
static void seq_init(void)
{
    if (digit_cache_len)
        return;   /* already initialized */

    /* Integer part of sqrt(2805) */
    const uint32_t int_root = 52;
    digit_cache[0] = 5;            /* first integer digit */
    digit_cache[1] = 2;            /* second integer digit */
    digit_cache_len = 2;

    /* Remainder after subtracting int_root^2 = 2704 from 2805 */
    R_digits[0] = 101;
    R_len = 1;

    /* C = 20 * int_root */
    C_digits[0] = int_root * 20;   /* 1040 fits in one word */
    C_len = 1;
}

/* ------------------------------------------------------------------------- */
/* Ensure that we have computed digits up to index idx (0‑based).             */
/* idx=0 refers to a(2), idx=1 to a(3), ...                                   */
/* ------------------------------------------------------------------------- */
static void seq_ensure_len(size_t idx)
{
    seq_init();

    while (digit_cache_len <= idx) {
        /* Bring down next pair of zeros: R = R * 100 */
        big_mul_small(R_digits, &R_len, 100);

        /* Find largest d with (C + d) * d <= R,  d=0..9 */
        int dchosen = 0;
        for (int d = 9; d >= 0; --d) {
            /* temp = C + d */
            memcpy(temp_digits, C_digits, C_len * sizeof(uint32_t));
            temp_len = C_len;
            big_add_small(temp_digits, &temp_len, d);

            /* temp = temp * d */
            big_mul_small(temp_digits, &temp_len, (uint32_t)d);

            if (big_cmp(temp_digits, temp_len, R_digits, R_len) <= 0) {
                dchosen = d;
                break;
            }
        }

        /* Compute product = (C + dchosen) * dchosen */
        memcpy(temp_digits, C_digits, C_len * sizeof(uint32_t));
        temp_len = C_len;
        big_add_small(temp_digits, &temp_len, (uint32_t)dchosen);
        big_mul_small(temp_digits, &temp_len, (uint32_t)dchosen);

        /* R = R - product */
        big_sub(R_digits, R_digits, R_len, temp_digits, temp_len);
        R_len = (R_len > 1 && R_digits[R_len - 1] == 0) ? R_len - 1 : R_len;

        /* Update C: C = C * 10 + 20*dchosen */
        big_mul_small(C_digits, &C_len, 10);
        big_add_small(C_digits, &C_len, 20U * (uint32_t)dchosen);

        /* Store the chosen digit */
        if (digit_cache_len < MAX_DIGITS)
            digit_cache[digit_cache_len++] = (unsigned char)dchosen;
    }
}

/* ------------------------------------------------------------------------- */
/* Main sequence function a(n).  For n < 2 return 0 as undefined.           */
/* ------------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n < 2)
        return 0;  /* undefined below offset 2 */

    size_t idx = (size_t)(n - 2);   /* zero‑based index into cache */
    seq_ensure_len(idx);
    return (seq_t)digit_cache[idx];
}
