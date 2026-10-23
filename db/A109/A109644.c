/*
 * A109644  {Sum of all k-digit numbers > n }-{sum of all k-digit numbers <
 *          n}, n is a 'k'digit number.
 * https://oeis.org/A109644
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, sign
 * Formula:   a(n) = Σk‑digit numbers > n – Σk‑digit numbers < n, with k =
 *            ⌊log10(n)⌋ + 1. The function uses 128‑bit arithmetic for safety
 *            and casts the final result to seq_t. It works for any seq_t that
 *            may be 8, 16, 32, 64 or 128 bits (or larger) and for any idx_t
 *            (here uint64_t)
 * Range:     i8 n <= 9, i16 n <= 99, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static __uint128_t pow10[21];   /* 10^0 .. 10^20 (10^20 fits in 128‑bit) */
static bool pow10_initialized = false;

/* Initialise the powers of 10 only once. */
static void init_pow10(void)
{
    if (pow10_initialized) return;
    pow10[0] = 1;
    for (int i = 1; i <= 20; ++i)
        pow10[i] = pow10[i-1] * 10ULL;
    pow10_initialized = true;
}

seq_t a(idx_t n)
{
    /* n = 0 is outside the OEIS offset, return 0 safely. */
    if (n == 0) return (seq_t)0;

    init_pow10();

    /* Find the digit count k and the bounds low = 10^(k‑1), high = 10^k ‑ 1. */
    unsigned int m = 0;
    __uint128_t nn = ( __uint128_t ) n;
    while (m + 1 <= 20 && nn >= pow10[m+1])
        ++m;

    __uint128_t low  = pow10[m];
    __uint128_t high = pow10[m+1] - 1;   /* fits in 128‑bit even for 20‑digit numbers */

    __uint128_t count_low  = nn - low;          /* numbers < n */
    __uint128_t count_high = high - nn;        /* numbers > n */

    __uint128_t sum_low  = 0;
    __uint128_t sum_high = 0;

    if (count_low > 0)
    {
        /* sum_{i=low}^{n-1} i = (low + (n-1)) * count_low / 2 */
        __uint128_t tmp = (low + (nn - 1)) * count_low;
        sum_low = tmp / 2;
    }

    if (count_high > 0)
    {
        /* sum_{i=n+1}^{high} i = (n+1 + high) * count_high / 2 */
        __uint128_t tmp = ((nn + 1) + high) * count_high;
        sum_high = tmp / 2;
    }

    __int128_t diff = ( __int128_t )sum_high - ( __int128_t )sum_low;

    return (seq_t)diff;
}
