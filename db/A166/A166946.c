/*
 * A166946  a(n) = 1 if n is a rounded multiple of phi (1.618033989...), the
 *          larger golden ratio value; else a(n) = 0.
 * https://oeis.org/A166946
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 iff (n + 1/2) mod φ < 1 – rounded multiple of the
 *            golden ratio This implementation uses only integer arithmetic
 * Range:     u8 n <= 0, u16 n <= 0, u32 n <= 0, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    /* High‑precision rational approximation of φ = (1+√5)/2. */
    const unsigned long long PHI_NUM = 1618033988749895ULL;   /* numerator  */
    const unsigned long long PHI_DEN = 1000000000000000ULL;   /* denominator 10^15 */

    for (idx_t k = 0;; ++k) {
        /* Compute k·φ exactly using 128‑bit arithmetic (C/C++). */
        unsigned __int128 prod = (unsigned __int128)k * PHI_NUM;
        seq_t floor_kphi = (seq_t)(prod / PHI_DEN);
        seq_t rem       = (seq_t)(prod % PHI_DEN);

        /* floor(k·φ + ½) = floor(k·φ) + 1 if 2·rem ≥ PHI_DEN, else floor(k·φ). */
        seq_t r = floor_kphi + ((rem * 2 >= PHI_DEN) ? 1 : 0);

        seq_t sn = (seq_t)n;          /* cast index to seq_t for comparison */
        if (r == sn) return 1;
        if (r >  sn) return 0;
        /* otherwise r < n – continue searching for the matching k. */
    }
}
