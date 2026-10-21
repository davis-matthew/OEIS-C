/*
 * A214308  a(n) is the number of all two colored bracelets (necklaces with
 *          turning over allowed) with n beads with the two colors from a
 *          repertoire of n distinct colors, for n>=2.
 * https://oeis.org/A214308
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = C(n,2) * (B(n) - 2) * where B(n) = number of binary
 *            bracelets (necklaces with turning over allowed) * of length n,
 *            and C(n,2) is the binomial coefficient n*(n-1)/2. * B(n) is
 *            computed by Polya's enumeration: * N(n) = (1/n) * Σ_{d|n} φ(d) *
 *            2^{n/d} -- binary necklaces * R(n) = { 2^{(n+1)/2}...
 * Range:     u8 n <= 6, u16 n <= 14, u32 n <= 29, u64 n <= 60, u128 n <= 100,
 *            big any n
 * Helpers:   phi, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"
#include "../utils/phi.h"

seq_t a(idx_t n)
{
    if (n < 2)
        return (seq_t)0;   /* undefined for n < 2 */

    /* --- compute number of binary necklaces N(n) --- */
    seq_t N = 0;
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            idx_t d1 = d;
            idx_t d2 = n / d;

            seq_t term1 = phi(d1);
            term1 *= pow2(n / d1);
            N += term1;

            if (d1 != d2) {
                seq_t term2 = phi(d2);
                term2 *= pow2(n / d2);
                N += term2;
            }
        }
    }
    N /= (seq_t)n;   /* divide by n */

    /* --- compute reflection term R(n) --- */
    seq_t refl;
    if (n & 1U) {                       /* n odd */
        refl = pow2((n + 1) / 2);
    } else {                            /* n even */
        refl = pow2(n / 2 - 1);
        refl += pow2(n / 2);
    }

    /* --- number of binary bracelets B(n) --- */
    seq_t B = (N + refl) / 2;

    /* --- final value a(n) --- */
    seq_t binom = ((seq_t)n * (seq_t)(n - 1)) / 2;  /* C(n,2) */
    seq_t result = binom * (B - 2);
    return result;
}
