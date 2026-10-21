/*
 * A208647  Numerators of Pokrovskiy's lower bound on the ratio of e(G^n) the
 *          number of edges in the n-th power of a graph G, to E(G) the number
 *          of edges of G.
 * https://oeis.org/A208647
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, frac
 * Formula:   a(n) = 2*[(n/3)+1]^2 - 1 if n % 3 == 0 ceil(n/3) otherwise (with
 *            ceil(n/3) = (n + 2) / 3)
 * Range:     u8 n <= 15, u16 n <= 15, u32 n <= 15, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* compute n mod 3 using unsigned arithmetic */
    idx_t mod = n % 3;

    if (mod == 0) {
        /* n = 3k; k = n / 3 */
        seq_t k   = (seq_t)(n / 3);
        seq_t k1  = k + 1;                /* k+1 */
        /* numerator = 2*(k+1)^2 - 1 */
        return (2 * k1 * k1) - 1;
    } else {
        /* ceil(n/3) = (n + 2) / 3 */
        return ((seq_t)n + 2) / 3;
    }
}
