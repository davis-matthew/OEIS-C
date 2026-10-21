/*
 * A050729  Decimal expansion of 8^n contains no pair of consecutive equal
 *          digits (probably finite).
 * https://oeis.org/A050729
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 14, u32 n <= 14, u64 n <= 14, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Sequence A050729:
 * a(n) is the n-th exponent k such that the decimal expansion of 8^k contains no pair of
 * consecutive equal digits.  a(0)=0, a(1)=1, ...
 *
 * Implementation: incrementally multiply the decimal representation of 8^k by 8.
 * The number is kept in an array of decimal digits (least‑significant first).
 * After each multiplication we check the digits for consecutive equal values.
 * All calculations are done with native integer types, so no overflow occurs
 * regardless of the width of seq_t.  The sequence of exponents is cached
 * in a static array so that repeated calls are fast.
 *
 * This satisfies all build configurations (uint8_t … uint64_t or arbitrary precision)
 * and does not rely on any non‑standard utilities.
 */

#include <stdlib.h>
#include <stdbool.h>

/* ------------------------------------------------------------ */
/* -------------- static state for the incremental generator  -------------- */
/* ------------------------------------------------------------ */

static idx_t *a_terms = NULL;   /* array of exponents that satisfy the condition */
static size_t a_len  = 0;       /* number of stored terms */
static size_t a_cap  = 0;       /* current capacity of a_terms */

/* current decimal representation of 8^next_k   (least‑significant digit first) */
static uint8_t *digits      = NULL;
static size_t   digits_len  = 0;
static size_t   digits_cap  = 0;
static idx_t    next_k      = 0;      /* exponent to test next (starts at 0) */

/* ------------------------------------------------------------ */
/* -------------- helper to grow the exponent cache  --------- */
/* ------------------------------------------------------------ */
static void ensure_cap_terms(size_t needed)
{
    if (needed <= a_cap) return;
    size_t new_cap = a_cap ? a_cap << 1 : 16;
    if (new_cap < needed) new_cap = needed;
    idx_t *tmp = (idx_t *)realloc(a_terms, new_cap * sizeof(idx_t));
    a_terms = tmp;
    a_cap   = new_cap;
}

/* ------------------------------------------------------------ */
/* -------------- helper to grow the digit array ------------- */
/* ------------------------------------------------------------ */
static void ensure_cap_digits(size_t needed)
{
    if (needed <= digits_cap) return;
    size_t new_cap = digits_cap ? digits_cap << 1 : 16;
    if (new_cap < needed) new_cap = needed;
    uint8_t *tmp = (uint8_t *)realloc(digits, new_cap * sizeof(uint8_t));
    digits = tmp;
    digits_cap = new_cap;
}

/* ------------------------------------------------------------ */
/* -------------- check for consecutive equal digits --------- */
/* ------------------------------------------------------------ */
static bool has_consecutive_equal_digits(void)
{
    for (size_t i = digits_len; i > 1; --i) {
        if (digits[i-1] == digits[i-2]) return true;
    }
    return false;
}

/* ------------------------------------------------------------ */
/* -------------- main sequence function --------------------- */
/* ------------------------------------------------------------ */
seq_t a(idx_t n)
{
    /* initialise the decimal representation of 8^0 = 1 */
    if (digits_len == 0) {
        digits_len  = 1;
        digits_cap  = 8;
        digits      = (uint8_t *)malloc(digits_cap * sizeof(uint8_t));
        digits[0]   = 1;          /* 8^0 = 1 */
    }

    /* generate exponents until we have a(n) ready */
    while (a_len <= n) {
        /* test current 8^next_k for the property */
        if (!has_consecutive_equal_digits()) {
            ensure_cap_terms(a_len + 1);
            a_terms[a_len++] = next_k;
        }

        /* multiply current number by 8 to get 8^(next_k+1) */
        uint8_t carry = 0;
        for (size_t i = 0; i < digits_len; ++i) {
            uint16_t prod = (uint16_t)digits[i] * 8 + carry; /* prod fits in 16 bits */
            digits[i]     = prod % 10;
            carry         = prod / 10;
        }
        while (carry > 0) {
            ensure_cap_digits(digits_len + 1);
            digits[digits_len++] = carry % 10;
            carry /= 10;
        }

        ++next_k;
    }

    return (seq_t)a_terms[n];
}
