/*
 * A104617  Write the natural numbers in base 14 in a triangle with k digits
 *          in the k-th row, as shown below. Sequence gives the leading
 *          diagonal.
 * https://oeis.org/A104617
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = the (n(n+1)/2)-th base‑14 digit in the * concatenation of
 *            1,2,3,…. That digit equals the * nth element of the diagonal of
 *            the triangle
 * Range:     u8 n <= 15, u16 n <= 99, u32 n <= 99, u64 n <= 99, u128 n <= 99,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n) {
    /* position of the diagonal digit (1‑based) */
    seq_t pos = ((seq_t)n * ((seq_t)n + 1)) / 2;

    /* iterate over groups of numbers with the same base‑14 digit
       length.  For group d we have
          first number = 14^(d-1)
          count = 13 * 14^(d-1)
          total digits = count * d            */
    idx_t d = 1;                 /* number of digits in group */
    seq_t pow14 = 1;             /* 14^(d-1) */
    seq_t cumulative = 0;        /* digits used by previous groups */

    seq_t count_d, digits_d;

    while (true) {
        count_d  = (seq_t)13 * pow14;
        digits_d = count_d * (seq_t)d;
        if (cumulative + digits_d >= pos)
            break;
        cumulative += digits_d;
        ++d;
        pow14 *= 14;
    }

    /* position inside the current group (0‑based) */
    seq_t offset = pos - cumulative - 1;
    seq_t index_in_group = offset / (seq_t)d;         /* which number of the group */
    idx_t digit_index = (idx_t)(offset % (seq_t)d);   /* 0‑based from left */
    seq_t first = pow14;  /* first number of the group */
    seq_t number = first + index_in_group;

    /* power to isolate the desired digit: 14^(d-1-digit_index) */
    seq_t power_sub = pow14;
    for (idx_t i = 0; i < digit_index; ++i)
        power_sub /= 14;

    /* extract digit */
    seq_t digit = (number / power_sub) % 14;
    return digit;
}
