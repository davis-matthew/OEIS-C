/*
 * A021469  Decimal expansion of 1/465.
 * https://oeis.org/A021469
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons
 * Formula:   a(n) = nth decimal digit of 1/465 (a(0) is the first digit after
 *            the decimal point)
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
    /* Buffer of previously computed digits.
       The buffer grows as needed. */
    static unsigned char *digits = NULL;
    static size_t cap = 0;          /* current capacity of the buffer */
    static size_t sz  = 0;          /* number of digits actually stored */
    static uint64_t rem = 1;        /* current remainder in the long division */

    /* Ensure we have digits up to index n (inclusive). */
    if (sz <= (size_t)n) {
        size_t needed = (size_t)n + 1;
        if (cap < needed) {
            size_t new_cap = cap ? (cap << 1) : 1;
            if (new_cap < needed) new_cap = needed;
            unsigned char *tmp = (unsigned char *)realloc(digits, new_cap);
            if (!tmp) abort();                 /* memory allocation failed */
            digits = tmp;
            cap    = new_cap;
        }

        for (size_t i = sz; i <= (size_t)n; ++i) {
            uint64_t q = (rem * 10ULL) / 465ULL;  /* digit 0..9 */
            rem      = (rem * 10ULL) % 465ULL;    /* new remainder */
            digits[i] = (unsigned char)q;
        }
        sz = needed;
    }

    return (seq_t)digits[(size_t)n];
}
