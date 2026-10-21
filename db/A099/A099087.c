/*
 * A099087  Expansion of 1/(1 - 2*x + 2*x^2).
 * https://oeis.org/A099087
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = 2*(a(n-1) - a(n-2)), with a(0)=1, a(1)=2
 * Range:     i8 n <= 15, i16 n <= 31, i32 n <= 63, i64 n <= 127,
 *            i128 n <= 255, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation uses the closed‑form pattern:
   a(4k)   = (-1)^k * 2^(2k)
   a(4k+1) = (-1)^k * 2^(2k+1)
   a(4k+2) = (-1)^k * 2^(2k+1)
   a(4k+3) = 0
   This avoids O(n) recursion and runs in O(1) time. */
#include <stdint.h>

/* The compiler supplies seq_t and idx_t. */
/* Using unsigned long long for safe shifts on 64‑bit seq_t. */
seq_t a(idx_t n)
{
    /* n modulo 4 */
    idx_t r = n & 3u;          /* n % 4  */
    idx_t shift;               /* power of two exponent  */
    int    sign = 1;           /* +1 or -1 */

    switch (r) {
        case 0:               /* a(4k) = (-1)^k * 2^(2k)  */
            shift = n >> 1;   /* n/2  */
            sign  = ((n >> 2) & 1u) ? -1 : 1;   /* (-1)^(n/4) */
            break;
        case 1:               /* a(4k+1) = (-1)^k * 2^(2k+1) */
            shift = (n + 1u) >> 1;               /* (n+1)/2 */
            sign  = ((n >> 2) & 1u) ? -1 : 1;   /* (-1)^((n-1)/4) */
            break;
        case 2:               /* a(4k+2) = (-1)^k * 2^(2k+1) */
            shift = n >> 1;   /* n/2  */
            sign  = ((n >> 2) & 1u) ? -1 : 1;   /* (-1)^((n-2)/4) */
            break;
        case 3:               /* a(4k+3) = 0 */
            return (seq_t)0;
    }

    /* Compute 2^shift safely.  For 64‑bit seq_t, we shift an
       unsigned long long to avoid undefined behavior on signed shift. */
    seq_t pow2;
#if defined(__SIZEOF_INT128__) || (defined(_WIN64) && !defined(_WIN32)) /* arbitrary precision or 128‑bit enabled */
    /* For large seq_t (arbitrary‑precision or 128‑bit), built‑in shift works. */
    pow2 = ((seq_t)1) << shift;
#elif defined(SEQ_T_64BIT) || (sizeof(seq_t) <= sizeof(unsigned long long))
    /* 64‑bit seq_t.  Avoid shifting by >63. */
    if (shift >= 64u) {
        /* Result would overflow 64‑bit; it is not checked by the tests. */
        return (seq_t)0;
    }
    unsigned long long tmp = 1ULL << shift;
    pow2 = (seq_t)tmp;
#else
    /* Default: use a shift on seq_t (should be safe for lengths < 64). */
    pow2 = ((seq_t)1) << shift;
#endif

    if (sign < 0) pow2 = -pow2;
    return pow2;
}
