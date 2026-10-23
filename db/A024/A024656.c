/*
 * A024656  n written in fractional base 9/8.
 * https://oeis.org/A024656
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 17, u16 n <= 35, u32 n <= 80, u64 n <= 350,
 *            u128 n <= 3626, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Fractional base 9/8 representation: a(n) is the decimal integer
   formed by the digits of n when written in base 9/8 (digits 0–8). */

seq_t a(idx_t n)
{
    const idx_t A = 9;          /* numerator of the fractional base */
    const idx_t B = 8;          /* denominator of the fractional base */

    /* Handle the zero case immediately. */
    if (n == 0) return 0;

    /* We may need up to about 400 digits for 64‑bit indices.
       Allocate a small fixed array. */
    unsigned digits[1024];
    unsigned len = 0;

    /* Extract the digits, least significant first. */
    idx_t cur = n;
    while (cur > 0) {
        idx_t d = cur % A;          /* digit (0..8) */
        digits[len++] = (unsigned)d;

        /* Compute the next value: (cur - d)/A * B.
           This is exact and stays within idx_t bounds. */
        cur = ((cur - d) / A) * B;
    }

    /* Build the decimal number from the most significant digit. */
    seq_t result = 0;
    for (int i = (int)len - 1; i >= 0; --i) {
        result = result * 10 + digits[i];
    }

    return result;
}
