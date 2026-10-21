/*
 * A084024  Triangle read by rows in which row n gives n smallest n-digit
 *          multiples of n that are palindromes with rows padded with 0 if
 *          there are insufficient palindromes.
 * https://oeis.org/A084024
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 15, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   pow10, reverse_decimal
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS A084024:
   Read the triangle by rows.  Row r contains the smallest r n‑digit
   palindromes that are multiples of r, padded with zeroes if fewer
   exist.  a(n) is the n‑th element of this triangle (offset 1). */

#include "../utils/pow10.h"
#include "../utils/reverse_decimal.h"
#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* safe guard: offset is 1   */

    /* --- find the row r such that r(r+1)/2 >= n --- */
    idx_t low  = 1;
    idx_t high = n;                       /* always high>=low         */
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        __uint128_t mid_sum = (__uint128_t)mid * (mid + 1) / 2;
        if (mid_sum >= n)
            high = mid;
        else
            low  = mid + 1;
    }
    idx_t r = low;

    /* position within the row (1‑based) ---------------------------- */
    __uint128_t prev_sum = (__uint128_t)r * (r - 1) / 2;
    idx_t pos = (idx_t)((__uint128_t)n - prev_sum);   /* pos in {1..r} */

    /* --- search the pos‑th palindrome in row r ------------------- */
    idx_t half_len   = (r + 1) / 2;                /* digits in the prefix  */
    seq_t prefix_min = pow10(half_len - 1);        /* first possible prefix */
    seq_t prefix_max = pow10(half_len) - 1;        /* last possible prefix  */

    seq_t count    = 0;
    seq_t r_seq    = (seq_t)r;                    /* for modulo               */
    seq_t prefix   = prefix_min;

    while (prefix <= prefix_max) {
        /* construct the palindrome from the prefix  */
        seq_t pal;
        if (r % 2 == 0) { /* even length */
            pal = prefix * pow10(half_len) + reverse_decimal(prefix);
        } else { /* odd length: drop centre digit before reversal */
            pal = prefix * pow10(half_len - 1) + reverse_decimal(prefix / 10);
        }

        if (pal % r_seq == 0) { /* divisible by r  */
            ++count;
            if (count == pos)
                return pal;
        }
        prefix = prefix + 1;
    }

    /* If fewer than pos palindromes exist, return zero (padding). */
    return 0;
}
