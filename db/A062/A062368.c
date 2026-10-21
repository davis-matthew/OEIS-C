/*
 * A062368  Multiplicative with a(p^e) = (e+1)*(e+2)*(4*e+3)/6.
 * https://oeis.org/A062368
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 5039, u32 n <= 9793, u64 n <= 9793,
 *            u128 n <= 9793, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = ∏_{p^e‖n} (e+1)(e+2)(4e+3)/6  (multiplicative definition)
 *
 * The function factors n by trial division (sufficient for n up to at least 10 000,
 * and fast enough for larger values expected by the tests).  All arithmetic is
 * performed in the seq_t type, which may be a 64‑bit integer or an arbitrary‑precision
 * integer class, guaranteeing correctness for all values that fit.
 */
#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n) {
    /* multiplicative identity */
    seq_t result = 1;
    uint64_t nn = (uint64_t)n;   /* idx_t is uint64_t per build */

    /* factor 2 separately for speed */
    if (nn == 0) return 1;   /* defensive, should not happen for n>=1 */
    uint64_t e = 0;
    while ((nn & 1ULL) == 0) {
        nn >>= 1;
        e++;
    }
    if (e) {
        seq_t num = (seq_t)(e + 1) * (e + 2) * (4 * e + 3);
        result *= num / 6;
    }

    /* trial division by odd primes up to sqrt(nn) */
    for (uint64_t p = 3; p * p <= nn; p += 2) {
        if (nn % p == 0) {
            e = 0;
            while (nn % p == 0) {
                nn /= p;
                e++;
            }
            seq_t num = (seq_t)(e + 1) * (e + 2) * (4 * e + 3);
            result *= num / 6;
        }
    }

    /* if remaining nn is a prime > 1 */
    if (nn > 1) {
        e = 1;
        seq_t num = (seq_t)(e + 1) * (e + 2) * (4 * e + 3);
        result *= num / 6;
    }
    return result;
}
