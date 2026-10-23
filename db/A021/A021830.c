/*
 * A021830  Decimal expansion of 1/826.
 * https://oeis.org/A021830
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons
 * Formula:   a(n) = floor((10^(n+1) / 826)) mod 10
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    static seq_t *digits = NULL;
    static size_t cap = 0;
    static size_t len = 0;
    static unsigned long long r_next = 1; /* remainder after the last computed digit */

    while (len <= n) {
        unsigned long long prod = r_next * 10ULL;
        seq_t digit = (seq_t)(prod / 826ULL);
        r_next = prod % 826ULL;

        if (len == cap) {
            size_t new_cap = cap == 0 ? 16 : cap * 2;
            seq_t *tmp = (seq_t *)realloc(digits, new_cap * sizeof(seq_t));
            if (!tmp) {
                abort(); /* or exit(EXIT_FAILURE); */
            }
            digits = tmp;
            cap = new_cap;
        }
        digits[len++] = digit;
    }

    return digits[n];
}
