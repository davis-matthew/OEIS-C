/*
 * A099892  XOR BINOMIAL transform of A003188 (Gray code numbers); also the
 *          main diagonal of the XOR difference triangle A099891.
 * https://oeis.org/A099892
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   not stated
 * Range:     u8 n <= 255, u16 n <= 8192, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A099892  -  XOR BINOMIAL transform of A003188 (Gray code numbers)
    The sequence is zero for all n except powers of two.
    a(0) = 0
    a(1) = 1
    for e > 0,   a(2^e) = 3 * 2^(e-1)  (i.e. 3 * (n/2) when n is a power of two)
    For all other n, a(n) = 0.
    This follows from the XOR-binomial transform definition and the fact that
    a(n) is nonzero only when n is a power of two.                            */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)          /* a(0) = 0 */
        return (seq_t)0;

    /* Check if n is a power of two: n & (n-1) == 0 */
    if ((n & (n - 1)) != 0)
        return (seq_t)0; /* zero for non‑powers of two */

    if (n == 1)          /* a(1) = 1 */
        return (seq_t)1;

    /* For n = 2^e, e > 0: a(n) = 3 * 2^(e-1) = 3 * (n/2) */
    return (seq_t)3 * ((seq_t)n >> 1);
}
