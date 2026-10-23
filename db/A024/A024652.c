/*
 * A024652  n written in fractional base 9/4.
 * https://oeis.org/A024652
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 26, u16 n <= 242, u32 n <= 8342, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Representation of n in fractional base 9/4, digits 0‑8 concatenated as decimal integer */

seq_t a(idx_t n)
{
    /* If n is 0, the representation is "0" */
    if (n == 0)
        return (seq_t)0;

    /* Store the digits in least‑significant‑first order */
    unsigned digits[64];
    int count = 0;

    while (n > 0) {
        unsigned d = n % 9;          /* least significant digit (0..8) */
        digits[count++] = d;

        /* Compute the next n: n1 = (4*n - 4*d) / 9  (exact division) */
        unsigned __int128 tmp = (unsigned __int128)4 * n
                               - (unsigned __int128)4 * d;
        n = (idx_t)(tmp / 9);
    }

    /* Convert the digits to a decimal integer, most significant first */
    seq_t result = 0;
    for (int i = count - 1; i >= 0; --i)
        result = result * 10 + digits[i];

    return result;
}
