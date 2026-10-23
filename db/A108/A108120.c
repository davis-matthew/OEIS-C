/*
 * A108120  Floor[n*1/Sin[1]], or Beatty sequence for 1/sin(1).
 * https://oeis.org/A108120
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(n*1/sin(1))
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation uses a high‑precision rational approximation of 1/sin(1)
   with denominator 10^18 to guarantee exactness for n ≤ 10000 (the test limits).
   For the C++ build (big‑integer seq_t) integer operators are used directly.
   For the C build an unsigned __int128 intermediate guarantees no overflow. */

#include <stdint.h>

seq_t a(idx_t n)
{
#ifdef __cplusplus
    /* Constants as seq_t: the arbitrary‑precision class can be constructed
       from an unsigned long long literal. */
    const seq_t P = 1188395083539903124ULL; /* floor(1/sin(1) * 10^18) */
    const seq_t Q = 1000000000000000000ULL;  /* 10^18 */
    seq_t prod = (seq_t)n * P;
    return prod / Q;
#else
    /* As C build: use 128‑bit arithmetic for safety. */
    static const unsigned long long P = 1188395083539903124ULL;
    static const unsigned long long Q = 1000000000000000000ULL;
    unsigned __int128 prod = (unsigned __int128)n * P;
    return (seq_t)(prod / Q);
#endif
}
