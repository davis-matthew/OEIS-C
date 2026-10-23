/*
 * A046043  Autobiographical numbers (or curious numbers): list of numbers m =
 *          x_0 x_1 x_2 ... x_{b-1} (written in base b) such that x_i is the
 *          number of "digits" in m that are equal to i, for all i=0,...,b-1.
 * https://oeis.org/A046043
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, nice, fini, full
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 3, u32 n <= 6, u64 n <= 7, u128 n <= 7,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A046043: Autobiographical numbers.
   a(1) = 1210, a(2) = 2020, a(3) = 21200.
   For n >= 4, let b = n + 3 (base).  Then
   x0 = b-4, x1 = 2, x2 = 1, x(b-4) = 1, all other x_i = 0.
   The number is formed by concatenating the digits x0…x_{b-1}. */
seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)1210;
    if (n == 2)
        return (seq_t)2020;
    if (n == 3)
        return (seq_t)21200;

    /* n >= 4 */
    idx_t b = n + 3;                    /* base (number of digits) */
    seq_t val = 0;

    for (idx_t i = 0; i < b; ++i) {
        seq_t digit;
        if (i == 0)
            digit = (seq_t)(b - 4);
        else if (i == 1)
            digit = (seq_t)2;
        else if (i == 2)
            digit = (seq_t)1;
        else if (i == (b - 4))
            digit = (seq_t)1;
        else
            digit = (seq_t)0;

        val = val * (seq_t)10 + digit;
    }

    return val;
}
