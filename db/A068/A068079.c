/*
 * A068079  Decimal expansion of 355 / 113.
 * https://oeis.org/A068079
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, cons
 * Formula:   a(n) = the n‑th decimal digit of 355 ÷ 113 (1‑based)
 * Range:     u8 n <= 119, u16 n <= 119, u32 n <= 119, u64 n <= 119,
 *            u128 n <= 119, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence is obtained by long division of 355 by 113. */

#include <stdlib.h>
#include <stddef.h>

static seq_t *digits = NULL;     /* 0‑based array of cached digits, digits[0] = a(1) */
static idx_t  digits_len = 0;    /* number of digits currently cached */
static idx_t  digits_cap = 0;    /* allocated capacity of the array */
static idx_t  rem       = 0;     /* current remainder after the last cached digit */

/* Initialise cache with the integer part of the fraction. */
static void init_cache(void)
{
    if (digits == NULL) {
        digits_cap = 1;
        digits = (seq_t *)malloc((size_t)digits_cap * sizeof(seq_t));
        if (digits == NULL) exit(EXIT_FAILURE);
    }
    digits[0] = (seq_t)3;          /* integer part of 355 / 113 */
    digits_len = 1;
    rem = 355 % 113;               /* remainder after the integer part */
}

/* Ensure the array can hold at least `need` digits.  Exponentially grow capacity. */
static void ensure_cache_capacity(idx_t need)
{
    if (need <= digits_cap) return;

    idx_t new_cap = digits_cap ? digits_cap : 1;
    while (new_cap < need) new_cap <<= 1;

    seq_t *ptr = (seq_t *)realloc(digits, (size_t)new_cap * sizeof(seq_t));
    if (ptr == NULL) exit(EXIT_FAILURE);
    digits = ptr;
    digits_cap = new_cap;
}

/* Generate digits up to position `n` (1‑based).  */
static void ensure_digits(idx_t n)
{
    if (n <= digits_len) return;      /* already computed */

    if (digits_len == 0) init_cache(); /* first call */

    ensure_cache_capacity(n);

    for (idx_t i = digits_len; i < n; ++i) {
        idx_t d = (rem * 10) / 113;    /* next decimal digit */
        rem   = (rem * 10) % 113;     /* update remainder */
        digits[i] = (seq_t)d;
    }
    digits_len = n;
}

/* External sequence entry point */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;    /* undefined for n < 1; return 0 */
    ensure_digits(n);
    return digits[n - 1];
}
