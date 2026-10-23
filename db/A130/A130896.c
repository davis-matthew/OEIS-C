/*
 * A130896  For D_n type groups as polyhedra that are pyramid-like:
 *          {F,V,E,dimension}->{2*n+1,2*n+1,4*n,(2*n+1)*((2*n+1)-1)/2} such
 *          that Euler's equation is true: V=E-F+2.
 * https://oeis.org/A130896
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   Let m = ⌊(n+3)/4⌋. Then a(n) = 2*m+1 if n mod 4 == 1 or 2, a(n)
 *            = 4*m if n mod 4 == 3, a(n) = m*(2*m+1) if n mod 4 == 0
 * Range:     u8 n <= 47, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* undefined for offset 1, safe fallback */

    /* Compute m = floor((n+3)/4) */
    seq_t m = (seq_t)(n + 3) / 4;

    idx_t r = n % 4;                     /* n modulo 4 */

    seq_t result;
    switch (r) {
        case 1:
        case 2:
            result = 2 * m + 1;
            break;
        case 3:
            result = 4 * m;
            break;
        default:   /* r == 0 */
            result = m * (2 * m + 1);
            break;
    }

    return result;
}
