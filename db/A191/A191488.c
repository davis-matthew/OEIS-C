/*
 * A191488  A companion to Gould's sequence A001316.
 * https://oeis.org/A191488
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 62, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: Let p = v₂(n+1) (the exponent of the largest power of two dividing n+1).
 * Let m   = (n+1) >> p  (the odd part of n+1).  
 * Let k   = (m - 1) >> 1.  
 * Then a(n) = (2^(p+1)+2) * 2^{popcount(k)} .
 * This follows from the given relation 
 * a((2·k+1)·2^p−1) = (2^(p+1)+2) · A001316(k)  with A001316(k)=2^{popcount(k)}.
 */

/* include utilities needed for big powers of two */
#include "../utils/pow2.h"

/* compute popcount of a 64‑bit integer */
#if defined(__GNUC__) || defined(__clang__)
static inline unsigned popcount64(idx_t x) { return __builtin_popcountll(x); }
#else
static inline unsigned popcount64(idx_t x)
{
    unsigned c = 0;
    while (x) { x &= x - 1; c++; }
    return c;
}
#endif

seq_t a(idx_t n)
{
    idx_t m = n + 1ULL;           /* n+1   */
    idx_t tmp = m;
    idx_t p = 0;                  /* v₂(n+1) */
    while ((tmp & 1ULL) == 0) {   /* leap over factors of 2 */
        tmp >>= 1;
        ++p;
    }
    idx_t odd = tmp;              /* odd part */
    idx_t k = (odd - 1ULL) >> 1ULL;   /* k = (odd-1)/2 */

    unsigned pop = popcount64(k); /* popcount(k) */

    seq_t C = pow2(p + 1) + 2;    /* C(p) = 2^(p+1)+2   */
    seq_t P = pow2(pop);          /* 2^{popcount(k)}    */

    return C * P;
}
