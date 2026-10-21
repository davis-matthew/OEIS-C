/*
 * A226894  Position of n-th harmonic number H(n) in the joint ranking of
 *          {H(k)} and {log k}, for k >= 1; complement of A226896.
 * https://oeis.org/A226894
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n + ⌊e^(H(n))⌋, where H(n) = Σ_{k=1}^n 1/k
 * Range:     u8 n <= 91, u16 n <= 1002, u32 n <= 1002, u64 n <= 1002,
 *            u128 n <= 1002, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>

seq_t a(idx_t n)
{
    /* compute the n‑th harmonic number as a long double */
    long double H = 0.0L;
    for (idx_t k = 1; k <= n; ++k)
        H += 1.0L / (long double)k;

    /* compute ⌊e^H⌋.  The small epsilon shields against rounding errors. */
    long double eH = expl(H);
    idx_t m = (idx_t)floorl(eH + 1e-12L);

    /* correct possible off‑by‑one errors caused by floating point inaccuracy */
    while (m + 1 > 0 && logl((long double)(m + 1)) <= H)
        ++m;
    while (m > 0 && logl((long double)m) >= H)
        --m;

    return (seq_t)(n + m);
}
