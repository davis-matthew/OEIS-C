/*
 * A037818  Number of i such that d(i) > d(i-1), where Sum_{i=0..m} d(i)*3^i
 *          is the base-3 representation of n.
 * https://oeis.org/A037818
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = number of indices i (i >= 1) such that the i‑th base‑3
 *            digit of n * is strictly greater than the preceding digit. Here
 *            d(0) is the least * significant base‑3 digit of n
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    seq_t count = (seq_t)0;      /* result to return */
    unsigned prev = 0;           /* previous digit, valid only when 'has_prev' is true */
    bool has_prev = false;       /* true after first digit is processed */

    /* Process digits of n in base 3, least significant first */
    while (n > 0) {
        unsigned digit = (unsigned)(n % 3);
        if (has_prev && digit > prev) {
            count += (seq_t)1;
        }
        prev = digit;
        has_prev = true;
        n /= 3;                 /* integer division by 3 */
    }

    return count;
}
