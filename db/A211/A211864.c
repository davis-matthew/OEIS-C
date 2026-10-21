/*
 * A211864  Powers of three read in base 2.
 * https://oeis.org/A211864
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 28, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Powers of three read in base 2
   a(n) is obtained by multiplying 3 by itself n times,
   recording the decimal digits of 3^n, then interpreting those
   digits as a base‑2 number (digits may exceed 1, so a carry
   is applied during the base‑2 conversion). */
#include <stdint.h>
#include <stddef.h>

/* ---------- static workspace ---------- */
static seq_t a_cache[20001];        /* store a(k) for k <= current max  */
static int  initialized = 0;        /* 0 = uninitialised, 1 = initialised */
static idx_t max_n = 0;             /* highest n already computed */

/* decimal digits of 3^max_n , least significant first */
static uint8_t digits[50000];
static size_t digits_len = 0;       /* number of digits in current power */

/* ---------- helper to initialise the workspace ---------- */
static void init(void)
{
    if (initialized)
        return;
    a_cache[0] = (seq_t)1;         /* a(0) = 1 */
    digits[0] = 1;                 /* 3^0 = 1 */
    digits_len = 1;
    max_n = 0;
    initialized = 1;
}

/* ---------- helper to multiply the current decimal number by 3 ---------- */
static void mul3(void)
{
    uint16_t carry = 0;
    for (size_t i = 0; i < digits_len; ++i) {
        uint16_t tmp = digits[i] * 3u + carry;
        digits[i] = tmp % 10u;
        carry    = tmp / 10u;
    }
    if (carry) {
        /* carry <= 2 for multiplication by 3 */
        digits[digits_len++] = (uint8_t)carry;
    }
}

/* ---------- helper to compute a(k) from the current decimal digits ---------- */
static seq_t make_value(void)
{
    seq_t val = 0;
    /* loop from most significant digit to least */
    for (size_t i = 0; i < digits_len; ++i)
        val = (val << 1) + (seq_t)digits[digits_len - 1 - i];
    return val;
}

/* ---------- ensure that a(n) is computed and stored in a_cache ---------- */
static void compute_up_to(idx_t n)
{
    if (n <= max_n || n > 20000)
        return;  /* nothing to do or out of bounds */

    for (idx_t k = max_n + 1; k <= n; ++k) {
        mul3();                     /* now digits hold 3^k */
        a_cache[(size_t)k] = make_value();
    }
    max_n = n;
}

/* ---------- public sequence function ---------- */
seq_t a(idx_t n)
{
    init();                /* initialise workspace */
    compute_up_to(n);      /* make sure a(n) is available */
    return a_cache[(size_t)n];
}
