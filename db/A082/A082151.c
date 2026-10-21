/*
 * A082151  A transform of C(n,2).
 * https://oeis.org/A082151
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = C(n,2)*(3^(n-2)+5^(n-2))/2, with C(n,2)=n*(n-1)/2.
 *            a(0)=a(1)=0. All arithmetic is done with the build's seq_t type
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 13, u64 n <= 26, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Fast exponentiation – works for any seq_t that supports +,-,*,/ operators. */
static seq_t pow_seq(unsigned base, idx_t exp)
{
    seq_t result = (seq_t)1;
    seq_t b = (seq_t)base;
    while (exp > 0) {
        if (exp & 1U)
            result *= b;
        b *= b;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    if (n < 2U)
        return 0;                     /* a(0)=a(1)=0 */

    /* C(n,2) = n*(n-1)/2  computed safely in seq_t */
    seq_t bin = ((seq_t)n * ((seq_t)n - 1)) / 2;

    /* 3^(n-2) and 5^(n-2) */
    seq_t p3 = pow_seq(3, n - 2U);
    seq_t p5 = pow_seq(5, n - 2U);

    seq_t sum = p3 + p5;

    seq_t prod;
    /* If seq_t fits in 64 bits, use a 128‑bit intermediate to avoid overflow
       before the final division.  For larger seq_t (e.g. arbitrary‑precision),
       the plain multiplication is used. */
    if (sizeof(seq_t) <= sizeof(unsigned long long)) {
        unsigned __int128 tmp = (unsigned __int128)bin * (unsigned __int128)sum;
        prod = (seq_t)(tmp / 2);
    } else {
        prod = (bin * sum) / 2;
    }
    return prod;
}
