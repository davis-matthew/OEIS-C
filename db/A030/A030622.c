/*
 * A030622  Powers of 2 grouped in pairs of two digits (version 1).
 * https://oeis.org/A030622
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = two‑digit group formed from the decimal representation of
 *            2^k where k is the smallest integer such that the sequence of
 *            decimal digits of 2^1, 2^2, …, 2^k has at least 2*n digits. When
 *            the leading digit of a group is 0, the group is interpreted as
 *            an integer without that leading zero
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>

/* Working buffer for decimal digits of successive powers of two.
   g_digits contains each digit as a byte 0–9. */
static unsigned char *g_digits = NULL;
static size_t g_digit_cnt = 0;     /* number of digits stored */
static size_t g_digit_cap = 0;     /* allocated capacity */

/* Big‑integer representation of the current power of two.
   Stored in base 1e9, little endian. */
static uint32_t pow2_limbs[256] = {0};
static size_t pow2_len = 0;        /* number of limbs used */

/* Number of a(n) terms that have already been computed. */
static idx_t g_max_pairs_computed = 0;

/* Base for limb arithmetic: 10^9. */
#define BASE 1000000000U

/* Append 'len' characters from 'buf' (each '0'‑'9') to g_digits.  */
static void append_digits(const char *buf, size_t len)
{
    if (g_digit_cnt + len > g_digit_cap) {
        size_t new_cap = (g_digit_cap ? g_digit_cap * 2 : 512);
        while (new_cap < g_digit_cnt + len) new_cap *= 2;
        unsigned char *tmp = (unsigned char *)realloc(g_digits, new_cap);
        if (!tmp) { free(g_digits); exit(1); }
        g_digits = tmp;
        g_digit_cap = new_cap;
    }
    for (size_t i = 0; i < len; ++i) g_digits[g_digit_cnt++] = (unsigned char)(buf[i] - '0');
}

/* Double the current 2^k represented in pow2_limbs. */
static void double_pow2(void)
{
    uint64_t carry = 0;
    for (size_t i = 0; i < pow2_len; ++i) {
        uint64_t t = (uint64_t)pow2_limbs[i] * 2 + carry;
        pow2_limbs[i] = (uint32_t)(t % BASE);
        carry = t / BASE;
    }
    if (carry) {
        if (pow2_len == 256) exit(1);  /* safety check */
        pow2_limbs[pow2_len++] = (uint32_t)carry;
    }
}

/* Convert current 2^k in pow2_limbs to decimal digits and append them. */
static void add_power_digits(void)
{
    char buf[12];
    /* Most significant limb – no leading zeros. */
    size_t ms_pos = pow2_len - 1;
    int ms_len = snprintf(buf, sizeof(buf), "%u", pow2_limbs[ms_pos]);
    append_digits(buf, ms_len);
    /* Remaining limbs – always 9 digits with leading zeros. */
    for (size_t i = ms_pos; i-- > 0; ) {
        int len = snprintf(buf, sizeof(buf), "%09u", pow2_limbs[i]);
        append_digits(buf, len);
    }
}

/* Ensure that a(1)…a(n) are available.  */
static void ensure_terms(idx_t n)
{
    while (g_max_pairs_computed < n) {
        size_t needed_digits = 2 * (size_t)(g_max_pairs_computed + 1);
        while (g_digit_cnt < needed_digits) {
            if (pow2_len == 0) {          /* first power 2^1 = 2 */
                pow2_limbs[0] = 2;
                pow2_len = 1;
            } else {
                double_pow2();
            }
            add_power_digits();
        }
        ++g_max_pairs_computed;
    }
}

/* Main function required by the problem statement. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined; safety guard */

    ensure_terms(n);

    size_t idx = 2 * (size_t)(n - 1);
    unsigned char d1 = g_digits[idx];
    unsigned char d2 = g_digits[idx + 1];
    unsigned int val = d1 * 10 + d2;
    if (d1 == 0) val = d2;          /* ignore leading zero of the pair */
    return (seq_t)val;
}
