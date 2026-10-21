/*
 * A056027  Numbers k such that k^16 == 1 (mod 17^2).
 * https://oeis.org/A056027
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 3628, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A056027 – numbers k such that k^16 ≡ 1 (mod 17² = 289).
   The residues modulo 289 satisfying this congruence are found by a
   simple search (k = 1 … 288).  Let R[0] … R[15] be these 16 residues
   sorted increasingly.  Every solution is then
      k = R[i] + 289 · m   (m ≥ 0).
   The nth term (n ≥ 1) is obtained by
      i   = (n – 1) mod 16
      m   = (n – 1) / 16
      a(n) = R[i] + 289 · m
   The implementation keeps R cached after the first call.  No
   external libraries are required. */

#include <stddef.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Cached residue set */
    static int    initialized = 0;
    static seq_t R[16];
    static size_t rcount;
    
    if (!initialized) {
        rcount = 0;
        for (uint32_t k = 1; k < 289; ++k) {
            uint32_t r = 1;
            for (int e = 0; e < 16; ++e)
                r = (uint32_t)((uint32_t)r * k % 289);
            if (r == 1) {
                R[rcount++] = (seq_t)k;   /* residues already in ascending order */
            }
        }
        /* In principle rcount should be 16; assume the congruence
           yields exactly 16 residues.  No further checks are needed. */
        initialized = 1;
    }

    /* Calculate indices */
    idx_t idx   = n - 1;               /* 0‑based index */
    size_t pos  = (size_t)(idx % 16);  /* residue index 0…15   */
    idx_t  quo  = idx / 16;            /* multiplier for 289   */

    /* Compute a(n) = R[pos] + 289 * quo.
       Use a loop by repeated doubling to avoid overflow of 64‑bit
       arithmetic for very large n (the result is truncated to seq_t).
    */
    seq_t result = R[pos];
    seq_t mult   = (seq_t)289;
    idx_t q      = quo;
    while (q) {
        if (q & 1) result += mult;
        mult *= 2;
        q >>= 1;
    }
    return result;
}
