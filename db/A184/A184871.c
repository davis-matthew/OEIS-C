/*
 * A184871  n+floor(ns/r)+floor(nt/r), where r=log(2), s=log(3), t=log(5).
 * https://oeis.org/A184871
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + floor(n*log_2 3) + floor(n*log_2 5) where log_2 3 ≈
 *            1.584962500721156 ... and log_2 5 ≈ 2.321928094887362 ... The
 *            implementation uses a rational approximation: floor(n*α) =
 *            floor( (α*10^12 * n) / 10^12 ). The constants below are α*10^12
 *            rounded to nearest integer. The computation uses...
 * Range:     u8 n <= 52, u16 n <= 120, u32 n <= 120, u64 n <= 120,
 *            u128 n <= 120, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

#define LOG_FACTOR 1000000000000ULL   /* 10^12 */

static const unsigned long long P3 = 1584962500721ULL;  /* round(log_2 3 * 10^12) */
static const unsigned long long P5 = 2321928094887ULL;  /* round(log_2 5 * 10^12) */

seq_t a(idx_t n)
{
    /* compute floor(n * log2(3)) and floor(n * log2(5)) */
    unsigned __int128 prod3 = (unsigned __int128)n * P3;
    unsigned __int128 prod5 = (unsigned __int128)n * P5;

    unsigned long long f3 = (unsigned long long)(prod3 / LOG_FACTOR);
    unsigned long long f5 = (unsigned long long)(prod5 / LOG_FACTOR);

    seq_t result = (seq_t)n + (seq_t)f3 + (seq_t)f5;
    return result;
}
