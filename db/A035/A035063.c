/*
 * A035063  Numbers n such that 2^n does not contain the digit 8 (probably
 *          finite).
 * https://oeis.org/A035063
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, more
 * Formula:   not stated
 * Range:     u8 n <= 26, u16 n <= 26, u32 n <= 26, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n): the n‑th integer k such that 2^k does not contain the digit 8.
   The function iteratively multiplies the current power of two by 2 and checks
   its decimal representation.  Exponents are stored in a static array so that
   repeated calls are efficient, and the routine works for any build in which
   seq_t and idx_t are integer types. */
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

#define BASE 1000000000ULL          /* 10^9, used for the big‑integer base */

/*------------------------------------------------------------------*/
/* Big integer stored in base 1e9, little‑endian (digits[0] is least significant) */
static uint32_t *bi_digits = NULL;   /* array of digits */
static size_t   bi_len    = 0;       /* number of used digits   */
static size_t   bi_cap    = 0;       /* allocated capacity     */

/*------------------------------------------------------------------*/
/* Array of exponents that satisfy the condition */
static seq_t *exponents = NULL;      /* array of found exponents */
static size_t exponents_len = 0;     /* number of exponents stored */
static size_t exponents_cap = 0;     /* allocated capacity */

/*------------------------------------------------------------------*/
static void ensure_bi_capacity(size_t needed)
{
    if (needed > bi_cap) {
        size_t new_cap = bi_cap ? bi_cap * 2 : 8;
        while (new_cap < needed) new_cap *= 2;
        uint32_t *tmp = (uint32_t *)realloc(bi_digits, new_cap * sizeof(uint32_t));
        if (!tmp) abort();                /* allocation failure */
        bi_digits = tmp;
        bi_cap = new_cap;
    }
}

/*------------------------------------------------------------------*/
static void ensure_exp_capacity(size_t needed)
{
    if (needed > exponents_cap) {
        size_t new_cap = exponents_cap ? exponents_cap * 2 : 8;
        while (new_cap < needed) new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(exponents, new_cap * sizeof(seq_t));
        if (!tmp) abort();               /* allocation failure */
        exponents = tmp;
        exponents_cap = new_cap;
    }
}

/*------------------------------------------------------------------*/
/* Multiply the current big integer by two.  bi_len is updated. */
static void bi_mul2(void)
{
    uint64_t carry = 0;
    for (size_t i = 0; i < bi_len; ++i) {
        uint64_t prod = (uint64_t)bi_digits[i] * 2 + carry;
        bi_digits[i] = (uint32_t)(prod % BASE);
        carry        = prod / BASE;
    }
    if (carry > 0) {
        if (bi_len == bi_cap) ensure_bi_capacity(bi_len + 1);
        bi_digits[bi_len++] = (uint32_t)carry;
    }
}

/*------------------------------------------------------------------*/
/* Returns true if the current big integer contains no decimal digit 8. */
static bool bi_has_no_digit8(void)
{
    for (size_t i = 0; i < bi_len; ++i) {
        uint32_t w = bi_digits[i];
        if (w == 0) continue;
        while (w > 0) {
            if (w % 10 == 8) return false;
            w /= 10;
        }
    }
    return true;
}

/*------------------------------------------------------------------*/
/*------------------------------------------------------------------*/
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;    /* safety – offset is 1 */

    /* Initialise data structures on first call */
    if (exponents_len == 0) {
        /* big integer starts at 2^0 = 1 */
        ensure_bi_capacity(8);
        bi_digits[0] = 1;
        bi_len = 1;

        /* first exponent is 0 (for 2^0 = 1) */
        ensure_exp_capacity(8);
        exponents[0] = (seq_t)0;
        exponents_len = 1;
    }

    /* Generate exponents until we have at least n of them */
    static idx_t current_exp = 0;   /* corresponds to the exponent of bi */
    while (exponents_len < n) {
        bi_mul2();              /* bi now represents 2^(current_exp+1) */
        ++current_exp;

        if (bi_has_no_digit8()) {
            ensure_exp_capacity(exponents_len + 1);
            exponents[exponents_len++] = (seq_t)current_exp;
        }
    }

    return exponents[n - 1];
}
