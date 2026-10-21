/*
 * A200790  Number of 0..n arrays x(0..7) of 8 elements without any two
 *          consecutive increases.
 * https://oeis.org/A200790
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (6679*n^8 + 76784*n^7 + 370286*n^6 + 968576*n^5 * +
 *            1485071*n^4 + 1348256*n^3 + 698284*n^2 + * 186864*n + 20160) /
 *            20160
 * Range:     u8 none, u16 n <= 3, u32 n <= 16, u64 n <= 136, u128 n <= 136,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Integer coefficients of the numerator polynomial above */
    static const unsigned long long coeff[9] = {
        20160ULL,   /* n^0 */
        186864ULL,  /* n^1 */
        698284ULL,  /* n^2 */
        1348256ULL, /* n^3 */
        1485071ULL, /* n^4 */
        968576ULL,  /* n^5 */
        370286ULL,  /* n^6 */
        76784ULL,   /* n^7 */
        6679ULL     /* n^8 */
    };

    /* The denominator (constant term of the division) */
    static const unsigned long long denom = 20160ULL;

#if defined(__SIZEOF_INT128__)
    /* 128‑bit arithmetic is available; use it for all integer */ 
    unsigned __int128 val = coeff[8];              /* start with n^8 coefficient */
    for (int k = 7; k >= 0; --k) {
        val = val * (unsigned __int128)n + (unsigned __int128)coeff[k];
    }
    return (seq_t)(val / (unsigned __int128)denom);
#else
    /* Fallback: use the sequence type itself (works for arbitrary‑precision) */
    seq_t val = coeff[8];
    for (int k = 7; k >= 0; --k) {
        val = val * (seq_t)n + (seq_t)coeff[k];
    }
    return val / (seq_t)denom;
#endif
}
