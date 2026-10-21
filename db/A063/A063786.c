/*
 * A063786  G.f.: A(x) =
 *          (x-2*x^2-2*x^3-(1+x)*sqrt(1-4*x^2)+sqrt(1-4*x^6))/(2*x^2).
 * https://oeis.org/A063786
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Catalan(floor((n-1)/2)) for odd n. For even n: if n ≡ 4
 *            (mod 6) then a(n) = Catalan(3*((n-4)/6) + 2) – Catalan((n-4)/6)
 *            else a(n) = Catalan(n/2)
 * Range:     u8 n <= 11, u16 n <= 19, u32 n <= 35, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   catalan
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Offset 2: a(2) is the first term listed. */
#include "../utils/catalan.h"

seq_t a(idx_t n)
{
    /* Any n < 2 is outside the offset; return 0 as a safe default. */
    if (n < 2) return 0;

    /* Odd indices use the simple Catalan formula. */
    if ((n & 1) == 1) {
        idx_t m = (n - 1) >> 1;           /* (n-1)/2 */
        return catalan(m);
    }

    /* Even indices: check for n ≡ 4 (mod 6). */
    if ((n % 6) != 4) {
        idx_t m = n >> 1;                  /* n/2 */
        return catalan(m);
    }

    /* n = 6*k + 4; use the corrected formula. */
    idx_t k   = (n - 4) / 6;                 /* k = (n-4)/6  */
    seq_t c1  = catalan(3 * k + 2);         /* Catalan(3k+2) */
    seq_t c2  = catalan(k);                 /* Catalan(k)    */
    return c1 - c2;
}
