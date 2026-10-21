/*
 * A055313  Number of labeled rooted trees with n nodes and 12 leaves.
 * https://oeis.org/A055313
 *
 * Offset:    13    (a(13) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! *
 *            (n-12)^2*(n-11)^2*(n-10)*(n-9)*(n-8)*(n-7)*(n-6)*(n-5)*(n-4)*(n-3)*(n-2)*(n-1)*(9*n^8
 *            - 708*n^7 + 24018*n^6 - 458136*n^5 + 5363513*n^4 - 39369364*n^3
 *            + 176423596*n^2 - 439700816*n + 464486400) /
 *            352423730384732160000
 * Range:     u8 n <= 13, u16 n <= 13, u32 n <= 15, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/factorial.h"

seq_t a(idx_t n)
{
    if (n < 13) return 0;

    /* --------------------------------------------------
       Small seq_t types (≤ 64 bits): use 128‑bit
       intermediate arithmetic to avoid overflow.
    -------------------------------------------------- */
    if (sizeof(seq_t) <= 8) {
        /* Build the huge constant once, in 128‑bit */
        static unsigned __int128 denom = 0;
        static int denom_inited = 0;
        if (!denom_inited) {
            const char *s = "352423730384732160000";
            for (const char *p = s; *p; ++p)
                denom = denom * 10 + (*p - '0');
            denom_inited = 1;
        }

        unsigned __int128 n128 = n;

        /* factorial */
        unsigned __int128 fact = 1;
        for (idx_t i = 2; i <= n; ++i) fact *= i;

        /* product of the linear factors */
        unsigned __int128 prod = 1;
        for (idx_t k = 12; k >= 1; --k) {
            unsigned __int128 f = n128 - k;
            if (k == 12 || k == 11) prod *= f * f;
            else prod *= f;
        }

        /* polynomial by Horner, signed to avoid underflow */
        signed __int128 poly128 = 9;
        poly128 = poly128 * n128 - 708;
        poly128 = poly128 * n128 + 24018;
        poly128 = poly128 * n128 - 458136;
        poly128 = poly128 * n128 + 5363513;
        poly128 = poly128 * n128 - 39369364;
        poly128 = poly128 * n128 + 176423596;
        poly128 = poly128 * n128 - 439700816;
        poly128 = poly128 * n128 + 464486400;

        unsigned __int128 poly = (unsigned __int128)poly128;

        /* numerator and division */
        unsigned __int128 numer = fact * prod;
        numer *= poly;
        unsigned __int128 result128 = numer / denom;
        return (seq_t)result128;
    }

    /* --------------------------------------------------
       seq_t is ≥ 128 bits or an arbitrary‑precision type.
    -------------------------------------------------- */
    /* Build the denominator once, using seq_t arithmetic */
    static seq_t denom = 0;
    static bool denom_inited = false;
    if (!denom_inited) {
        const char *s = "352423730384732160000";
        denom = 0;
        for (const char *p = s; *p; ++p) denom = denom * 10 + (*p - '0');
        denom_inited = true;
    }

    seq_t nseq = (seq_t)n;
    seq_t fact = factorial(n);

    /* product of the linear factors */
    seq_t prod = 1;
    for (idx_t k = 12; k >= 1; --k) {
        seq_t f = nseq - k;
        if (k == 12 || k == 11) prod = prod * f * f;
        else prod = prod * f;
    }

    /* polynomial (using signed 128‑bit intermediate) */
    signed __int128 n128 = n;
    signed __int128 poly128 = 9;
    poly128 = poly128 * n128 - 708;
    poly128 = poly128 * n128 + 24018;
    poly128 = poly128 * n128 - 458136;
    poly128 = poly128 * n128 + 5363513;
    poly128 = poly128 * n128 - 39369364;
    poly128 = poly128 * n128 + 176423596;
    poly128 = poly128 * n128 - 439700816;
    poly128 = poly128 * n128 + 464486400;

    seq_t poly = (seq_t)poly128;

    /* numerator and division */
    seq_t numer = fact * prod;
    numer = numer * poly;
    seq_t result = numer / denom;
    return result;
}
