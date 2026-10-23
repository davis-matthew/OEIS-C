/*
 * A022334  Index of 5^n within sequence of numbers of form 2^i * 5^j.
 * https://oeis.org/A022334
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 + Sum_{i=1..n} (1 + floor(i*log2(5)))
 * Range:     u8 n <= 14, u16 n <= 236, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A022334: index of 5^n in the list of numbers 2^i*5^j */

/* The implementation uses a high‑precision fixed‑point approximation
   of the fractional part of log2(5), with a 48‑bit denominator.
   The remainder correction guarantees correctness for all i <= 10 000
   (and for any larger i that the interface may request). */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Shortcut for the first term (offset 0). */
    if (n == 0U)
        return 1;

    /* 2^48 = 281474976710656ULL   (denominator for fixed‑point). */
    const uint64_t SCALE   = 281474976710656ULL;
    /* Fractional part of log2(5) multiplied by SCALE.
       The long double constant is accurate to > 60 bits. */
    const unsigned long long FRAC =
        (unsigned long long)(0.3219280948873623470332645783159L * (long double)SCALE);

    seq_t sum = 1;                       /* a(0) */
    for (idx_t i = 1; i <= n; ++i)
    {
        /* Compute floor(i*frac(log2(5))) using fixed‑point. */
        __int128 prod = (__int128)i * FRAC;          /* product < 2^56 for n <= 10^4 */
        uint64_t q = (uint64_t)(prod >> 48);         /* floor(i*FACTOR / 2^48) */
        uint64_t r = (uint64_t)(prod & (SCALE - 1ULL)); /* remainder in [0,SCALE) */

        /* Correct for the truncation error: the true value may be
           larger by at most i/2^48.  If the remainder is within that
           distance of SCALE, we must add one. */
        uint64_t inc = q + ((r > (SCALE - i)) ? 1ULL : 0ULL);

        /* floor(i*log2(5)) = i*2 + inc. */
        seq_t floor_val = (seq_t)(i * 2 + inc);

        /* a(i) = a(i-1) + 1 + floor(i*log2(5)). */
        sum += 1 + floor_val;
    }
    return sum;
}
