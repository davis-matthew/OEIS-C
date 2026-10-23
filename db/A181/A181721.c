/*
 * A181721  The smallest number k such that the product k*n contains
 *          digit-reverse(n) as a substring.
 * https://oeis.org/A181721
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the smallest k such that k * n contains the digit‑reverse
 *            of n as a substring. The digit‑reverse keeps trailing zeros,
 *            e.g. rev(10) = "01"
 * Range:     u8 n <= 9, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   digits, seq_to_str
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include "../utils/digits.h"
#include "../utils/seq_to_str.h"

seq_t a(idx_t n) {
    /* n must be ≥ 1; for safety return 0 if 0 is passed */
    if (n == 0) return 0;

    /* Compute rev(n) as a string of length digits(n), padding leading zeros. */
    idx_t tmp = n;
    unsigned long long rev_small = 0;
    while (tmp > 0) {
        rev_small = rev_small * 10 + (tmp % 10);
        tmp /= 10;
    }
    int D = digits(n);                         /* length of n in decimal  */
    char rev_buf[32];
    size_t rev_len;
    seq_t rev_int = (seq_t)rev_small;
    seq_to_str(rev_int, rev_buf, &rev_len);     /* rev_buf contains rev_small  */

    /* Pad leading zeros to reach length D. */
    char rev_full[32];
    int pad = D - (int)rev_len;
    for (int i = 0; i < pad; ++i) rev_full[i] = '0';
    for (size_t i = 0; i < rev_len; ++i) rev_full[pad + i] = rev_buf[i];
    rev_full[pad + rev_len] = '\0';

    /* Search for the smallest k. */
    seq_t k = 1;
    char prod_buf[128];            /* 128 digits are enough for any 128‑bit value */
    while (1) {
        seq_t product = (seq_t)n * k;
        size_t prod_len;
        seq_to_str(product, prod_buf, &prod_len);
        prod_buf[prod_len] = '\0';
        if (strstr(prod_buf, rev_full) != NULL)
            return k;
        ++k;
    }
}
