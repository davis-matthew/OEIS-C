/*
 * A035776  Coordination sequence for C_39 lattice.
 * https://oeis.org/A035776
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = coefficient of x^(2n) in ((1+x)/(1-x))^39
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 3, u64 n <= 9, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth term of OEIS sequence A035776. */
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Precompute binomial(39,r) and 2^r for r = 0..39 */
    static seq_t C39[40];
    static seq_t P2[40];
    static int initialized = 0;

    if (!initialized) {
        seq_t val = 1;
        C39[0] = 1;
        for (idx_t r = 1; r <= 39; ++r) {
            /* C(39,r) = C(39,r-1) * (39-r+1)/r */
            val = val * (seq_t)(39 - r + 1) / (seq_t)r;
            C39[r] = val;
        }
        P2[0] = 1;
        for (idx_t r = 1; r <= 39; ++r) {
            P2[r] = P2[r-1] << 1;  /* multiply by 2 */
        }
        initialized = 1;
    }

    /* a(0) = 1 */
    if (n == 0) {
        return (seq_t)1;
    }

    /* k = 2n (sequence uses x^(2n) ) */
    seq_t k = (seq_t)n * (seq_t)2;

    seq_t result = 0;
    /* For each r = 1..min(39,k) compute term = C(39,r) * 2^r * C(k-1, r-1) */
    idx_t rmax = 39;
    if (k < rmax) rmax = (idx_t)k;
    for (idx_t r = 1; r <= rmax; ++r) {
        idx_t r1 = r - 1;          /* r-1 for the binomial (k-1 choose r-1) */
        seq_t bin = 1;             /* C(k-1, r-1) */
        if (r1 > 0) {
            seq_t m = k - 1;
            /* Compute multiplicatively: (m - r1 + i) / i for i=1..r1 */
            for (idx_t i = 1; i <= r1; ++i) {
                bin = bin * (m - r1 + i) / (seq_t)i;
            }
        }
        seq_t term = C39[r];
        term *= P2[r];
        term *= bin;
        result += term;
    }

    return result;
}
