/*
 * A160834  Expansion of:
 *          (1+62*x+567*x^2+1068*x^3+503*x^4+54*x^5+x^6)/(1-x)^7.
 * https://oeis.org/A160834
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 + n*(n+1)*(47*n^4+104*n^3+171*n^2+114*n+74)/15
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 32, u64 n <= 1343,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute using 128‑bit intermediate to avoid overflow */
    unsigned __int128 nn = (unsigned __int128)n;
    unsigned __int128 n1 = nn + 1;
    unsigned __int128 n2 = nn * nn;          /* n^2 */
    unsigned __int128 n3 = n2 * nn;          /* n^3 */
    unsigned __int128 n4 = n3 * nn;          /* n^4 */

    unsigned __int128 poly = 47 * n4
                           + 104 * n3
                           + 171 * n2
                           + 114 * nn
                           + 74;

    unsigned __int128 prod = nn * n1;
    prod *= poly;
    prod /= 15;
    prod += 1;

    return (seq_t)prod;
}
