/*
 * A162537  a(n) is the smallest positive multiple k of n such that every
 *          length of the runs of 0's and 1's in the binary representation of
 *          k is coprime to n.
 * https://oeis.org/A162537
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 59, u32 n <= 75, u64 n <= 75, u128 n <= 75,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) of OEIS A162537 by brute‑force search.  */
#include <stdbool.h>
#include "../utils/gcd.h"

/* Return true if every run of consecutive equal bits in val has length
   coprime to n.  The function consumes a copy of val. */
static bool runs_coprime(seq_t val, idx_t n)
{
    seq_t nseq = (seq_t)n;
    while (val != (seq_t)0) {
        seq_t bit = val & (seq_t)1;   /* current bit (0 or 1) */
        seq_t run = 0;
        while (val != (seq_t)0 && (val & (seq_t)1) == bit) {
            run++;
            val >>= 1;
        }
        if (gcd(run, nseq) != (seq_t)1)
            return false;
    }
    return true;
}

/* Smallest positive multiple of n whose binary representation has
   all run lengths coprime to n.  */
seq_t a(idx_t n)
{
    if (n == 0)          /* n ≥ 1 by definition, but guard anyway */
        return (seq_t)0;

    for (idx_t m = 1; ; ++m) {
        seq_t k = (seq_t)n * (seq_t)m;
        if (runs_coprime(k, n))
            return k;
    }
}
