/*
 * A095521  a(n) = 64 written in base 12 - n.
 * https://oeis.org/A095521
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = representation of 64 in base (12 - n) written as a
 *            decimal number
 * Range:     u8 n <= 7, u16 n <= 9, u32 n <= 10, u64 n <= 10, u128 n <= 10,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* The sequence is only defined for n = 0 .. 11 (base 12 down to base 1) */
    if (n > 11)
        return 0;

    idx_t base = 12 - n;

    /* Base 1 (unary) – 64 ones */
    if (base == 1) {
        seq_t rep = 0;
        for (idx_t i = 0; i < 64; ++i)
            rep = rep * 10 + 1;
        return rep;
    }

    seq_t rep  = 0;  /* resulting decimal number */
    seq_t mult = 1;  /* current place value (units, tens, ...) */
    idx_t  val  = 64;/* the number to convert */

    while (val != 0) {
        seq_t digit = val % base;
        rep += digit * mult;
        mult *= 10;
        val /= base;
    }

    return rep;
}
