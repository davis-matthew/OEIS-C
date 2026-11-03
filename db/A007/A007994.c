/*
 * A007994  Poincaré series (or Poincare series) of Lie algebra associated
 *          with a certain braid group.
 * https://oeis.org/A007994
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   G.f.:
 *            x*(8*x^9-54*x^8+144*x^7-178*x^6+60*x^5+102*x^4-137*x^3+70*x^2-18*x+4)/(1-x)^10,
 *            i.e. a(n) = Sum_{j=0..9} p_j * binomial(n+8-j, 9) with
 *            (p_0..p_9) = (4,-18,70,-137,102,60,-178,144,-54,8)
 * Range:     u8 n <= 3, u16 n <= 9, u32 n <= 43, u64 n <= 568, u128 n <= 1000,
 *            big any n
 * Validated: standard, pass, 44 terms through n = 905 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* binomial(N, 9) computed by cancelling 9! from the nine factors first,
   so the remaining product is exact (or exact modulo 2^bits). */
static seq_t c9(uint64_t N) {
    uint64_t f[9];
    static const uint64_t primes[4] = {2, 3, 5, 7};
    static const int mult[4] = {7, 4, 1, 1};
    int i, k, r;
    seq_t res;
    if (N < 9) return (seq_t)0;
    for (i = 0; i < 9; i++) f[i] = N - (uint64_t)i;
    for (k = 0; k < 4; k++) {
        for (r = 0; r < mult[k]; r++) {
            for (i = 0; i < 9; i++) {
                if (f[i] % primes[k] == 0) { f[i] /= primes[k]; break; }
            }
        }
    }
    res = (seq_t)1;
    for (i = 0; i < 9; i++) res *= (seq_t)f[i];
    return res;
}

seq_t a(idx_t n) {
    seq_t pos, neg;
    pos = (seq_t)4 * c9(n + 8) + (seq_t)70 * c9(n + 6) + (seq_t)102 * c9(n + 4)
        + (seq_t)60 * c9(n + 3) + (seq_t)144 * c9(n + 1) + (seq_t)8 * c9(n - 1);
    neg = (seq_t)18 * c9(n + 7) + (seq_t)137 * c9(n + 5) + (seq_t)178 * c9(n + 2)
        + (seq_t)54 * c9(n);
    return pos - neg;
}
