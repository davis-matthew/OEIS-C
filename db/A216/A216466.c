/*
 * A216466  a(n) = n!! mod n!
 * https://oeis.org/A216466
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n!! mod n!
 * Range:     u8 n <= 7, u16 n <= 12, u32 n <= 20, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* For n ≥ 3 the double factorial is less than the factorial, so the
   result equals n!! itself. For n = 0, 1, 2 the remainder is 0. */
seq_t a(idx_t n)
{
    if (n < 3)
        return 0;

    seq_t res = 1;
    idx_t i = n;
    while (i > 0) {
        res *= (seq_t)i;
        if (i <= 2)
            break;
        i -= 2;
    }
    return res;
}
