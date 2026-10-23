/*
 * A174991  A005606 applied to itself.
 * https://oeis.org/A174991
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, word
 * Formula:   a(n) = A005606(A005606(n))
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A174991: apply the alphabet position of the first letter
 * of the English word for n to the same function again.  Only a tiny
 * lookup table is required, no heavy preprocessing. */

#include <stddef.h>

/* Map for numbers 0..19 (index 0 unused) */
static const seq_t _first_20[20] = {
    0, 15, 20, 20, 6, 6, 19, 19, 5, 14,
    20, 5, 20, 20, 6, 6, 19, 19, 5, 14
};

/* Map for tens 0..9 (index 0,1 unused) */
static const seq_t _tens[10] = {
    0, 0, 20, 20, 6, 6, 19, 19, 5, 14
};

/* Map for digits 0..9 (index 0 unused) */
static const seq_t _digit[10] = {
    0, 15, 20, 20, 6, 6, 19, 19, 5, 14
};

/* Return the alphabet position of the first letter of n's English word.
 * For n==0 it is 'Z' -> 20.  For n>0 we look only at the most significant
 * group of up to 3 digits, following standard English naming. */
static seq_t _first_letter_pos(idx_t n)
{
    if (n == 0)
        return 20;  /* "zero" -> Z */

    /* Reduce to the most significant block of 3 digits. */
    idx_t tmp = n;
    while (tmp >= 1000)
        tmp /= 1000;         /* now tmp in [1..999] */

    if (tmp < 20)
        return _first_20[(size_t)tmp];

    if (tmp < 100)          /* 20..99 */
        return _tens[(size_t)(tmp / 10)];

    /* 100..999 : take the hundreds digit */
    return _digit[(size_t)(tmp / 100)];
}

seq_t a(idx_t n)
{
    seq_t first = _first_letter_pos(n);
    return _first_letter_pos((idx_t)first);
}
