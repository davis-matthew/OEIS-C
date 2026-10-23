/*
 * A043379  Numbers having three 2's in base 6.
 * https://oeis.org/A043379
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   numbers having exactly three 2’s in base‑6, listed in increasing
 *            order
 * Range:     u8 n <= 1, u16 n <= 41, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t val = 0;          /* current candidate number */
    idx_t count = 0;        /* how many qualifying numbers have been found */
    const seq_t six = 6;    /* base */

    while (count < n) {
        /* Move to the next integer */
        val++;

        /* Count how many digits are equal to 2 in base 6 */
        seq_t tmp = val;
        int twos = 0;
        while (tmp > 0) {
            seq_t rem = tmp % six;
            if (rem == 2) twos++;
            tmp /= six;
        }

        /* If the count of 2’s is exactly 3, we have found another term */
        if (twos == 3)
            count++;
    }

    return val;
}
