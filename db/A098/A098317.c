/*
 * A098317  Decimal expansion of phi^3 = 2 + sqrt(5).
 * https://oeis.org/A098317
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, cons, easy, changed
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A098317: decimal expansion of 2+sqrt(5) (phi^3)
 *  a(1) = 4
 *  a(n) = digit at position n-1 in the decimal expansion of sqrt(5)
 *  (for n>=2).  The digit is found by extracting the decimal digits of
 *  sqrt(5) using a big‑integer digit‑by‑digit square‑root algorithm.
 */

#include <stdint.h>
#include <stddef.h>

#define MAX_WORDS 1300          /* enough for 11000 decimal digits */
#define BASE      1000000000u   /* 1e9 */

typedef struct {
    size_t len;
    uint32_t d[MAX_WORDS];     /* least‑significant word first */
} Big;

/* Initialise Big from small integer */
static void big_set_uint(Big *a, uint32_t v)
{
    a->len = 0;
    if (v > 0) {
        a->d[0] = v;
        a->len = 1;
    }
}

/* Trim leading zero words */
static void big_trim(Big *a)
{
    while (a->len > 0 && a->d[a->len - 1] == 0)
        a->len--;
}

/* Compare a and b: return -1 if a<b, 0 if a==b, +1 if a>b */
static int big_cmp(const Big *a, const Big *b)
{
    if (a->len != b->len)
        return (a->len < b->len) ? -1 : 1;
    for (size_t i = a->len; i-- > 0;) {
        if (a->d[i] != b->d[i])
            return (a->d[i] < b->d[i]) ? -1 : 1;
    }
    return 0;
}

/* a *= m (small) */
static void big_mul_small(Big *a, uint32_t m)
{
    if (m == 0 || a->len == 0) {
        a->len = 0;
        return;
    }
    uint64_t carry = 0;
    for (size_t i = 0; i < a->len; ++i) {
        uint64_t prod = (uint64_t)a->d[i] * m + carry;
        a->d[i] = (uint32_t)(prod % BASE);
        carry   = prod / BASE;
    }
    if (carry) {
        if (a->len < MAX_WORDS) {
            a->d[a->len++] = (uint32_t)carry;
        }
    }
}

/* a += m (small) */
static void big_add_small(Big *a, uint32_t m)
{
    if (m == 0)
        return;
    if (a->len == 0) {
        a->d[0] = m;
        a->len = 1;
        return;
    }
    uint64_t sum = (uint64_t)a->d[0] + m;
    a->d[0] = (uint32_t)(sum % BASE);
    uint64_t carry = sum / BASE;
    size_t i = 1;
    while (carry && i < a->len) {
        sum = (uint64_t)a->d[i] + carry;
        a->d[i] = (uint32_t)(sum % BASE);
        carry   = sum / BASE;
        ++i;
    }
    if (carry) {
        if (a->len < MAX_WORDS)
            a->d[a->len++] = (uint32_t)carry;
    }
}

/* a -= b; assume a >= b */
static void big_sub(Big *a, const Big *b)
{
    uint64_t borrow = 0;
    for (size_t i = 0; i < a->len; ++i) {
        uint64_t aval = a->d[i];
        uint64_t bval = (i < b->len) ? b->d[i] : 0;
        uint64_t sub  = aval - bval - borrow;
        if (aval < bval + borrow) {
            sub += BASE;
            borrow = 1;
        } else
            borrow = 0;
        a->d[i] = (uint32_t)sub;
    }
    big_trim(a);
}

/* Copy Big b into a */
static void big_copy(Big *a, const Big *b)
{
    a->len = b->len;
    for (size_t i = 0; i < b->len; ++i)
        a->d[i] = b->d[i];
}

/* Static state for digit extraction */
static uint8_t sqrt5_digits[10000];  /* digits after decimal point of sqrt(5) */
static size_t sqrt5_digit_len = 0;

static Big curr;          /* integer part of sqrt(5) digits so far */
static Big remainder;     /* remainder in the extraction algorithm */

/* Initialise static big integers once */
static void init_sqrt5_extractor(void)
{
    /* integer part of sqrt(5) is 2 */
    big_set_uint(&curr, 2);
    /* remainder = 5 - 2^2 = 1 */
    big_set_uint(&remainder, 1);
}

/* Generate digits up to 'need' (number of decimal digits) */
static void ensure_sqrt5_digits(size_t need)
{
    if (sqrt5_digit_len >= need)
        return;
    if (sqrt5_digit_len == 0)
        init_sqrt5_extractor();

    Big temp, temp2, candidate;
    for (; sqrt5_digit_len < need; ++sqrt5_digit_len) {
        /* bring down two zeros: remainder *= 100 */
        big_mul_small(&remainder, 100);

        /* temp = curr * 20 */
        big_copy(&temp, &curr);
        big_mul_small(&temp, 20);

        int digit = 0;
        for (int x = 9; x >= 0; --x) {
            big_copy(&temp2, &temp);
            big_add_small(&temp2, (uint32_t)x);
            big_copy(&candidate, &temp2);
            big_mul_small(&candidate, (uint32_t)x);
            if (big_cmp(&candidate, &remainder) <= 0) {
                digit = x;
                big_sub(&remainder, &candidate);
                break;
            }
        }

        /* curr = curr * 10 + digit */
        big_mul_small(&curr, 10);
        big_add_small(&curr, (uint32_t)digit);

        sqrt5_digits[sqrt5_digit_len] = (uint8_t)digit;
    }
}

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)4;
    size_t need = (size_t)(n - 1);    /* number of sqrt(5) digits required */
    ensure_sqrt5_digits(need);
    return (seq_t)sqrt5_digits[need - 1];
}
