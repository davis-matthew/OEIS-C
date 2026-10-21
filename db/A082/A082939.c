/*
 * A082939  Numbers such that sum of the digits of the product of the
 *          factorial of digits of the number is equal to the sum of the
 *          digits of the number.
 * https://oeis.org/A082939
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 27, u16 n <= 2076, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   mult_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   OEIS A082939: numbers k such that the sum of the digits of k
   equals the sum of the digits of the product of the factorials
   of the digits of k.
   The function performs a brute‑force search for such numbers,
   caching all results so that repeated calls to a(n) are fast.
*/

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/mult_small.h"

/* Pre‑computed factorials of 0‑9 */
static const uint32_t fact[10] = {
    1,        /* 0! */
    1,        /* 1! */
    2,        /* 2! */
    6,        /* 3! */
    24,       /* 4! */
    120,      /* 5! */
    720,      /* 6! */
    5040,     /* 7! */
    40320,    /* 8! */
    362880    /* 9! */
};

/* Compute the sum of the digits of the product of the factorials
   of the digits of k.  The calculation is done in decimal form to
   avoid overflow on ordinary integer types. */
static uint32_t product_digit_sum(idx_t k)
{
    /* Extract digits of k (stored reversed). 20 digits suffice for 64‑bit. */
    int digit_buf[20];
    int nd = 0;
    if (k == 0) {
        digit_buf[0] = 0;
        nd = 1;
    } else {
        idx_t t = k;
        while (t) {
            digit_buf[nd++] = t % 10;
            t /= 10;
        }
    }

    /* Start product = 1 as decimal digits array. */
    unsigned char prod[200]; /* 200 decimal digits is more than enough */
    int prod_len = 1;
    prod[0] = 1;

    for (int i = 0; i < nd; ++i) {
        mult_small(prod, &prod_len, fact[digit_buf[i]]);
    }

    uint32_t sum = 0;
    for (int i = 0; i < prod_len; ++i) sum += prod[i];
    return sum;
}

/* Cache for previously computed terms of the sequence. */
#define MAX_CACHE 20000
static seq_t cache[MAX_CACHE];
static size_t cache_len = 0;
static idx_t next_candidate = 1;

/* Main function required by the harness */
seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined for n < 1, but harmless */

    /* Generate terms until we have at least n of them */
    while (cache_len < n) {
        idx_t k = next_candidate;
        next_candidate++;

        /* Sum of digits of k */
        uint32_t sum_k = 0;
        idx_t t = k;
        if (t == 0) sum_k = 0;
        while (t) {
            sum_k += t % 10;
            t /= 10;
        }

        /* Sum of digits of product of factorials of digits of k */
        uint32_t sum_f = product_digit_sum(k);

        if (sum_k == sum_f) {
            cache[cache_len] = (seq_t)k;
            ++cache_len;
        }
    }

    return cache[n - 1];
}
