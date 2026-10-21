/*
 * A074843  Quadruplets: base 10 representation is the juxtaposition of four
 *          identical strings.
 * https://oeis.org/A074843
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   a(n) = Concatenation of the decimal representation of n with
 *            itself 4 times
 * Range:     u8 none, u16 n <= 9, u32 n <= 99, u64 n <= 1000, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*   For an n with d decimal digits, a(n) = n * (10^(3d)+10^(2d)+10^d+1)   */
#include <stdint.h>

/* Count the number of decimal digits of a positive idx_t integer. */
static unsigned int digits(idx_t n)
{
    unsigned int d = 0;
    do {
        ++d;
        n /= 10;
    } while (n != 0);
    return d;
}

/* Compute a(n) for the OEIS sequence A074843. */
seq_t a(idx_t n)
{
    seq_t sn = (seq_t)n;           /* cast idx_t to seq_t for arithmetic */
    unsigned int d = digits(n);    /* number of decimal digits of n */

    /* Compute 10^d as a seq_t. */
    seq_t base = 1;
    for (unsigned int i = 0; i < d; ++i)
        base *= (seq_t)10;

    /* Concatenate n four times: result = n, then *base + n three times. */
    seq_t res = sn;
    for (int i = 0; i < 3; ++i)
        res = res * base + sn;

    return res;
}
