/*
 * A031312  Successive digits of odd numbers.
 * https://oeis.org/A031312
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 105, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Function for OEIS A031312 – successive digits of odd numbers.
 *
 *  a(n) is the n‑th digit of the infinite decimal string
 *  obtained by concatenating the positive odd integers:
 *      1 3 5 7 9 11 13 15 17 19 21 23 25 …
 *
 *  The routine works for any idx_t ≥ 1 and any seq_t that supports
 *  the usual unsigned integer operations.  No global state is used.
 */

#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    /* Zero‑based position in the digit stream. */
    seq_t pos = (seq_t)n - (seq_t)1;

    /* Current number of digits (d) of the odd numbers in this block.
       pow10 = 10^(d-1).  Initially d = 1, pow10 = 1. */
    idx_t d = 1;
    seq_t pow10 = 1;

    while (1)
    {
        /* Count of odd numbers having d digits. */
        seq_t count_odds;
        if (d == 1)
        {
            count_odds = (seq_t)5;
        }
        else
        {
            /* For d ≥ 2: 45 × 10^(d‑2) = 45 × (pow10 / 10). */
            count_odds = (seq_t)45 * (pow10 / 10);
        }

        /* Total digits contributed by this block. */
        seq_t digits_k = count_odds * d;

        if (pos < digits_k)
        {
            break; /* the desired digit lies in this block */
        }

        /* Skip this whole block */
        pos   -= digits_k;
        d     += 1;
        pow10 *= (seq_t)10;
    }

    /* Index of the odd number within this block (zero‑based) and
       which digit (from the left, zero‑based) inside that number. */
    seq_t offset_in_block = pos / d;
    seq_t digit_in_num    = pos % d;

    /* First odd number in the current block. */
    seq_t first_odd = (d == 1) ? (seq_t)1 : pow10 + (seq_t)1;

    /* The odd number that contains the target digit. */
    seq_t odd_num = first_odd + offset_in_block * (seq_t)2;

    /* The power of ten that places the required digit at units place. */
    seq_t pow = pow10;
    for (idx_t i = 0; i < digit_in_num; ++i)
    {
        pow /= (seq_t)10;
    }

    /* Extract the desired digit. */
    seq_t digit = (odd_num / pow) % (seq_t)10;
    return digit;
}
