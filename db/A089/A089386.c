/*
 * A089386  Smallest number with digital product = 10^n.
 * https://oeis.org/A089386
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 14, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Construction of the minimal integer whose decimal
 * digits product equals 10^n.
 * For n = 0 return 1.
 * For n > 0: let q = n / 3, r = n % 3.
 *   - always use n copies of the digit 5 (each contributes one factor 5).
 *   - use floor(n/3) copies of the digit 8 (each contributes three factors 2).
 *   - if r == 1, add one digit 2 (one factor 2).
 *   - if r == 2, add one digit 4 (two factors 2).
 * The digits are then sorted ascending (2,4,5,8) to obtain the smallest integer.  */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;

    idx_t q = n / 3;
    idx_t r = n % 3;

    idx_t x2 = 0, x4 = 0, x8 = q;
    if (r == 1) {
        x2 = 1;
    } else if (r == 2) {
        x4 = 1;
    }

    /* Build the result by appending digits in ascending order. */
    seq_t res = 0;

    /* Append '2's */
    for (idx_t i = 0; i < x2; ++i) {
        res = res * (seq_t)10 + (seq_t)2;
    }
    /* Append '4's */
    for (idx_t i = 0; i < x4; ++i) {
        res = res * (seq_t)10 + (seq_t)4;
    }
    /* Append '5's (n copies) */
    for (idx_t i = 0; i < n; ++i) {
        res = res * (seq_t)10 + (seq_t)5;
    }
    /* Append '8's */
    for (idx_t i = 0; i < x8; ++i) {
        res = res * (seq_t)10 + (seq_t)8;
    }

    return res;
}
