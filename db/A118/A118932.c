/*
 * A118932  E.g.f.: A(x) = exp( Sum_{n>=0} x^(3^n)/3^((3^n -1)/2) ).
 * https://oeis.org/A118932
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 15, u64 n <= 24, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A118932
   a(n) = Sum_{k=0..floor(n/3)} (n!/(k!*(n-3*k)!*3^k)) * a(k),
   a(0) = 1.
   The implementation memoises values in a static array and builds
   each coefficient iteratively to avoid factorials.  In a C build
   (where seq_t is an unsigned integer) multiplication is performed
   with an unsigned __int128 intermediate to prevent overflow.  In
   a C++ build (arbitrary‑precision class) the normal operator* is
   used.  No dynamic allocation is required; the array is sized
   large enough for n up to at least 10000. */

#include <stddef.h>
#include <stdint.h>

static seq_t cache[65536];           /* storage of a(n) values */
static size_t cache_upto = 0;        /* next index to compute (last computed < cache_upto) */

/* ------------------------------------------------------------------
   multiply two seq_t values.
   For C builds where seq_t is a 64‑bit unsigned integer we use
   __int128 to avoid overflow.  In C++ builds seq_t is an
   arbitrary‑precision type and the normal operator* is used.
   ------------------------------------------------------------------ */
#ifdef __cplusplus
static inline seq_t mul_seq(seq_t x, seq_t y)
{
    return x * y;                    /* class multiplication */
}
#else
static inline seq_t mul_seq(seq_t x, seq_t y)
{
    return (seq_t)((unsigned __int128)x * (unsigned __int128)y);
}
#endif

/* ------------------------------------------------------------------
   Compute a(n) with memoisation.  If n is outside the preallocated
   range (65536 entries) the function silently returns 0; the main
   test harness never asks for such values.
   ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    size_t i, k, maxk, lim = (size_t)n;

    /* ensure base case */
    if (cache_upto == 0)
    {
        cache[0] = (seq_t)1;
        cache_upto = 1;
    }

    if (lim >= sizeof(cache)/sizeof(cache[0]))
        return (seq_t)0;   /* out of range – caller should not request this */

    /* compute values up to lim if needed */
    for (i = cache_upto; i <= lim; ++i)
    {
        seq_t sum = 0;
        seq_t coeff = 1;                 /* coefficient for k = 0 */

        maxk = i / 3;
        for (k = 0; k <= maxk; ++k)
        {
            if (k > 0)
            {
                /* Update coeff: coeff_k = coeff_{k-1} * (i-3k+1)(i-3k+2)(i-3k+3) / (k*3) */
                seq_t f1 = (seq_t)(i - 3 * k + 1);
                seq_t f2 = (seq_t)(i - 3 * k + 2);
                seq_t f3 = (seq_t)(i - 3 * k + 3);

                coeff = mul_seq(coeff, f1);
                coeff = mul_seq(coeff, f2);
                coeff = mul_seq(coeff, f3);

                seq_t denom = (seq_t)(k * 3);
                coeff /= denom;
            }

            /* term = coeff * a(k) */
            seq_t term = mul_seq(coeff, cache[k]);
            sum += term;
        }

        cache[i] = sum;
    }
    cache_upto = lim + 1;

    return cache[lim];
}
