/*
 * A046169  Number of minimal covers on n objects with 9 members.
 * https://oeis.org/A046169
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, changed
 * Formula:   a(n) = (1/9!) * (511^n - 9*510^n + 36*509^n - 84*508^n +
 *            126*507^n - 126*506^n + 84*505^n - 36*504^n + 9*503^n - 502^n)
 * Range:     u8 n <= 9, u16 n <= 10, u32 n <= 11, u64 n <= 14, u128 n <= 19,
 *            big any n
 * Helpers:   big_pow, mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Helpers for arbitrary‑precision builds */
#include "../utils/big_pow.h"
#include "../utils/mul_small.h"

/* Small integer helper for 128‑bit intermediate arithmetic */
static unsigned __int128 pow128_u128(unsigned __int128 base, idx_t exp)
{
    unsigned __int128 result = 1;
    while (exp)
    {
        if (exp & 1U) result *= base;
        base *= base;
        exp >>= 1U;
    }
    return result;
}

/* Coefficients and bases (descending) */
static const long coeff[10] = { 1L, -9L, 36L, -84L, 126L,
                                -126L, 84L, -36L, 9L, -1L };
static const unsigned long long base[10] = {
    511ULL, 510ULL, 509ULL, 508ULL, 507ULL,
    506ULL, 505ULL, 504ULL, 503ULL, 502ULL
};

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;          /* Offset: a(1) is the first term */

    /* ---------- Small seq_t (≤ 8 bytes) ---------- */
    if (sizeof(seq_t) <= sizeof(unsigned long long))
    {
        const unsigned __int128 fact9 = 362880ULL; /* 9! */
        unsigned __int128 sum = 0;

        for (idx_t i = 0; i < 10; ++i)
        {
            unsigned __int128 pow_val = pow128_u128((unsigned __int128)base[i], n);
            unsigned __int128 mag = (coeff[i] >= 0 ? (unsigned __int128)coeff[i]
                                                   : (unsigned __int128)(-coeff[i]));
            unsigned __int128 term = pow_val * mag;

            if (coeff[i] >= 0) sum += term;
            else              sum -= term;
        }

        unsigned __int128 result128 = sum / fact9;
        return (seq_t)result128;
    }

    /* ---------- Arbitrary‑precision seq_t ---------- */
    /* Use big_pow and mul_small (and the unsigned big‑int type) */
    seq_t pos_sum = (seq_t)0;
    seq_t neg_sum = (seq_t)0;
    const seq_t fact9 = (seq_t)362880ULL;

    for (idx_t i = 0; i < 10; ++i)
    {
        seq_t pow_val = big_pow((seq_t)base[i], n);   /* (base[i])^n */
        unsigned int mag = (coeff[i] >= 0 ? (unsigned int)coeff[i]
                                      : (unsigned int)(-coeff[i]));     /* magnitude */
        seq_t term = mul_small(pow_val, mag);         /* positive*count */

        if (coeff[i] >= 0) pos_sum += term;
        else              neg_sum += term;
    }

    /* final result = (pos_sum - neg_sum) / 9! */
    seq_t diff = pos_sum;
    diff -= neg_sum;           /* pos_sum ≥ neg_sum for this formula          */
    return diff / fact9;       /* division exact for all n                     */
}
