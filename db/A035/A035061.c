/*
 * A035061  Numbers n such that 2^n does not contain the digit 6 (probably
 *          finite).
 * https://oeis.org/A035061
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, more
 * Formula:   not stated
 * Range:     u8 n <= 28, u16 n <= 28, u32 n <= 28, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: a(n) is the n-th exponent k such that 2^k has no digit 6
   (offset 1: a(1)=0) */

#include <stdlib.h>
#include <stdint.h>

/* One global cache: the exponents that satisfy the condition */
static seq_t *terms = NULL;          /* stored exponents a(1), a(2), …  */
static size_t terms_len = 0;         /* number of terms stored          */
static size_t terms_cap = 0;         /* capacity of the terms array     */

/* The next exponent to examine and the current value of 2^next_exp
   represented in decimal, least‑significant digit first.  */
static idx_t next_exp = 0;           /* next exponent to examine        */
static unsigned char *dec = NULL;    /* array of base‑10 digits         */
static size_t dec_len = 0;           /* number of used digits            */
static size_t dec_cap = 0;           /* capacity of the dec array       */

/* Ensure the decimal array has at least 'need' slots */
static void ensure_dec_cap(size_t need)
{
    if (need <= dec_cap) return;
    size_t newcap = dec_cap ? dec_cap << 1 : 8;
    while (newcap < need) newcap <<= 1;
    unsigned char *tmp = (unsigned char *)realloc(dec, newcap);
    if (!tmp) exit(1);                /* out of memory */
    dec = tmp;
    dec_cap = newcap;
}

/* Multiply the decimal number by 2, in place, with carry propagation */
static void dec_mul2(void)
{
    unsigned int carry = 0;
    for (size_t i = 0; i < dec_len; ++i) {
        unsigned int prod = (unsigned int)dec[i] * 2 + carry;
        dec[i] = prod % 10;
        carry = prod / 10;
    }
    while (carry) {
        ensure_dec_cap(dec_len + 1);
        dec[dec_len++] = carry % 10;
        carry /= 10;
    }
}

/* Test whether the decimal number contains a digit 6 */
static int dec_has_digit6(void)
{
    for (size_t i = 0; i < dec_len; ++i)
        if (dec[i] == 6)
            return 1;
    return 0;
}

/* --------------------------------------------------------------- */
/* Sequence generator                                               */
/* --------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n < 1) return 0;             /* undefined, but safe return */

    /* initialise structures on first call */
    if (!terms) {
        terms_cap = 8;
        terms = (seq_t *)malloc(terms_cap * sizeof(seq_t));
        if (!terms) exit(1);
    }
    if (!dec) {
        dec_cap = 8;
        dec = (unsigned char *)malloc(dec_cap * sizeof(unsigned char));
        if (!dec) exit(1);
        dec[0] = 1;                  /* 2^0 = 1 */
        dec_len = 1;
        next_exp = 0;                 /* exponent for current dec value */
    }

    /* generate terms until we have enough */
    while (terms_len < (size_t)n) {
        /* check current exponent (next_exp) */
        if (!dec_has_digit6()) {
            if (terms_len == terms_cap) {
                terms_cap <<= 1;
                seq_t *tmp = (seq_t *)realloc(terms, terms_cap * sizeof(seq_t));
                if (!tmp) exit(1);
                terms = tmp;
            }
            terms[terms_len++] = (seq_t)next_exp;
        }
        /* advance to next exponent */
        ++next_exp;
        dec_mul2();                  /* now dec holds 2^next_exp */
    }

    return terms[n - 1];
}
