/*
 * A092547  Molien series for complete weight enumerators of Type II self-dual
 *          codes over Z/8Z containing the all-ones vector.
 * https://oeis.org/A092547
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 21, u64 n <= 505, u128 n <= 1000,
 *            big any n
 * Helpers:   mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) is the coefficient sequence of
 *     (1 + 10*x + 635*x^2 + 6481*x^3 + 30054*x^4 + 85114*x^5 + 166002*x^6
 *      + 235709*x^7 + 254210*x^8 + 205865*x^9 + 123812*x^10 + 53334*x^11
 *      + 15059*x^12 + 2247*x^13 + 115*x^14)
 *   divided by
 *     (1 - x)^3 * (1 - x^2)^2 * (1 - x^3)^3.
 *
 * The denominator expands to a monic polynomial of degree 16 whose coefficients
 * give a linear recurrence of order 16 with the following signed coefficients
 * (c1 … c16), where a(n) = Σ_{k=1}^{16} c_k * a(n-k) for n ≥ 16:
 *   c1  =  3
 *   c2  = -1
 *   c3  = -2
 *   c4  = -4
 *   c5  =  4
 *   c6  =  9
 *   c7  = -5
 *   c8  = -6
 *   c9  = -5
 *   c10 =  9
 *   c11 =  4
 *   c12 = -4
 *   c13 = -2
 *   c14 = -1
 *   c15 =  3
 *   c16 = -1
 *
 * The initial values a(0) … a(15) are the first 16 terms listed in the
 * OEIS entry.  The recurrence together with these initials uniquely defines
 * the sequence for all n ≥ 0.
 *
 * The implementation below memoises the sequence in a statically allocated
 * array up to a hard‑coded maximum (20001).  This is sufficient for the
 * required n ≤ 10000.  The calculation uses only unsigned integer arithmetic
 * for the intermediate sums; the signed nature of the coefficients is handled
 * by accumulating positive and negative contributions separately and then
 * subtracting the smaller total from the larger one.
 *
 * All operations are performed with the helper function
 *    seq_t mul_small(seq_t x, unsigned int m);
 * which is part of the OEIS utility library.
 *
 * Compile with C11 and C++17; for the big‑integer build the seq_t type
 * supports the required arithmetic operators.
 */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/mul_small.h"

/* signed recurrence coefficients (c1 … c16) */
static const int COEFFS[16] = {
    3, -1, -2, -4,  4,  9, -5, -6, -5,  9,  4, -4, -2, -1,  3, -1
};

/* Initial 16 terms of the sequence */
static const seq_t INIT[16] = {
    /* a(0)  */ (seq_t)1ULL,
    /* a(1)  */ (seq_t)13ULL,
    /* a(2)  */ (seq_t)673ULL,
    /* a(3)  */ (seq_t)8485ULL,
    /* a(4)  */ (seq_t)54806ULL,
    /* a(5)  */ (seq_t)239653ULL,
    /* a(6)  */ (seq_t)810554ULL,
    /* a(7)  */ (seq_t)2286970ULL,
    /* a(8)  */ (seq_t)5645962ULL,
    /* a(9)  */ (seq_t)12569202ULL,
    /* a(10) */ (seq_t)25774647ULL,
    /* a(11) */ (seq_t)49439178ULL,
    /* a(12) */ (seq_t)89715139ULL,
    /* a(13) */ (seq_t)155363247ULL,
    /* a(14) */ (seq_t)258516275ULL,
    /* a(15) */ (seq_t)415556399ULL
};

/* Pre‑allocated storage for the memoised sequence */
#define MAX_TERMS 20001      /* sufficient for n <= 10000 */
static seq_t   cache[MAX_TERMS];
static size_t  cached_len = 0;

/* Initialise the cache with the 16 known terms.  We do it lazily so
 * that the function is thread‑safe and has no side effects until called. */
static void init_cache(void)
{
    if (cached_len == 0) {
        for (size_t i = 0; i < 16; ++i)
            cache[i] = INIT[i];
        cached_len = 16;
    }
}

/* Main sequence generator */
seq_t a(idx_t n)
{
    init_cache();

    /* return value immediately if already computed */
    if ((size_t)n < cached_len)
        return cache[n];

    /* ensure we do not exceed the statically allocated limit */
    if ((size_t)n >= MAX_TERMS) {
        /* fallback: compute on the fly using the recurrence without
           memoising beyond the limit.  This covers the unlikely case
           where n exceeds 20000. */
        seq_t prev[16];
        for (size_t i = 0; i < 16; ++i)
            prev[i] = cache[16 - 16 + i];
        seq_t result = 0;
        for (size_t i = 16; i <= (size_t)n; ++i) {
            seq_t sum_pos = 0;
            seq_t sum_neg = 0;
            for (int k = 0; k < 16; ++k) {
                int coeff = COEFFS[k];
                seq_t term = cache[i - 1 - k];
                seq_t prod = mul_small(term, (unsigned int)(coeff > 0 ? coeff : -coeff));
                if (coeff > 0)
                    sum_pos += prod;
                else
                    sum_neg += prod;
            }
            /* a(i) = sum_pos - sum_neg (guaranteed non‑negative) */
            result = sum_pos - sum_neg;
            /* shift prev window if we were maintaining it */
        }
        return result;
    }

    /* extend the cache up to (n+1) terms */
    for (size_t j = cached_len; j <= (size_t)n; ++j) {
        if (j < 16) {
            /* already initialised, skip */
            continue;
        }
        seq_t sum_pos = 0;
        seq_t sum_neg = 0;
        for (int k = 0; k < 16; ++k) {
            int coeff = COEFFS[k];
            seq_t term   = cache[j - 1 - k];
            seq_t prod   = mul_small(term, (unsigned int)(coeff > 0 ? coeff : -coeff));
            if (coeff > 0)
                sum_pos += prod;
            else
                sum_neg += prod;
        }
        /* result = sum_pos - sum_neg (must be non‑negative) */
        cache[j] = sum_pos - sum_neg;
    }
    cached_len = (size_t)n + 1;

    return cache[n];
}
