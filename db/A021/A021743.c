/*
 * A021743  Decimal expansion of 1/739.
 * https://oeis.org/A021743
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = floor(10^(n+1) / 739) mod 10
 * Range:     u8 n <= 1, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static seq_t *digits = NULL;     /* stored digits 0..stored_len-1             */
static idx_t stored_len = 0;     /* number of digits currently stored        */
static idx_t remainder = 1;      /* remainder before computing next digit    */
static size_t capacity = 0;      /* allocated size of digits array           */

static void ensure_capacity(idx_t needed)
{
    if ((size_t)needed < capacity)
        return;
    size_t new_cap = capacity ? capacity * 2 : 128;
    while ((size_t)needed >= new_cap)
        new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(digits, new_cap * sizeof(seq_t));
    digits = tmp;
    capacity = new_cap;
}

seq_t a(idx_t n)
{
    if (n < stored_len)
        return digits[n];

    ensure_capacity(n + 1);
    for (idx_t i = stored_len; i <= n; ++i)
    {
        seq_t digit = (seq_t)(remainder * 10) / 739;
        remainder = (remainder * 10) % 739;
        digits[i] = digit;
    }
    stored_len = n + 1;
    return digits[n];
}
