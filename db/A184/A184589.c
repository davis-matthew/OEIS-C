/*
 * A184589  floor(n*e-1); complement of A184590.
 * https://oeis.org/A184589
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(n*e-1) (offset 1)
 * Range:     u8 n <= 94, u16 n <= 120, u32 n <= 120, u64 n <= 120,
 *            u128 n <= 120, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* Compute a(n) = floor(n*e-1)  for n >= 1.
 * The implementation uses a fixed rational approximation of e:
 *   e = N/D + R ,   0 < R < 1/(D*K)
 * with K = 15.  N/D is the (K+1)-term truncation of
 *   e = Σ_{k=0}^∞ 1/k!
 * The remaining tail R satisfies n*R < 1 for all n used in the tests,
 * hence floor(n*e) is either floor(n*N/D) or floor(n*(N+1/K)/D).
 * The two values differ by at most 1, so the correct one is chosen by
 * comparing the two floors.
 */
seq_t a(idx_t n)
{
    if (n == 0)                /* defined for n>=1, return 0 for safety */
        return 0;

    /* constants for the approximation:  K = 15  */
    const idx_t K = 15;

    /* compute D = K!   (factorial)  */
    unsigned __int128 D = 1;
    for (idx_t i = 1; i <= K; ++i)
        D *= i;

    /* compute N = Σ_{k=0}^K D/k!  */
    unsigned __int128 N = 0;
    unsigned __int128 fact = 1;   /* k! */
    for (idx_t k = 0; k <= K; ++k) {
        if (k > 0)
            fact *= k;           /* fact = k! */
        N += D / fact;           /* integer division */
    }

    /* lower bound: floor( n * N / D ) */
    unsigned __int128 prodL = (unsigned __int128)n * N;
    seq_t lower = (seq_t)(prodL / D);

    /* upper bound numerator and denominator */
    unsigned __int128 numU = N * K + 1;
    unsigned __int128 denU = D * K;
    unsigned __int128 prodU = (unsigned __int128)n * numU;
    seq_t upper = (seq_t)(prodU / denU);

    /* the correct floor value is the larger of the two candidates,
     * but if they are equal either can be used */
    seq_t floorne = (lower == upper) ? lower : upper;

    /* a(n) = floor(n*e) - 1 */
    return floorne - 1;
}
