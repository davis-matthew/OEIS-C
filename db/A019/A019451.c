/*
 * A019451  Coordination sequence T1 for Zeolite Code CGF.
 * https://oeis.org/A019451
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 177, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   A019451 – Coordination sequence T1 for Zeolite Code CGF
   Computation is based on the rational generating function

      A(x) = (1 + x)
          * (1 + x + 5x² + 4x³ + 9x⁴ + 12x⁵ + 16x⁶ + 17x⁷ + 16x⁸ + 17x⁹
              + 16x¹⁰ + 12x¹¹ + 9x¹² + 4x¹³ + 5x¹⁴ + x¹⁵ + x¹⁶)
          / ((1 - x)³ * (1 - x + x²) * (1 + x²)
             * (1 + x + x² + x³ + x⁴) * (1 + x + x² + x³ + x⁴ + x⁵ + x⁶))

   The denominator expands to
        D(x) = 1 + d1 x + d2 x² + … + d17 x¹⁷,
   with coefficients

        d = {  1,  -2,   3,  -3,   2,  -2,   2,  -4,   5, -5,   4, -2,
               2,  -2,   3, -3,   2,  -1 }.

   Let b[n] be the coefficient of xⁿ in the numerator N(x)= (1+x)P(x)
   (degree 17).  Then the sequence satisfies the linear recurrence

        a(n) = b[n] – Σ_{i=1}^{17} d[i] * a(n-i)    (for n ≥ 0),
        with a(k)=0 for k<0.

   This implementation evaluates the recurrence iteratively up to the
   requested index.  All arithmetic is performed with the user‑defined
   unsigned type seq_t; intermediate sums are cast to 64‑bit signed
   integers, which is safe for the supported range of n.
*/

#include <stdint.h>
#include <stdbool.h>

/* Hidden helper for polynomial multiplication */
static void mul_poly(const int *a, int deg_a, const int *b, int deg_b, int *res)
{
    for (int i = 0; i <= deg_a + deg_b; ++i) res[i] = 0;
    for (int i = 0; i <= deg_a; ++i)
        for (int j = 0; j <= deg_b; ++j)
            res[i + j] += a[i] * b[j];
}

seq_t a(idx_t n)
{
    /* static state: recurrence coefficients and numerator coefficients */
    static bool init_done = false;
    static int d[18];      /* denominator coefficients d[0..17]  (d[0] == 1) */
    static int b[18];      /* numerator coefficients b[0..17]  */

    if (!init_done) {
        /* Compute denominator coefficients D(x) = (1-x)^3 ... (1+x+...+x^6) */
        const int p1[4] = { 1, -3, 3, -1 };          /* (1-x)^3          */
        const int p2[3] = { 1, -1, 1 };              /* (1-x+x^2)        */
        const int p3[3] = { 1, 0, 1 };               /* (1+x^2)          */
        const int p4[5] = { 1, 1, 1, 1, 1 };         /* 1+x+...+x^4      */
        const int p5[7] = { 1, 1, 1, 1, 1, 1, 1 };   /* 1+x+...+x^6      */

        int tmp[18];
        int cur[18];
        int deg_cur = 3;  /* degree of p1 */

        /* start with p1 */
        for (int i = 0; i <= deg_cur; ++i) cur[i] = p1[i];
        for (int i = deg_cur + 1; i < 18; ++i) cur[i] = 0;

        /* multiply by p2 */
        mul_poly(cur, deg_cur, p2, 2, tmp);
        deg_cur += 2;
        for (int i = 0; i <= deg_cur; ++i) cur[i] = tmp[i];

        /* multiply by p3 */
        mul_poly(cur, deg_cur, p3, 2, tmp);
        deg_cur += 2;
        for (int i = 0; i <= deg_cur; ++i) cur[i] = tmp[i];

        /* multiply by p4 */
        mul_poly(cur, deg_cur, p4, 4, tmp);
        deg_cur += 4;
        for (int i = 0; i <= deg_cur; ++i) cur[i] = tmp[i];

        /* multiply by p5 */
        mul_poly(cur, deg_cur, p5, 6, tmp);
        deg_cur += 6;
        for (int i = 0; i <= deg_cur; ++i) d[i] = tmp[i];
        /* d[0] should be 1 */

        /* Numerator: N(x) = (1+x) * P(x) where
           P = 1 + x + 5x² + 4x³ + 9x⁴ + 12x⁵ + 16x⁶ + 17x⁷ + 16x⁸ + 17x⁹
               + 16x¹⁰ + 12x¹¹ + 9x¹² + 4x¹³ + 5x¹⁴ + x¹⁵ + x¹⁶
        */
        const int P[17] = { 1, 1, 5, 4, 9, 12, 16, 17, 16, 17,
                            16, 12, 9, 4, 5, 1, 1 };

        /* b[0] */
        b[0] = P[0];
        for (int i = 1; i <= 16; ++i) b[i] = P[i] + P[i - 1];
        b[17] = P[16];  /* last term of (1+x)*P */

        init_done = true;
    }

    /* several helpful constants */
    const int K = 17;               /* order of recurrence */
    const int MOD = K + 1;          /* circular buffer size */

    /* Ring buffer for the last K values of a(n) */
    static seq_t buf[18];
    static bool buf_initialized = false;
    if (!buf_initialized) {
        for (int i = 0; i < MOD; ++i) buf[i] = 0;
        buf_initialized = true;
    }

    /* Compute a(n) iteratively up to the requested index */
    for (idx_t k = 0; k <= n; ++k) {
        long long sum = 0;
        for (int i = 1; i <= K && i <= k; ++i) {
            /* value from buffer */
            seq_t val = buf[(int)((k - i) % MOD)];
            /* convert to 64‑bit for intermediate sum */
            uint64_t uval = (uint64_t)val;
            sum += (long long)d[i] * (long long)uval;
        }
        uint64_t bn = (k <= 17) ? (uint64_t)b[k] : 0ULL;
        long long res = (long long)bn - sum;
        /* store result */
        buf[(int)(k % MOD)] = (seq_t)(unsigned long long)res;
    }

    return buf[(int)(n % MOD)];
}
