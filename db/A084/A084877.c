/*
 * A084877  Number of (k,m,n)-antichains of multisets with k=3 and m=5.
 * https://oeis.org/A084877
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1/120 * (243^n - 20*162^n + 60*126^n + 20*108^n + 10*98^n
 *            - 120*93^n - 120*84^n + 10*81^n + 30*78^n + 120*77^n + 120*70^n
 *            - 120*63^n + 20*56^n - 120*54^n + 240*42^n + 40*36^n - 240*31^n
 *            + 35*27^n + 60*26^n - 210*18^n + 210*14^n + 50*9^n - 100*6^n +
 *            24*3^n) (see A084877)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 9, u128 n <= 16,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/big_pow.h"   /* Provides seq_t big_pow(seq_t base, seq_t exp) */

static const unsigned long long bases[] = {
    243ULL, 162ULL, 126ULL, 108ULL, 98ULL,
    93ULL, 84ULL, 81ULL, 78ULL, 77ULL,
    70ULL, 63ULL, 56ULL, 54ULL, 42ULL,
    36ULL, 31ULL, 27ULL, 26ULL, 18ULL,
    14ULL, 9ULL, 6ULL, 3ULL
};
static const int coeffs[] = {
     1, -20,  60,  20,  10,
   -120, -120,  10,  30, 120,
   120, -120,  20, -120, 240,
    40, -240,  35,  60, -210,
   210,  50, -100,  24
};
#define NUM_TERMS (sizeof(bases)/sizeof(bases[0]))

/* Helper: unsigned __int128 exponentiation by squaring (used for 64‑bit path) */
static unsigned __int128 pow128(unsigned long long base, idx_t exp)
{
    unsigned __int128 result = 1;
    unsigned __int128 cur    = base;
    while (exp) {
        if (exp & 1) result *= cur;
        cur *= cur;
        exp >>= 1;
    }
    return result;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /*-----------  64‑bit build path  -------------------------------------*/
    if (sizeof(seq_t) == sizeof(unsigned long long)) {
        signed __int128 total = 0;
        idx_t i;
        for (i = 0; i < NUM_TERMS; ++i) {
            unsigned __int128 pwr = pow128(bases[i], n);
            int coef = coeffs[i];
            if (coef >= 0)
                total += (signed __int128)coef * (signed __int128)pwr;
            else
                total -= (signed __int128)(-coef) * (signed __int128)pwr;
        }
        /* exact division by 120 */
        seq_t result = (seq_t)(total / 120);
        return result;
    }

    /*-----------  arbitrary‑precision build path  -------------------------*/
    seq_t pos = 0;   /* sum of positive terms */
    seq_t neg = 0;   /* sum of negative terms */
    idx_t i;
    for (i = 0; i < NUM_TERMS; ++i) {
        seq_t term = big_pow((seq_t)bases[i], (seq_t)n);
        int coef = coeffs[i];
        if (coef >= 0) {
            pos += (seq_t)coef * term;
        } else {
            neg += (seq_t)(-coef) * term;
        }
    }
    seq_t numerator = pos;
    numerator -= neg;          /* numerator is always >= 0 */
    seq_t result = numerator / 120;   /* exact division */
    return result;
}
