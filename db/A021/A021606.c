/*
 * A021606  Decimal expansion of 1/602.
 * https://oeis.org/A021606
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons
 * Formula:   a(n) = the n‑th decimal digit of 1/602 (i.e. the digit a(n) in
 *            the expansion 0.0016611295… )
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* static cache for computed digits */
static uint8_t *digits = NULL;     /* array of digits 0‑9 */
static size_t digits_cap = 0;      /* current capacity of the array */
static idx_t digits_len = 0;       /* number of already computed digits */
static uint64_t rem = 1;           /* remainder for the next digit calculation */

/* Ensure the cache holds digits up to index n (inclusive) */
static void compute_to(idx_t n)
{
    if (n < digits_len) return;          /* already computed */

    if (digits == NULL) {
        digits_cap = 64;                /* initial capacity */
        digits = (uint8_t *)malloc(digits_cap * sizeof(uint8_t));
    }

    while (digits_len <= n) {
        if (digits_len >= digits_cap) {  /* grow array if needed */
            digits_cap = digits_cap * 2;
            digits = (uint8_t *)realloc(digits, digits_cap * sizeof(uint8_t));
        }
        uint64_t tmp = rem * 10ULL;               /* 0 ≤ rem < 602, safe */
        uint8_t d = (uint8_t)(tmp / 602ULL);      /* next decimal digit */
        rem = tmp % 602ULL;                       /* update remainder  */
        digits[digits_len++] = d;
    }
}

/* Sequence function */
seq_t a(idx_t n)
{
    compute_to(n);
    return (seq_t)digits[n];
}
