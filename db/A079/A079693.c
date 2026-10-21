/*
 * A079693  Number of 8's in n!.
 * https://oeis.org/A079693
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = number of 8's in the decimal representation of n!
 * Range:     u8 n <= 967, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation maintains an arbitrary‑length decimal big integer
   (stored least‑significant digit first) and counts the digit '8'
   after each multiplication.  Because the factorial grows very quickly,
   the decimal representation is dynamically expanded as needed.
   The algorithm is O(n * digits(n)) and is fast enough for all
   n tested (including n up to about 10 000). */

#include <stddef.h>   /* size_t */
#include <stdint.h>   /* uint8_t, uint32_t, uint64_t */
#include <stdlib.h>   /* malloc, realloc */

static unsigned char *digits      = NULL;   /* decimal digits (LSB first) */
static size_t      digits_len    = 0;       /* current number of digits  */
static size_t      digits_cap    = 0;       /* allocated capacity        */

static seq_t    *count8  = NULL;   /* a(n) values */
static size_t     cnt_len = 0;       /* number of computed a(n) */
static size_t     cnt_cap = 0;       /* capacity of count8  */

static void ensure_digits_cap(size_t min_cap)
{
    if (digits_cap >= min_cap) return;
    size_t new_cap = digits_cap ? digits_cap << 1 : 16;
    while (new_cap < min_cap) new_cap <<= 1;
    digits = (unsigned char *)realloc(digits, new_cap * sizeof(unsigned char));
    digits_cap = new_cap;
}

static void ensure_cnt_cap(size_t min_cap)
{
    if (cnt_cap >= min_cap) return;
    size_t new_cap = cnt_cap ? cnt_cap << 1 : 16;
    while (new_cap < min_cap) new_cap <<= 1;
    count8 = (seq_t *)realloc(count8, new_cap * sizeof(seq_t));
    cnt_cap = new_cap;
}

/* Initialize static state on first call */
static void init_state(void)
{
    if (digits_len != 0) return;   /* already initialized */

    ensure_digits_cap(16);
    digits_len = 1;
    digits[0]  = 1;                 /* 0! = 1 */

    ensure_cnt_cap(16);
    cnt_len    = 1;                 /* a(0) */
    count8[0]  = (seq_t)0;          /* no '8' in "1" */
}

/* Multiply current factorial by i (i > 0) and update digits array */
static void multiply_by(idx_t i)
{
    uint64_t carry = 0;
    for (size_t j = 0; j < digits_len; ++j) {
        uint64_t prod   = (uint64_t)digits[j] * (uint64_t)i + carry;
        digits[j]        = (unsigned char)(prod % 10);
        carry            = prod / 10;
    }
    while (carry > 0) {
        ensure_digits_cap(digits_len + 1);
        digits[digits_len++] = (unsigned char)(carry % 10);
        carry /= 10;
    }
}

/* Count how many digits equal to 8 in current digits array */
static seq_t count_eights(void)
{
    seq_t c = 0;
    for (size_t j = 0; j < digits_len; ++j)
        if (digits[j] == 8) ++c;
    return c;
}

/* Public function required by the test harness */
seq_t a(idx_t n)
{
    init_state();

    /* Extend factorials up to n */
    while (cnt_len <= (size_t)n) {
        idx_t i = cnt_len;           /* next multiplier (since cnt_len == current n) */
        multiply_by(i);
        ensure_cnt_cap(cnt_len + 1);
        count8[cnt_len] = count_eights();
        ++cnt_len;
    }
    return count8[(size_t)n];
}
