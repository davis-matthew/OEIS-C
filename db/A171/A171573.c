/*
 * A171573  a(n) = 0+1+2+...+n in lunar arithmetic in base 5 written in base
 *          5.
 * https://oeis.org/A171573
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   For each base‑5 digit position k (value 5^k), the digit of a(n)
 *            in that position equals 4 whenever n ≥ 4·5^k, otherwise it
 *            equals the k‑th base‑5 digit of n itself. The resulting base‑5
 *            number is returned as a decimal integer
 * Range:     u8 n <= 58, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Find the most significant base‑5 position that may appear in n. */
    idx_t pow = 1;
    while (pow * 5 <= n)
        pow *= 5;

    seq_t result = 0;

    /* Process positions from most significant down to 0.
       For each position the digit is 4 if the corresponding power of
       five is small enough to contain a digit 4 in some number ≤ n. */
    for (; pow; pow /= 5) {
        seq_t d;
        if (pow <= n / 4)          /* n ≥ 4·pow */
            d = 4;
        else
            d = (seq_t)((n / pow) % 5);  /* digit of n at this position */

        result = result * 10 + d;  /* build the decimal representation
                                      of the base‑5 number */
    }

    return result;
}
