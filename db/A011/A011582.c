/*
 * A011582  Legendre symbol (n,11).
 * https://oeis.org/A011582
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, mult, easy
 * Formula:   a(n) = Legendre symbol (n,11). Returns 0 if n is a multiple of
 *            11; otherwise +1 for residues mod 11 (1,3,4,5,9) and -1 for
 *            non‑residues
 * Range:     i8 n <= 10000, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;
    idx_t r = n % 11;
    if (r == 0)
        return 0;
    switch (r) {
        case 1:
        case 3:
        case 4:
        case 5:
        case 9:
            return (seq_t)1;
        default:
            return (seq_t)-1;
    }
}
