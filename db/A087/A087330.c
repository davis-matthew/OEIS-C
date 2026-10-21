/*
 * A087330  Sum of all digits of all integers less than or equal to 555...55
 *          (with n 5's) in base 10.
 * https://oeis.org/A087330
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn, easy
 * Formula:   a(n) = 20/9*n - 110/81*10^n + 110/81 + 5/2*n*10^n (equivalent to
 *            a(n) = (360*n + 405*n*10^n - 220*10^n + 220) / 162)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* pow10[i] = 10^i as seq_t */
static seq_t *pow10 = NULL;
static size_t pow10_len = 0;

/* Ensure that pow10[0..max_n] are ready.  pow10[0] is always 1. */
static void ensure_pow10(idx_t max_n)
{
    size_t need = (size_t)max_n + 1;
    if (need <= pow10_len)
        return;

    pow10 = (seq_t *)realloc(pow10, need * sizeof(seq_t));
    if (pow10_len == 0) {
        pow10[0] = (seq_t)1;
        pow10_len = 1;
    }

    for (size_t i = pow10_len; i < need; ++i) {
        /* pow10[i] = pow10[i-1] * 10 */
        pow10[i] = pow10[i-1] * (seq_t)10;
    }
    pow10_len = need;
}

seq_t a(idx_t n)
{
    ensure_pow10(n);
    seq_t ten_n = pow10[(size_t)n];          /* 10^n as seq_t */
    seq_t nseq   = (seq_t)n;                 /* n as seq_t */

    /* For builtin scalar seq_t types (<=64 bits) use 128‑bit intermediates
       to avoid overflow when n*10^n is large.  For big‑integer seq_t types
       the built‑in arithmetic is used directly. */
    if (sizeof(seq_t) <= 8) {
        /* 128‑bit intermediate arithmetic */
        unsigned __int128 n128   = (unsigned __int128)n;
        unsigned __int128 t128   = (unsigned __int128)ten_n;

        unsigned __int128 part1 = (unsigned __int128)360 * n128;
        unsigned __int128 part2 = (unsigned __int128)405 * n128 * t128;
        unsigned __int128 part3 = (unsigned __int128)220 * t128;

        unsigned __int128 numerator = part1 + part2 - part3 + (unsigned __int128)220;
        return (seq_t)(numerator / (unsigned __int128)162);
    }
    else {
        /* High‑precision seq_t type – use its natural arithmetic */
        seq_t part1 = (seq_t)360 * nseq;
        seq_t part2 = (seq_t)405 * (nseq * ten_n);
        seq_t part3 = (seq_t)220 * ten_n;

        seq_t numerator = part1 + part2 - part3 + (seq_t)220;
        return numerator / (seq_t)162;
    }
}
