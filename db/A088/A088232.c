/*
 * A088232  Numbers k such that 3 does not divide phi(k).
 * https://oeis.org/A088232
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 120, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A088232: Numbers k such that 3 does not divide φ(k). */
/* The function iterates over integers, computes φ(k) by prime factorization
   (used only modulo 3), and returns the n-th one.  This works for any n ≥ 1
   and satisfies the 2‑second per call restriction for n up to ~10 000. */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset 1: a(1) is the first term */
    idx_t count = 0;                /* how many qualifying numbers seen */
    seq_t k = 0;                    /* current candidate integer */

    for (;;) {
        k++;                        /* test next integer */
        seq_t temp = k;
        seq_t phi  = k;             /* start with φ(k) = k           */

        /* trial‑division factorisation up to sqrt(temp) */
        for (idx_t p = 2; (seq_t)p * (seq_t)p <= temp; ++p) {
            if (temp % p == 0) {
                /* remove all powers of p */
                while (temp % p == 0) temp /= p;
                /* φ(k) -= φ(k)/p  */
                phi -= phi / (seq_t)p;
            }
        }
        /* any remaining prime factor > 1?   */
        if (temp > 1) {
            phi -= phi / temp;
        }

        /* φ(k) intensive check */
        if (phi % 3 != 0) {         /* 3 does NOT divide φ(k) */
            ++count;
            if (count == n) {
                return k;          /* n-th such number found */
            }
        }
    }
}
