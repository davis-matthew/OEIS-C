/*
 * A044529  Numbers n such that string 1,3 occurs in the base 7 representation
 *          of n but not of n+1.
 * https://oeis.org/A044529
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 42, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n): the n‑th integer n such that the base‑7
 * representation of n contains the substring “13” while the
 * representation of n+1 does not. */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>

/* Returns true iff x has the consecutive digits 1,3 in base‑7. */
static bool has13(seq_t x)
{
    if (x == 0)
        return false;

    /* 128 digits are more than enough for the values that will
       be produced with the required limits (even 128‑bit integers). */
    uint8_t digits[128];
    size_t len = 0;

    /* Extract base‑7 digits (least significant first). */
    while (x > 0)
    {
        digits[len++] = (uint8_t)(x % 7);
        x = x / 7;
    }

    /* Scan from most significant to least significant digit. */
    for (size_t i = len; i-- > 1; )
    {
        if (digits[i] == 1 && digits[i - 1] == 3)
            return true;
    }
    return false;
}

/* Return the n‑th term of OEIS A044529; n ≥ 1. */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;

    /* The maximum value that can be represented by seq_t. */
    const seq_t max_seq_t = (seq_t)-1;
    const idx_t  max_idx   = (idx_t)max_seq_t;

    idx_t  found   = 0;
    idx_t  cur     = 1;

    while (cur < max_idx)
    {
        seq_t val = (seq_t)cur;
        /* We must be sure that val+1 is still representable. */
        if (has13(val) && !has13(val + (seq_t)1))
        {
            ++found;
            if (found == n)
                return val;
        }
        ++cur;
    }

    /* If we get here the requested n is out of range for the
       given seq_t type.  Return 0 to indicate an error. */
    return (seq_t)0;
}
