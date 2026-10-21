/*
 * A055149  The first n digits of the juxtaposition of the base-8 numbers
 *          converted to decimal.
 * https://oeis.org/A055149
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the first n digits of the concatenation of the base‑8
 *            representations of the positive integers, interpreted as an
 *            octal number and converted to decimal
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t result = 0;              /* current decimal value */
    idx_t digits_needed = n;       /* number of octal digits still to process */
    idx_t number = 1;              /* next integer to convert to base 8 */

    /* buffer for octal digits of a single number (max 32 digits for 64‑bit values) */
    unsigned char oct[32];

    while (digits_needed > 0) {
        /* convert 'number' to octal, store digits in oct[0..len-1] (least significant first) */
        idx_t temp = number;
        idx_t len = 0;
        do {
            oct[len++] = temp % 8;
            temp /= 8;
        } while (temp);

        /* process digits from most significant to least */
        while (len-- > 0) {
            seq_t digit = (seq_t)oct[len];
            result = result * 8 + digit;
            --digits_needed;
            if (digits_needed == 0)
                return result;
        }
        ++number;
    }
    return result; /* never reached */
}
