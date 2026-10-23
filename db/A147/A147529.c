/*
 * A147529  Numbers n such that there exists x in N : (x+1)^3 - x^3 = 103*n^2.
 * https://oeis.org/A147529
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n+2) = 128405450990*a(n+1) - a(n), a(1)=8827,
 *            a(2)=1133434915879903
 * Range:     u8 none, u16 n <= 1, u32 n <= 1, u64 n <= 2, u128 n <= 4,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* offset not used */
    if (n == 1) return 8827;
    if (n == 2) return 1133434915879903ULL;

    static const seq_t coeff = 128405450990ULL;
    seq_t prev2 = 8827;                    /* a(1) */
    seq_t prev1 = 1133434915879903ULL;     /* a(2) */
    seq_t curr;

    for (idx_t i = 3; i <= n; ++i) {
        curr = coeff * prev1 - prev2;
        prev2 = prev1;
        prev1 = curr;
    }
    return curr;
}
