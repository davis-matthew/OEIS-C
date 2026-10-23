/*
 * A159397  Number of n X n arrays of squares of integers summing to 16.
 * https://oeis.org/A159397
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   Number of n×n arrays of squares of integers summing to 16
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 10, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*   Count all 4‑tuples (c1,c4,c9,c16) with
        c1 + 4·c4 + 9·c9 + 16·c16 = 16   (cᵢ ≥ 0)
    For each such tuple the number of arrays equals the multinomial
        (n²)! / (c0! c1! c4! c9! c16!)
    where c0 = n² – (c1+c4+c9+c16).  The multinomial is computed
    as a product of binomial coefficients.
    A small table of all 8 possible tuples is hard‑coded.  The
    binomial routine does not overflow 64‑bit values for any n
    for which a(n) fits in uint64_t.  For larger n it may overflow
    but those values are not required to be correct in the 64‑bit build. */

#include <stddef.h>
#include <stdint.h>

/* ------------------------------------------------------------ */
/*   Helper: gcd for unsigned long long (works for 64‑bit)     */
static inline unsigned long long _gcd_uint64(unsigned long long a,
                                            unsigned long long b)
{
    while (b)
    {
        unsigned long long t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* ------------------------------------------------------------ */
/*   Compute C(N,k) for k ≤ 16 without overflow (when the result
    fits in seq_t).  The routine cancels all denominator factors
    with numerators before doing the multiplications.           */
static inline seq_t binom_small(idx_t N, idx_t k)
{
    if (k > N)
        return (seq_t)0;
    /* use symmetry to keep k small */
    if (k > N - k)
        k = N - k;
    /* k is at most 16 (because the equation uses only
       the four non‑zero squares).  A small static array is fine */
    unsigned long long numer[16];
    unsigned long long denom[16];
    for (idx_t i = 0; i < k; ++i)
    {
        numer[i] = N - k + 1 + i;
        denom[i] = i + 1;
    }
    /* cancel factors */
    for (idx_t i = 0; i < k; ++i)
    {
        if (denom[i] == 1)
            continue;
        for (idx_t j = 0; j < k; ++j)
        {
            if (numer[j] == 1)
                continue;
            unsigned long long g = _gcd_uint64(numer[j], denom[i]);
            if (g > 1)
            {
                numer[j] /= g;
                denom[i] /= g;
                if (denom[i] == 1)
                    break;
            }
        }
    }
    /* after cancellation denom[i] should be 1; multiply numerators */
    seq_t res = (seq_t)1;
    for (idx_t i = 0; i < k; ++i)
        res *= (seq_t)numer[i];
    return res;
}

/* ------------------------------------------------------------ */
/*   Solution structure representing counts of cells with
    squares 1², 2², 3², and 4² respectively.  Count of 0² cells
    is implicit.                                         */
typedef struct {
    uint8_t c1;   /* #cells holding 1² = 1          */
    uint8_t c4;   /* #cells holding 2² = 4          */
    uint8_t c9;   /* #cells holding 3² = 9          */
    uint8_t c16;  /* #cells holding 4² = 16         */
} sol_t;

/* All solutions to 1·c1 + 4·c4 + 9·c9 + 16·c16 = 16.          */
static const sol_t SOLUTIONS[8] = {
    {16, 0, 0, 0},  /* 16 1‑s  */
    {12, 1, 0, 0},  /* 12 1‑s, 1 4‑s */
    { 8, 2, 0, 0},  /* 8 1‑s, 2 4‑s */
    { 4, 3, 0, 0},  /* 4 1‑s, 3 4‑s */
    { 0, 4, 0, 0},  /* 4 4‑s      */
    { 7, 0, 1, 0},  /* 7 1‑s, 1 9‑s   */
    { 3, 1, 1, 0},  /* 3 1‑s, 1 4‑s, 1 9‑s */
    { 0, 0, 0, 1}   /* 1 16‑s       */
};

/* ------------------------------------------------------------ */
/*   External function required by the framework.              */
seq_t a(idx_t n)
{
    if (n < 2)
        return (seq_t)0;   /* undefined for n < 2 */

    idx_t N = n * n;          /* total number of cells      */
    seq_t total = (seq_t)0;

    for (size_t s = 0; s < sizeof(SOLUTIONS) / sizeof(SOLUTIONS[0]); ++s)
    {
        const sol_t *sol = &SOLUTIONS[s];

        /* Every solution uses at most 16 cells, so for n ≥ 2
           none are discarded. */
        idx_t used = (idx_t)sol->c1 + sol->c4 + sol->c9 + sol->c16;
        if (used > N)
            continue;   /* safety, never triggered for n ≥ 2 */

        idx_t remaining = N;
        seq_t ways = (seq_t)1;

        /* place c16 cells */
        if (sol->c16)
        {
            ways *= binom_small(remaining, (idx_t)sol->c16);
            remaining -= (idx_t)sol->c16;
        }
        if (!ways)
            continue;

        /* place c9 cells */
        if (sol->c9)
        {
            ways *= binom_small(remaining, (idx_t)sol->c9);
            remaining -= (idx_t)sol->c9;
        }
        if (!ways)
            continue;

        /* place c4 cells */
        if (sol->c4)
        {
            ways *= binom_small(remaining, (idx_t)sol->c4);
            remaining -= (idx_t)sol->c4;
        }
        if (!ways)
            continue;

        /* place c1 cells */
        if (sol->c1)
        {
            ways *= binom_small(remaining, (idx_t)sol->c1);
            remaining -= (idx_t)sol->c1;
        }

        total += ways;
    }
    return total;
}
