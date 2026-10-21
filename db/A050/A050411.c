/*
 * A050411  Analog of 3x+1 function.
 * https://oeis.org/A050411
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + k + 2 if n is in Beatty sequence A (floor(k·φ)), else
 *            a(n) = k where n = floor(k·φ²). k = ceil(n/φ) or ceil(n/φ²)
 *            respectively. Computation uses integer arithmetic and the exact
 *            value of √5 obtained via floor_sqrt(5·n²)
 * Range:     u8 n <= 3, u16 n <= 57, u32 n <= 76, u64 n <= 76, u128 n <= 76,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"   // provides seq_t floor_sqrt(seq_t);

/* Computes floor(k·φ) = floor((k + floor(k·√5))/2). */
static inline seq_t floor_phi(seq_t k)
{
    seq_t sq = floor_sqrt((seq_t)5 * k * k);   /* floor(k·√5) */
    return (k + sq) / 2;                      /* floor(k·φ)   */
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;                      /* undefined for n<1 */

    seq_t n_seq = (seq_t)n;

    /* Auxiliary value: floor(n·√5) */
    seq_t sq_n = floor_sqrt((seq_t)5 * n_seq * n_seq);

    /* First guess for k in the A‑sequence: k = ceil((sq_n - n)/2) */
    seq_t kA = (sq_n - n_seq + 1) / 2;

    if (floor_phi(kA) == n_seq)                /* n is in A */
        return n_seq + kA + 2;

    /* The guess may be off by one when sq_n = floor(n·√5) < n·√5. */
    seq_t kA2 = kA + 1;
    if (floor_phi(kA2) == n_seq)                /* n is in A with k+1 */
        return n_seq + kA2 + 2;

    /* Otherwise n belongs to the complementary sequence B. */
    seq_t kB = (3 * n_seq - sq_n + 1) / 2;       /* ceil(n/φ²) */
    return kB;
}
