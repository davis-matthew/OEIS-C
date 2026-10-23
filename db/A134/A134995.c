/*
 * A134995  a(n) = floor(3n/2log(n+1)).
 * https://oeis.org/A134995
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(3*n/(2*log(n+1)))
 * Range:     u8 n <= 79, u16 n <= 79, u32 n <= 79, u64 n <= 79, u128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Fixed–point scaling factor: 2^60 gives ~18 decimal digits of precision. */
static const unsigned __int128 SCALE = (unsigned __int128)1 << 60;
static unsigned __int128 ln2_fixed = 0;

/* Compute ln(2) in the same fixed–point representation. */
static void compute_ln2_fixed(void)
{
    if (ln2_fixed) return;          /* already computed */
    const unsigned __int128 one = SCALE;

    /* y = (2-1)/(2+1) = 1/3 */
    unsigned __int128 y = one / 3;
    unsigned __int128 y2 = (y * y) / SCALE;

    unsigned __int128 term = y;     /* first term y */
    unsigned __int128 sum  = term;  /* sum of series terms */
    int power = 1;

    while (1) {
        /* term for next odd exponent: term *= y^2 / SCALE */
        term = (term * y2) / SCALE;
        if (term == 0) break;
        power += 2;
        sum += term / power;
    }
    ln2_fixed = sum * 2;            /* ln(2) = 2 * sum */
}

/* Compute ln(x)*SCALE for integer x >= 1 using fixed‑point arithmetic. */
static unsigned __int128 ln_fixed_u128(uint64_t x)
{
    if (x <= 1) return 0;
    compute_ln2_fixed();

    /* Determine k = floor(log2(x)) and u = x/2^k with 1 <= u < 2 */
    int k = 0;
    uint64_t temp = x;
    while (temp > 1) {
        temp >>= 1;
        ++k;
    }
    unsigned __int128 u_fixed = ((unsigned __int128)x * SCALE) >> k;

    /* Compute ln(u) using the series ln(u) = 2 * ( y + y^3/3 + y^5/5 + … )
       where y = (u-1)/(u+1). */
    unsigned __int128 y = ((u_fixed - SCALE) * SCALE) / (u_fixed + SCALE);
    unsigned __int128 y2 = (y * y) / SCALE;

    unsigned __int128 term = y;          /* y^1 */
    unsigned __int128 sum  = term;       /* accumulate series terms */
    int power = 1;

    while (1) {
        /* Next odd power: term = term * y^2 / SCALE = y^(current+2) */
        term = (term * y2) / SCALE;
        if (term == 0) break;
        power += 2;
        sum += term / power;
    }

    unsigned __int128 ln_u_fixed = sum * 2;          /* ln(u) */
    unsigned __int128 ln_fixed    = ln2_fixed * k;   /* k * ln(2)   */
    ln_fixed += ln_u_fixed;
    return ln_fixed;
}

/* OEIS A134995: a(n) = floor(3*n/(2*log(n+1))) */
seq_t a(idx_t n)
{
    if (n < 2) return 0;                 /* behaviour below offset irrelevant */

    unsigned __int128 ln_val   = ln_fixed_u128(n + 1);           /* ln(n+1)*SCALE */
    unsigned __int128 numerator   = (unsigned __int128)3 * n * SCALE;   /* 3*n*SCALE  */
    unsigned __int128 denominator = (unsigned __int128)2 * ln_val;      /* 2*ln(n+1)*SCALE */

    unsigned __int128 result = numerator / denominator;            /* floor division */
    return (seq_t)result;
}
