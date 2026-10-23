/*
 * A030706  Decimal expansion of 11^n contains no zeros (probably finite).
 * https://oeis.org/A030706
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 16, u32 n <= 16, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: a(n) is the n‑th exponent k for which the decimal expansion of
   11^k contains no digit 0. The sequence is built lazily and cached,
   so repeated calls with arbitrary ordering of n return correctly. */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* dynamic array of decimal digits (least‑significant digit first) */
static uint8_t *digits = NULL;
static size_t     dlen  = 0;   /* number of used digits */
static size_t     dcap  = 0;   /* allocated capacity */

/* dynamic array of seen exponents (i.e. the sequence values) */
static idx_t *valid_n = NULL;
static size_t valid_size = 0;
static size_t valid_cap  = 0;

/* current exponent being considered */
static idx_t cur_k = 0;

/* enlarge a dynamic array of digits to at least new_cap */
static void ensure_digit_cap(size_t new_cap)
{
    if (new_cap > dcap) {
        dcap = new_cap;
        uint8_t *tmp = (uint8_t *)realloc(digits, dcap * sizeof(uint8_t));
        digits = tmp;
    }
}

/* multiply the big decimal number stored in digits by 11 */
static void mul11(void)
{
    int carry = 0;
    for (size_t i = 0; i < dlen; ++i) {
        int prod = (int)digits[i] * 11 + carry;
        digits[i] = (uint8_t)(prod % 10);
        carry     = prod / 10;
    }
    while (carry > 0) {
        if (dlen == dcap)
            ensure_digit_cap(dcap * 2 + 1);
        digits[dlen++] = (uint8_t)(carry % 10);
        carry /= 10;
    }
}

/* enlarge the exponents array to at least new_cap */
static void ensure_exponents_cap(size_t new_cap)
{
    if (new_cap > valid_cap) {
        valid_cap = new_cap;
        idx_t *tmp = (idx_t *)realloc(valid_n, valid_cap * sizeof(idx_t));
        valid_n = tmp;
    }
}

/* make sure that at least n terms of the sequence are available */
static void ensure_terms(idx_t n)
{
    if (valid_n == NULL) {
        /* initial allocation */
        dcap  = 32;
        digits = (uint8_t *)malloc(dcap * sizeof(uint8_t));
        digits[0] = 1;          /* 11^0 = 1 */
        dlen    = 1;
        cur_k   = 0;

        valid_cap = 8;
        valid_n = (idx_t *)malloc(valid_cap * sizeof(idx_t));
        valid_n[0] = cur_k;     /* a(1) = 0 */
        valid_size = 1;
    }

    while (valid_size < n) {
        /* move to next power of 11 */
        ++cur_k;
        mul11();

        /* check for zero digits */
        bool has_zero = false;
        for (size_t i = 0; i < dlen; ++i) {
            if (digits[i] == 0) {
                has_zero = true;
                break;
            }
        }

        if (!has_zero) {
            if (valid_size == valid_cap)
                ensure_exponents_cap(valid_cap * 2 + 1);
            valid_n[valid_size++] = cur_k;
        }
    }
}

/* public entry point for the sequence */
seq_t a(idx_t n)
{
    if (n == 0) return 0;             /* undefined below offset */
    ensure_terms(n);
    return (seq_t)valid_n[n - 1];
}
