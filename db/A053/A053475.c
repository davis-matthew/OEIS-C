/*
 * A053475  1 + the number of iterations of A051953 (Euler-cototient) function
 *          needed to reach 0, starting at n.
 * https://oeis.org/A053475
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of terms in the sequence n, n-φ(n),
 *            (n-φ(n))-φ(...), … until 0 (including both endpoints)
 * Range:     u8 n <= 1279, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Compute the Euler totient φ(m) for m ≥ 1.  The function works for any
   seq_t type as long as m fits in a uint64_t (true for the OEIS range). */
static seq_t phi_seq(seq_t m)
{
    if (m == 0) return 0;            /* φ(0) is never needed */
    uint64_t mm = (uint64_t)m;
    seq_t result = (seq_t)mm;
    for (uint64_t p = 2; p * p <= mm; ++p) {
        if (mm % p == 0) {
            while (mm % p == 0) mm /= p;
            result = result / (seq_t)p * (seq_t)(p - 1);
        }
    }
    if (mm > 1) {
        result = result / (seq_t)mm * (seq_t)(mm - 1);
    }
    return result;
}

seq_t a(idx_t n)
{
    seq_t curr = (seq_t)n;
    seq_t count = 1;   /* start term */
    while (curr > 0) {
        seq_t phi = phi_seq(curr);
        curr = curr - phi;
        count++;
    }
    return count;
}
