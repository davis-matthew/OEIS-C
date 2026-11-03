/*
 * A069904  Number of prime factors of n-th triangular number (with
 *          multiplicity).
 * https://oeis.org/A069904
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Ω(n) + Ω(n+1) – 1 where Ω(k) is the number of prime
 *            factors of k counted with multiplicity
 * Range:     u8 n <= 530, u16 n <= 20000, u32 n <= 20000, u64 n <= 20000,
 *            u128 n <= 20000, big any n
 * Validated: standard, pass, 106 terms through n = 9725 (2026-11-01)
 * Helpers:   bigomega_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/bigomega_seq.h"

seq_t a(idx_t n)
{
    if (n == 0) {
        /* The sequence is defined for n ≥ 1; return 0 for safety. */
        return 0;
    }
    seq_t omega_n   = bigomega_seq((seq_t)n);
    seq_t omega_np1 = bigomega_seq((seq_t)(n + 1));
    return omega_n + omega_np1 - 1;
}
