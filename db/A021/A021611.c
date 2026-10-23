/*
 * A021611  Decimal expansion of 1/607.
 * https://oeis.org/A021611
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   not stated
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the n-th term of the decimal expansion of 1/607.
   a(0) is the integer part (0),
   a(1) is the first decimal digit, a(2) is the second, etc.        */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/*  Keep a cache of the decimal digits already generated.
    digits[i] holds the (i+1)-th decimal digit (for i>=0).     */
static seq_t *digits = NULL;          /* array of digits          */
static size_t  dlen   = 0;            /* number of digits stored  */
static size_t  dcap   = 0;            /* current capacity         */
static uint64_t rem    = 1;           /* remainder of the division */

seq_t a(idx_t n)
{
    /* a(0) is always the integer part, which is 0. */
    if (n == 0) return 0;

    /* Initialise the digit buffer on the first call. */
    if (digits == NULL) {
        dcap = 16;                            /* initial capacity   */
        digits = (seq_t *)malloc(dcap * sizeof(seq_t));
        if (!digits) abort();
    }

    /* Generate decimal digits up to the required index.
       digits[0] is the first decimal digit, hence we need
       digits[n] for n >= 2.  The loop condition is
       dlen <= n so that we position the first missing
       digit for any later request.                      */
    while (dlen <= n) {
        if (dlen >= dcap) {                  /* grow the array    */
            size_t new_cap = dcap * 2;
            seq_t *tmp = (seq_t *)realloc(digits, new_cap * sizeof(seq_t));
            if (!tmp) abort();
            digits = tmp;
            dcap = new_cap;
        }

        /* Long‑division step: multiply remainder by 10,
           take quotient as the next digit, remainder as
           the new remainder.                     */
        rem *= 10;
        seq_t digit = (seq_t)(rem / 607);
        digits[dlen] = digit;
        rem %= 607;
        dlen++;
    }

    /* Mapping from the required index to the stored digits:
       n == 1  → digits[0]  (first decimal digit),
       n >= 2  → digits[n]  (because digits[0] is d₁, digits[1] is d₂, etc.). */
    if (n == 1) return digits[0];
    return digits[n];
}
