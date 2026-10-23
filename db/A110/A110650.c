/*
 * A110650  n^2 followed by n^4 followed by n followed by n^3.
 * https://oeis.org/A110650
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   For n ≥ 1 let k = ⌊(n-1)/4⌋, b = k+1. * a(4k+1) = b² * a(4k+2) =
 *            b⁴ * a(4k+3) = b * a(4k+4) = b³ This reproduces A110650
 * Range:     u8 n <= 13, u16 n <= 61, u32 n <= 1021, u64 n <= 8000,
 *            u128 n <= 8000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute base b = ceil(n/4). */
    idx_t k = (n - 1) / 4;          /* k = floor((n-1)/4) */
    seq_t b = (seq_t)(k + 1);       /* convert to seq_t type */

    /* Determine exponent according to n mod 4. */
    int exp;
    switch (n % 4) {
        case 1: exp = 2; break; /* a(4k+1) = b^2 */
        case 2: exp = 4; break; /* a(4k+2) = b^4 */
        case 3: exp = 1; break; /* a(4k+3) = b   */
        default: exp = 3;        /* n % 4 == 0 -> a(4k+4) = b^3 */
    }

    /* Compute b^exp (exp ≤ 4, so simple loop is fine). */
    seq_t result = 1;
    for (int i = 0; i < exp; ++i) {
        result *= b;
    }
    return result;
}
