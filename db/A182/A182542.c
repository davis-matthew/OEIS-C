/*
 * A182542  Second diagonal of triangle in A145879.
 * https://oeis.org/A182542
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4^(n-1) - Catalan(n) * n where Catalan(n) = C(2n,n)/(n+1)
 *            (n >= 3)
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 33, u128 n <= 64,
 *            big any n
 * Validated: standard, pass, 41 terms through n = 190 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Compute 2^exp for any exponent that may not fit in the bit width of seq_t. */
static seq_t pow2(idx_t exp)
{
    seq_t res = (seq_t)1;
    for (idx_t i = 0; i < exp; ++i)
        res *= (seq_t)2;
    return res;
}

static seq_t catalan_n(idx_t n)
{
    seq_t res = 1;
    for (idx_t i = 1; i <= n; ++i) {
        idx_t num = n + i;
        idx_t den = i;
        /* Compute (res * num) / den safely.
           For small seq_t types use a 128‑bit intermediate. */
        if (sizeof(seq_t) <= sizeof(unsigned long long)) {
            __int128 prod = (__int128)res * (__int128)((seq_t)num);
            res = (seq_t)(prod / (seq_t)den);
        } else {
            res = (seq_t)((seq_t)res * (seq_t)num / (seq_t)den);
        }
    }
    /* Divide by (n+1) to get the Catalan number. */
    res = res / (seq_t)(n + 1);
    return res;
}

seq_t a(idx_t n)
{
    if (n < 3)
        return 0; /* values for n < 3 are undefined */

    /* 4^(n-1) = 2^(2*(n-1)) */
    seq_t pow4 = pow2(2 * (n - 1));

    seq_t catalan = catalan_n(n);
    seq_t product = catalan * (seq_t)n;

    return pow4 - product;
}
