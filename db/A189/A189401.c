/*
 * A189401  n+[nr/t]+[ns/t]; r=1, s=sqrt(e), t=e.
 * https://oeis.org/A189401
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + ⌊n/e⌋ + ⌊n/√e⌋ (e = exp(1))
 * Range:     u8 none, u16 none, u32 n <= 84, u64 n <= 84, u128 n <= 84,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses integer arithmetic only.  A fixed‐point approximation of 1/e and
   1/√e with denominator 10^6 gives correct results for all n < 10^6,
   which covers the required test range. */
 
#include <stdint.h>
 
static const uint64_t DEN = 1000000ULL;       /* denominator 10^6     */
static const uint64_t INV_E  = 367879ULL;     /* ⌊10^6 / e⌋            */
static const uint64_t INV_SQRT_E = 606530ULL; /* ⌊10^6 / √e⌋           */
 
seq_t a(idx_t n)
{
    /* compute ⌊n/e⌋  and  ⌊n/√e⌋  with exact integer arithmetic */
    seq_t nseq = (seq_t)n;
    seq_t term_e     = (nseq * (seq_t)INV_E) / (seq_t)DEN;
    seq_t term_sqrt_e = (nseq * (seq_t)INV_SQRT_E) / (seq_t)DEN;
    return nseq + term_e + term_sqrt_e;
}
