/*
 * A008963  Initial digit of Fibonacci number F(n).
 * https://oeis.org/A008963
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = A000030(A000045(n)) – the first decimal digit of the n‑th
 *            Fibonacci number. For n≥6: a(n) = floor(10^frac( n*log10(φ) –
 *            log10(√5) )) (φ = (1+√5)/2). Special cases: a(0)=0, a(1)=1,
 *            a(2)=1, a(3)=2, a(4)=3, a(5)=5
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* Helper constants – fixed point with 12 decimal digits (scale M = 10^12). */
static const uint64_t M = 1000000000000ULL;          /* 10^12 */
static const uint64_t ALPHA_M = 208987640250ULL;     /* round( log10(φ) * M ) */
static const uint64_t BETA_M  = 349485002168ULL;     /* round( log10(√5) * M ) */

/* Thresholds for the interval [log10(d), log10(d+1)) */
static const uint64_t THRESH[10] = {
    0ULL,                      /* dummy 0‑th element */
    301029995664ULL,           /* log10(2) */
    477121254720ULL,           /* log10(3) */
    602059991328ULL,           /* log10(4) */
    698970004336ULL,           /* log10(5) */
    778151250384ULL,           /* log10(6) */
    845098040014ULL,           /* log10(7) */
    903089987112ULL,           /* log10(8) */
    954242509439ULL,           /* log10(9) */
    1000000000000ULL           /* log10(10) */
};

seq_t a(idx_t n) {
    /* Handle the first few terms directly. */
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)1;
    if (n == 3) return (seq_t)2;
    if (n == 4) return (seq_t)3;
    if (n == 5) return (seq_t)5;

    /* Compute fractional part of n*log10(φ) – log10(√5) in fixed point. */
    __int128 prod = (__int128)n * (__int128)ALPHA_M;   /* n*α_M  */
    __int128 diff = prod - (__int128)BETA_M;           /* nα_M – β_M */
    /* Modulo M, ensuring a positive result. */
    uint64_t f_int = (uint64_t)((diff % (__int128)M + (__int128)M) % (__int128)M);

    /* Find the digit d such that f_int < THRESH[d]. */
    for (int d = 1; d <= 9; ++d) {
        if (f_int < THRESH[d]) {
            return (seq_t)d;
        }
    }
    /* Should never reach here. */
    return (seq_t)0;
}
