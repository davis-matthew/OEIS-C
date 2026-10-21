/*
 * A084882  Number of (k,m,n)-multiantichains of multisets with k=3 and m=5.
 * https://oeis.org/A084882
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (243^n - 20*162^n + 60*126^n + 20*108^n + 10*98^n -
 *            120*93^n - 120*84^n + 30*81^n + 30*78^n + 120*77^n + 120*70^n -
 *            120*63^n + 20*56^n - 360*54^n + 720*42^n + 120*36^n - 720*31^n +
 *            275*27^n + 180*26^n - 1650*18^n + 1650*14^n + 870*9^n - 1740*6^n
 *            + 744*3^n) / 120
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 9, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <inttypes.h>

static const int64_t bases[24] = {
    243, 162, 126, 108, 98, 93, 84, 81, 78, 77,
    70, 63, 56, 54, 42, 36, 31, 27, 26, 18,
    14, 9, 6, 3
};

static const int64_t coeffs[24] = {
     1, -20, 60, 20, 10, -120, -120, 30, 30, 120,
   120, -120, 20, -360, 720, 120, -720, 275, 180, -1650,
   1650, 870, -1740, 744
};

/* Compute base^exp using unsigned __int128 (only safe for small exp). */
static unsigned __int128 pow_uint128(unsigned long long base, idx_t exp)
{
    unsigned __int128 result = 1;
    unsigned __int128 cur = base;
    while (exp)
    {
        if (exp & 1) result *= cur;
        cur *= cur;
        exp >>= 1;
    }
    return result;
}

/* Compute base^exp using seq_t itself (arbitrary‑precision). */
static seq_t pow_seq_t(unsigned long long base, idx_t exp)
{
    seq_t result = 1;
    seq_t b = base;
    for (idx_t i = 0; i < exp; ++i)
        result *= b;
    return result;
}

seq_t a(idx_t n)
{
    /* Handle 64‑bit or smaller seq_t using 128‑bit intermediates. */
    if (sizeof(seq_t) <= 8)
    {
        __int128 sum = 0;
        for (int i = 0; i < 24; ++i)
        {
            unsigned __int128 p = pow_uint128((unsigned long long)bases[i], n);
            __int128 term = (__int128)coeffs[i] * (__int128)p;
            sum += term;
        }
        /* sum is guaranteed divisible by 120 */
        seq_t res = (seq_t)(sum / 120);
        return res;
    }

    /* For 128‑bit or arbitrary‑precision seq_t, compute directly with seq_t. */
    seq_t sum = 0;
    for (int i = 0; i < 24; ++i)
    {
        seq_t p = pow_seq_t((unsigned long long)bases[i], n);
        seq_t term = (seq_t)coeffs[i] * p;
        sum += term;
    }
    seq_t res = sum / 120;
    return res;
}
