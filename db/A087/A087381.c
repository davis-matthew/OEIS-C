/*
 * A087381  Let Compri(n) be the number obtained by replacing each prime digit
 *          (2,3,5,7) of n with a '1' and a composite digit( 0,4,6,8,9) with a
 *          '0'. A 1 remains the same. a(n) = Compri(n).
 * https://oeis.org/A087381
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = Compri(n) – replace every prime digit 2,3,5,7 with 1,
 * Range:     u8 n <= 80, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

// every composite digit 0,4,6,8,9 with 0, and keep 1 as 1.

seq_t a(idx_t n) {
    idx_t temp = n;
    /* At most 20 decimal digits for 64‑bit n – safe for all builds. */
    int digits[20];
    unsigned count = 0;

    /* Extract digits in reverse order. */
    do {
        digits[count++] = (int)(temp % 10);
        temp /= 10;
    } while (temp != 0);

    seq_t result = 0;

    /* Build the result from most significant digit to least. */
    for (int i = count - 1; i >= 0; --i) {
        int d = digits[i];
        int mapped;
        if (d == 1) {
            mapped = 1;
        } else if (d == 2 || d == 3 || d == 5 || d == 7) {
            mapped = 1;
        } else { /* 0,4,6,8,9 */
            mapped = 0;
        }
        result = result * 10 + mapped;
    }
    return result;
}
