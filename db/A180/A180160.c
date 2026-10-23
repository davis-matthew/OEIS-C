/*
 * A180160  (sum of digits) mod (number of digits) of n in decimal
 *          representation.
 * https://oeis.org/A180160
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = A007953(n) mod A055642(n)
 * Range:     u8 n <= 9581, u16 n <= 9581, u32 n <= 9581, u64 n <= 9581,
 *            u128 n <= 9581, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sum = 0;
    idx_t tmp = n;
    unsigned int digits = 0;

    /* Compute digit sum and count of digits */
    do {
        sum += (seq_t)(tmp % 10);
        tmp /= 10;
        digits++;
    } while (tmp != 0);

    /* Return sum modulo number of digits */
    return sum % (seq_t)digits;
}
