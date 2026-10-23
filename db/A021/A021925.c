/*
 * A021925  Decimal expansion of 1/921.
 * https://oeis.org/A021925
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons
 * Formula:   a(n) = floor(10^(n+1)/921) - 10*floor(10^n/921) (digits of
 *            1/921)
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

static seq_t *digits = 0;      /* cached digits 0 … (digits_len-1) */
static size_t digits_len = 0;  /* number of digits currently stored */
static size_t digits_cap = 0;  /* capacity of the digits array */
static uint64_t remainder = 1;/* remainder before multiplying by 10 for the next digit */

/* Return the n-th digit (0‑based) of the decimal expansion of 1/921. */
seq_t a(idx_t n)
{
    /* If we already have the digit, return it. */
    if ((size_t)n < digits_len)
        return digits[n];

    /* Ensure there is capacity for up to digit n. */
    size_t target_len = (size_t)n + 1;
    if (target_len > digits_cap) {
        size_t new_cap = digits_cap ? digits_cap : 1;
        while (new_cap < target_len)
            new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(digits, new_cap * sizeof(seq_t));
        digits = tmp;
        digits_cap = new_cap;
    }

    /* Compute digits from the current length up to n. */
    for (idx_t i = (idx_t)digits_len; i <= (idx_t)n; ++i) {
        remainder *= 10ULL;                 /* bring next decimal digit */
        seq_t digit = (seq_t)(remainder / 921ULL);
        remainder %= 921ULL;                /* new remainder for next step */
        digits[i] = digit;
    }

    digits_len = target_len;  /* update length */
    return digits[n];
}
