/*
 * A062958  Numbers k such that phi(k+1) < 2*phi(k), where phi = A000010.
 * https://oeis.org/A062958
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the n‑th integer k satisfying φ(k+1) < 2·φ(k)
 * Range:     u8 n <= 182, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A062958: Numbers k such that φ(k+1) < 2·φ(k)   (offset 1) */

/* Note: the utility header "../utils/phi.h" defines the helper
   function static seq_t phi(idx_t m), which computes Euler's totient
   φ(m).  We use this function directly. */

#include "../utils/phi.h"

seq_t a(idx_t n)
{
    /* Since the sequence is ordered increasingly, we scan k from 1
       upward and count how many k satisfy the condition until we reach
       the n‑th one.  The algorithm runs in O(n·polylog n) time, which
       is easily fast for n ≤ 10000.  We keep the current totient value
       cached so that each iteration only requires one call to phi(). */
    idx_t k = 1;          /* current candidate k              */
    idx_t count = 0;      /* how many k’s found so far       */
    seq_t phi_k = phi(1); /* φ(k) for the current k          */

    while (1) {
        seq_t phi_k1 = phi(k + 1);   /* φ(k+1) */
        if (phi_k1 < phi_k * 2) {    /* condition holds */
            ++count;
            if (count == n) {
                return (seq_t)k;
            }
        }
        phi_k = phi_k1;   /* advance φ(k) for next iteration */
        ++k;
    }
}
