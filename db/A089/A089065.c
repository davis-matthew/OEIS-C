/*
 * A089065  Decimal expansion of 2^13466917 - 1, the 39th Mersenne prime
 *          A000668(39).
 * https://oeis.org/A089065
 *
 * Offset:    4053946    (a(4053946) is the first term)
 * Keywords:  fini, cons, nonn
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 none, u64 none, u128 none, big any n
 * Helpers:   pow2, seq_to_str
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: decimal expansion of 2^13466917 − 1 (the 39th Mersenne prime) */
#include <stdlib.h>
#include <stdint.h>
#include "../utils/pow2.h"
#include "../utils/seq_to_str.h"

static const idx_t OFFSET = 4053946;          /* first index of the sequence   */
static const idx_t EXP   = 13466917;           /* exponent for the Mersenne prime  */
static seq_t *digits = NULL;                 /* cache of decimal digits       */
static size_t digits_len = 0;                /* number of cached digits       */

/* Initialize the digits cache once.  The number 2^EXP is converted to a decimal
   string, and the final digit '2' is replaced with '1' to obtain 2^EXP − 1. */
static void init_digits(void)
{
    if (digits != NULL)
        return;

    /* Compute 2^EXP as a big integer. */
    seq_t val = pow2(EXP);

    /* Convert the number to a decimal string. */
    /* The exact number of digits is known: 4053946. */
    const size_t MAX_DIGITS = 4053946;
    char *str = (char *)malloc(MAX_DIGITS + 1);      /* +1 for safety or null */
    size_t bufsize = MAX_DIGITS + 1;
    seq_to_str(val, str, &bufsize);

    /* Replace the last digit (currently '2') with '1' for 2^EXP − 1. */
    if (bufsize > 0)
        str[bufsize - 1] = '1';

    /* Store digits in an array for quick access. */
    digits = (seq_t *)malloc(MAX_DIGITS * sizeof(seq_t));
    digits_len = MAX_DIGITS;
    for (size_t i = 0; i < MAX_DIGITS; ++i)
        digits[i] = (seq_t)(str[i] - '0');

    free(str);
}

/* Return the n-th term of the sequence. */
seq_t a(idx_t n)
{
    if (digits == NULL)
        init_digits();

    if (n < OFFSET || (size_t)(n - OFFSET) >= digits_len)
        return 0;          /* behavior below the offset is irrelevant */

    return digits[n - OFFSET];
}
