/*
 * A037659  Decimal expansion of a(n) is given by the first n terms of the
 *          periodic sequence with initial period 3,0,2.
 * https://oeis.org/A037659
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 10 * a(n-1) + d(n), where d(n) = 3,0,2 repeating. a(1) =
 *            3. This generates the integer whose decimal digits are the first
 *            n terms of the periodic sequence 3,0,2,3,0,2,
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 19, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t val = 0;
    idx_t rem = 1;   /* 1 -> 3, 2 -> 0, 3 -> 2 */

    for (idx_t i = 1; i <= n; ++i) {
        seq_t digit;
        switch (rem) {
            case 1: digit = 3; break;
            case 2: digit = 0; break;
            default: digit = 2; break;   /* rem == 3 */
        }
        val = val * 10 + digit;

        rem++;
        if (rem > 3) rem = 1;
    }

    return val;
}
