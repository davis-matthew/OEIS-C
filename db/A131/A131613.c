/*
 * A131613  Numbers k such that the decimal expansion of 3^k contains no 9.
 * https://oeis.org/A131613
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, fini, nonn
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 16, u32 n <= 16, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   grow_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A131613: Numbers k such that the decimal expansion of 3^k contains no 9. */
/* The function searches for such exponents by iteratively multiplying a big
   decimal integer by 3 and checking that none of its digits is 9.  The
   results are cached so calls with arbitrary n are efficient. */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/grow_seq_array.h"

/* Array of exponents k that satisfy the condition (0‑based indexing). */
static seq_t *terms      = NULL;   /* array of exponents k */
static size_t terms_size  = 0;      /* number of stored terms   */
static size_t terms_cap   = 0;      /* capacity of terms array */

/* Current exponent and big integer representation of 3^current_k. */
static idx_t   current_k   = 0;             /* current exponent of 3 */
static uint8_t *digits      = NULL;        /* least‑significant digit first */
static size_t   digits_len  = 0;            /* number of used digits      */
static size_t   digits_cap  = 0;            /* capacity of digits array   */

/* Ensure 'digits' can hold at least 'need' digits. */
static void ensure_digits_capacity(size_t need)
{
    if (digits_cap < need) {
        size_t new_cap = digits_cap ? digits_cap * 2 : 64;
        while (new_cap < need) new_cap *= 2;
        uint8_t *tmp = (uint8_t *)realloc(digits, new_cap);
        if (!tmp) abort();           /* allocation failure: fatal */
        digits     = tmp;
        digits_cap = new_cap;
    }
}

/* Multiply the big integer by 3 in place. */
static void multiply_by_3(void)
{
    int carry = 0;
    for (size_t i = 0; i < digits_len; ++i) {
        int prod = digits[i] * 3 + carry;
        digits[i] = prod % 10;
        carry     = prod / 10;
    }
    while (carry) {
        ensure_digits_capacity(digits_len + 1);
        digits[digits_len++] = carry % 10;
        carry /= 10;
    }
}

/* Return true if the current 3^current_k contains no digit 9. */
static inline bool has_no_digit9(void)
{
    for (size_t i = 0; i < digits_len; ++i)
        if (digits[i] == 9)
            return false;
    return true;
}

/* Make sure at least 'n' terms are available; uses lazy evaluation. */
static void ensure_terms(idx_t n)
{
    if (terms_size == 0) { /* first time */
        grow_seq_array(&terms, &terms_cap, 1);
        terms[0] = (seq_t)0;     /* 3^0 = 1 has no 9 */
        terms_size = 1;

        ensure_digits_capacity(1);
        digits[0] = 1;           /* big integer = 1 */
        digits_len = 1;
        current_k = 0;
    }

    while (terms_size < (size_t)n) {
        ++current_k;
        multiply_by_3();

        if (has_no_digit9()) {
            grow_seq_array(&terms, &terms_cap, terms_size + 1);
            terms[terms_size++] = (seq_t)current_k;
        }
    }
}

/* Public accessor ----------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;           /* undefined for n < 1, return 0 as guard */
    ensure_terms(n);
    return terms[n - 1];
}
