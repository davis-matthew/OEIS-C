/*
 * A008034  Coordination sequence T1 for Zeolite Code APD.
 * https://oeis.org/A008034
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 169, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A008034 – Coordination sequence T1 for Zeolite Code APD
    Recurrence derived from the generating function
        f(x) = P(x) / Q(x)
    where Q(x) = (x^5-x^4+x-1)(x^7-1)(x^13+x^12+x^11-x^2-x-1)
    and P(x) is the polynomial shown in the OEIS entry.
    The coefficients of Q(x) (in ascending order of exponents) are
        q[0] = -1,
        q[1] =  0,  q[2] =  0,  q[3] =  1,  q[4] = -1,
        q[5] =  0,  q[6] =  0,  q[7] =  2,  q[8] =  0,
        q[9] =  0,  q[10]= -1, q[11]=  2,  q[12]=  0,
        q[13]=  0,  q[14]= -2, q[15]=  1,  q[16]=  0,
        q[17]=  0,  q[18]= -2, q[19]=  0,  q[20]=  0,
        q[21]=  1,  q[22]= -1, q[23]=  0,  q[24]=  0,
        q[25]=  1                (degree 25)
    The recurrence is
        a(n) = -p_n + Σ_{i=1}^{25} q[i] * a(n-i),
    where p_n are the coefficients of the numerator P(x) (ascending).
    The numerator’s coefficients are
        p[0] = -1,  p[1] = -4,  p[2] = -10,  p[3] = -20,
        p[4] = -34, p[5] = -51, p[6] = -71,  p[7] = -94,
        p[8] = -117,p[9] = -139,p[10]= -157, p[11]= -170,
        p[12]= -177,p[13]= -178,p[14]= -172, p[15]= -159,
        p[16]= -142,p[17]= -121,p[18]= -98,  p[19]= -75,
        p[20]= -53, p[21]= -34, p[22]= -20,  p[23]= -9,
        p[24]= -2,  p[25]=  1,  p[26]=  2,  p[27]=  1   (zero for n>27)
    Initial values a(0)…a(23) are provided in the OEIS table.
*/

#include <stdlib.h>

static const int q[26] = { -1, 0, 0, 1, -1, 0, 0, 2, 0, 0, -1, 2, 0, 0, -2, 1, 0, 0, -2, 0, 0, 1, -1, 0, 0, 1 };
static const int p[28] = { -1, -4, -10, -20, -34, -51, -71, -94, -117, -139, -157, -170, -177, -178, -172, -159, -142, -121, -98, -75, -53, -34, -20, -9, -2, 1, 2, 1 };

/* Memoised recurrence evaluation */
seq_t a(idx_t n)
{
    /* Static cache for memoised values */
    static seq_t *cache = 0;
    static size_t cache_cap = 0;
    static idx_t cache_len = 0;   /* number of elements stored (next free index) */

    if (n < cache_len)
        return cache[n];

    /* Ensure capacity */
    size_t required = (size_t)n + 1;
    if (required > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 64;
        if (new_cap < required) new_cap = required;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();  /* out of memory – terminate program */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute terms from cache_len up to n */
    for (idx_t k = cache_len; k <= n; ++k) {
        /* Signed 128‑bit intermediate for safety (fits all 64‑bit values) */
        __int128 sum = 0;

        /* Add contributions q[i] * a(k-i) for 1 <= i <= min(k,25) */
        for (idx_t i = 1; i <= k && i <= 25; ++i) {
            /* Convert a(k-i) to signed 128 for multiplication */
            sum += (__int128)q[i] * (__int128)cache[k - i];
        }

        /* Add (-p_k) term; p[k] is zero for k>27 */
        if (k <= 27)
            sum += -(__int128)p[k];

        /* Store result as seq_t (value is non‑negative) */
        cache[k] = (seq_t)sum;
    }

    cache_len = n + 1;
    return cache[n];
}
