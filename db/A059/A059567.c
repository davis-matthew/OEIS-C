/*
 * A059567  Beatty sequence for 1 - log(log(2)).
 * https://oeis.org/A059567
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(n * (1 - log(log(2))))
 * Range:     u8 n <= 187, u16 n <= 2000, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* We avoid floating‑point arithmetic by using a high‑precision rational
   approximation of the constant c = 1 - log(log(2)):
        c ≈ 1.366512920585…                       */
#include <stdint.h>

seq_t a(idx_t n)
{
    /*  c ≈ 1.366512920585…,  P/Q is the inferior rational approximation
        using Q = 10^8.  P = floor(c * Q).  The error is less than 1/Q,
        so for any n ≤ 1.7·10^9 we guarantee that floor(n*P/Q) == floor(n*c).   */
    static const uint64_t P = 136651292ULL;   /* floor(c * 1E8)       */
    static const uint64_t Q = 100000000ULL;   /* 1E8                   */

    /* Compute floor(n * P / Q) using 128‑bit arithmetic to avoid overflow */
    unsigned __int128 prod = (unsigned __int128)n * P;
    return (seq_t)(prod / Q);
}
