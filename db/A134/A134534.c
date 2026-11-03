/*
 * A134534  Numbers n such that the sum of the digits of 11^n is prime.
 * https://oeis.org/A134534
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 76 terms through n = 995 (2026-11-03)
 * Helpers:   is_prime_seq, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes sequence A134534: numbers n such that the sum of decimal digits of 11^n is prime. */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/is_prime_seq.h"
#include "../utils/mem.h"

/* Append a value to the cached array, expanding it if necessary.  `p` is a
   pointer to the array pointer. */
static void append_candidate(seq_t **p, size_t *len, size_t *cap, seq_t val)
{
    if (*len == *cap) {
        size_t newcap = (*cap == 0) ? 16 : (*cap << 1);
        seq_t *tmp = mem_resize(seq_t, *p, newcap);
        if (!tmp) {
            exit(EXIT_FAILURE);
        }
        *p = tmp;
        *cap = newcap;
    }
    (*p)[*len] = val;
    (*len)++;
}

static void multiply_by_11(uint8_t *digs, size_t *len, size_t *cap)
{
    uint16_t carry = 0;
    for (size_t i = 0; i < *len; ++i) {
        uint16_t prod = (uint16_t)digs[i] * 11 + carry;
        digs[i] = (uint8_t)(prod % 10);
        carry   = prod / 10;
    }
    while (carry) {
        if (*len == *cap) {
            *cap *= 2;
            digs = (uint8_t*)realloc(digs, *cap);
        }
        digs[*len] = (uint8_t)(carry % 10);
        carry      = carry / 10;
        ++(*len);
    }
}

static seq_t digit_sum_array(const uint8_t *digs, size_t len)
{
    uint32_t sum = 0;                  /* sum fits in 32‑bit for our sizes */
    for (size_t i = 0; i < len; ++i)
        sum += digs[i];
    return (seq_t)sum;
}

seq_t a(idx_t n)
{
    /* static state for memoisation */
    static bool initialized = false;
    static seq_t *terms = NULL;
    static size_t terms_len = 0, terms_cap = 0;
    static uint8_t *digits = NULL;
    static size_t digits_len = 0, digits_cap = 0;
    static idx_t next_k = 0;            /* power currently equals 11^next_k */

    if (!initialized) {
        /* allocate storage for terms */
        terms_cap = 16;
        terms = mem_new(seq_t, terms_cap);

        /* initialise decimal digits array for 11^0 = 1 */
        digits_cap = 32;
        digits = (uint8_t*)malloc(digits_cap);
        digits[0] = 1;
        digits_len = 1;

        initialized = true;
    }

    /* generate more terms until we have at least n */
    while (terms_len < n) {
        /* multiply current power by 11 to get 11^(next_k+1) */
        if (digits_len == digits_cap) {
            digits_cap *= 2;
            digits = (uint8_t*)realloc(digits, digits_cap);
        }
        multiply_by_11(digits, &digits_len, &digits_cap);
        ++next_k;                             /* now represents exponent next_k */

        /* compute digit sum */
        seq_t dsum = digit_sum_array(digits, digits_len);

        /* test primality of the digit sum */
        if (is_prime_seq(dsum))
            append_candidate(&terms, &terms_len, &terms_cap, (seq_t)next_k);
    }

    return terms[n - 1];
}
