/*
 * A073785  Numbers in base -3.
 * https://oeis.org/A073785
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = decimal integer obtained from the base –3 *
 *            representation of n with digits 0,1,2. That representation * is
 *            unique for every non‑negative integer n
 * Range:     u8 n <= 20, u16 n <= 182, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    /* Use a signed 128‑bit integer for the division algorithm
     * so that we can handle any unsigned idx_t safely. */
    __int128 N = (__int128)n;

    /* Store the base–3 digits (least significant first). */
    int digits[128];
    int pos = 0;

    while (N != 0) {
        /* Divide by –3 using truncating division. */
        __int128 q = N / -3;
        __int128 r = N - q * (-3);
        if (r < 0) {
            r += 3;
            q += 1;
        }
        digits[pos++] = (int)r;
        N = q;
    }

    /* Build the decimal integer from the digits (most significant first). */
    seq_t result = 0;
    for (int i = pos - 1; i >= 0; --i) {
        result = result * 10 + (seq_t)digits[i];
    }
    return result;
}
