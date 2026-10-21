/*
 * A072998  To get a(n), write n in balanced ternary notation (using only
 *          digits -1, 0, 1, -1), then change -1's to 0's, 0's to 1's, and 1's
 *          to 2's.
 * https://oeis.org/A072998
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = decimal number obtained by writing n in balanced ternary,
 *            replacing each digit -1→0, 0→1, 1→2
 * Range:     u8 n <= 13, u16 n <= 44, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Special case: n == 0 → balanced digit 0 → mapped to 1. */
    if (n == 0)
        return (seq_t)1;

    /* Temporary storage for the mapped digits (0‑2).  Balanced ternary
       length for a 64‑bit integer is at most 41, so 64 is safe. */
    unsigned char digits[64];
    unsigned pos = 0;

    /* Convert n to balanced ternary while mapping digits to 0‑2. */
    while (n > 0) {
        /* Compute remainder modulo 3. */
        unsigned r = n % 3;
        n /= 3;

        /* Balanced ternary digit: 0, 1, or -1. */
        int d = 0;
        if (r == 2) {
            d = -1;
            n += 1;                 /* carry */
        } else {
            d = (int)r;              /* 0 or 1 */
        }

        /* Mapped digit: -1→0, 0→1, 1→2 */
        digits[pos++] = (unsigned char)(d + 1);
    }

    /* Assemble decimal value from most significant digit to least. */
    seq_t result = 0;
    for (int i = (int)pos - 1; i >= 0; --i) {
        result = result * (seq_t)10 + (seq_t)digits[i];
    }
    return result;
}
