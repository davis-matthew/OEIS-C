/*
 * A141105  Upper Even Swappage of Upper Wythoff Sequence.
 * https://oeis.org/A141105
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(n * φ²) + (floor(n * φ²) mod 2) i.e. return the
 *            next even integer ≥ floor(n * φ²), where φ = (1 + √5)/2. The
 *            computation uses an integer approximation of φ to avoid
 *            floating‑point arithmetic
 * Range:     u8 n <= 97, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static const unsigned long long SCALE  = 10000000000000ULL;            /* 10^13  */
static const unsigned long long PHI_N = 16180339887498ULL;            /* floor(φ * SCALE) */

seq_t a(idx_t n)
{
    /* Compute floor(n * φ)  using (n * PHI_N) / SCALE          */
    __int128 prod = (__int128)n * PHI_N;
    seq_t floor_n_phi = (seq_t)(prod / SCALE);

    /* floor(n * φ²) = n + floor(n * φ)                         */
    seq_t floor_n_phi2 = (seq_t)n + floor_n_phi;

    /* If floor_n_phi2 is odd, add 1 to make it even             */
    return floor_n_phi2 + (floor_n_phi2 & 1ULL);
}
