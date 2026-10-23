/*
 * A019553  a(n) is the concatenation of n and 5n.
 * https://oeis.org/A019553
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy, less
 * Formula:   a(n) = concatenation of n and 5n
 * Range:     u8 n <= 2, u16 n <= 65, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

/* Helper: number of decimal digits of a seq_t number */
static int digit_len_seq_t(seq_t x)
{
    int len = 0;
    do {
        len++;
        x /= 10;
    } while (x != 0);
    return len;
}

seq_t a(idx_t n)
{
    /* a(1)... = 15, 210, 315, ... as per OEIS */
    if (n == 0)     /* undefined for offset 1 but safe guard */
        return 0;

    seq_t five_n = (seq_t)n * 5;            /* 5*n */
    int d       = digit_len_seq_t(five_n); /* number of digits of 5*n */

    seq_t pow10d = 1;
    for (int i = 0; i < d; ++i)
        pow10d *= 10;                       /* 10^d */

    return (seq_t)n * pow10d + five_n;     /* concatenation of n and 5*n */
}
