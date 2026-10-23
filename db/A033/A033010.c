/*
 * A033010  Numbers each of whose runs of digits in base 12 has length 2.
 * https://oeis.org/A033010
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 132, u32 n <= 1400, u64 n <= 1400,
 *            u128 n <= 1400, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A033010
 *  Numbers each of whose runs of digits in base 12 has length 2.
 *
 *  Formula: a(n) = 13 * A043316(n)   (A043316 = numbers whose base‑12
 *              digits come in equal pairs).  The first 11 terms are
 *              a(n) = 13 * n.
 *
 *  This implementation generates the n‑th term directly by interpreting
 *  the index in a mixed alphabet of size 12 with no repeated adjacent
 *  symbols, then converting to a base‑144 number and multiplying by 13.
 *  It is O(L) where L is the number of base‑144 digits (≤ 10 for all
 *  relevant n) so it easily satisfies the time requirement for n up
 *  to 10 000.
 */

#include <stdint.h>   /* for uint64_t, etc.  seq_t/idx_t are defined elsewhere */

seq_t a(idx_t n)
{
    /* Find the length L of the base‑144 representation (number of pairs). */
    idx_t pow11 = 11;        /* 11¹   */
    idx_t L     = 1;         /* current length   */
    idx_t cum   = 11;        /* cumulative count up to length L */

    while (cum < n) {
        pow11 *= 11;  /* next 11^L */
        L++;
        cum += pow11;
    }
    /* count of numbers with fewer digits */
    idx_t prevCum = cum - pow11;
    /* zero‑based offset among length‑L numbers */
    idx_t offset = n - prevCum - 1;

    /* Pre‑compute powers of 11 up to L for fast base‑11 conversion. */
    idx_t pow[64];
    pow[0] = 1;
    for (idx_t i = 1; i <= L; ++i) {
        pow[i] = pow[i - 1] * 11;
    }

    /* Convert the offset into the sequence of base‑12 pair digits.
     *   pos = 1   → leading digit (1…11)
     *   pos > 1   → digit that is not equal to the previous one
     */
    idx_t base_val = 0;   /* holds the value in base‑144 */
    idx_t prev_digit = 0;
    idx_t rem = offset;

    for (idx_t pos = 1; pos <= L; ++pos) {
        idx_t power = pow[L - pos];         /* 11^(L‑pos)  */
        idx_t idx   = rem / power;          /* digit index  */
        rem         %= power;

        idx_t digit;
        if (pos == 1) {                     /* leading digit 1‑11            */
            digit = idx + 1;
        } else {                            /* skip the previous digit      */
            if (idx < prev_digit)
                digit = idx;
            else
                digit = idx + 1;
        }
        prev_digit = digit;

        base_val = base_val * 144 + digit;   /* build base‑144 number */
    }

    /* Convert to the final value: multiply by 13. */
    seq_t result = (seq_t)base_val * (seq_t)13;
    return result;
}
