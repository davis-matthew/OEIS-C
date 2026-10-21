/*
 * A094757  Least positive k <= n such that n*pi(k) = k*pi(n), where pi(n) is
 *          the number of primes <= n (A000720).
 * https://oeis.org/A094757
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 255, u16 n <= 9349, u32 n <= 9349, u64 n <= 9349,
 *            u128 n <= 9349, big any n
 * Helpers:   compute_pi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute a(n) = least positive k ≤ n satisfying n*pi(k) = k*pi(n)
   by brute‑force search using the provided compute_pi routine. */

#include "../utils/compute_pi.h"

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;          /* undefined indices return 0 */

    seq_t pi_n = compute_pi(n);           /* pre‑compute pi(n) once */

    for (idx_t k = 1; k <= n; ++k) {
        seq_t pi_k = compute_pi(k);
        if ((seq_t)n * pi_k == (seq_t)k * pi_n)
            return (seq_t)k;             /* least k found */
    }

    return (seq_t)n;                     /* fallback: k == n always works */
}
