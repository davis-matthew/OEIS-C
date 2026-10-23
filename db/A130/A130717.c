/*
 * A130717  Second differences of A130845.
 * https://oeis.org/A130717
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = { 0 if n ≡ 0 (mod 4) * (1 + n/4) if n ≡ 1 (mod 4) *
 *            -(n/2) if n ≡ 2 (mod 4) * n/4 if n ≡ 3 (mod 4) }
 * Range:     i8 n <= 83, i16 n <= 83, i32 n <= 83, i64 n <= 83, i128 n <= 83,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* use unsigned modulo to determine residue class */
    unsigned r = (unsigned)(n % 4);
    switch (r)
    {
        case 0: return 0;
        case 1: return (seq_t)(n / 4) + 1;
        case 2: return - (seq_t)(n / 2);
        case 3: return (seq_t)(n / 4);
    }
    /* unreachable, but keep compiler happy */
    return 0;
}
