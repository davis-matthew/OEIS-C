/*
 * A162013  The sequence of the absolute values of the a(n-3) coefficients of
 *          A162011.
 * https://oeis.org/A162013
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) =
 *            (280*n^12+1680*n^11-252*n^10-16660*n^9-13758*n^8+63408*n^7+68705*n^6-104265*n^5-111657*n^4+66997*n^3+56682*n^2-11160*n)/45360
 *            Recurrence: a(n) = 13*a(n-1) - 78*a(n-2) + 286*a(n-3) -
 *            715*a(n-4) + 1287*a(n-5) - 1716*a(n-6) + 1716*a(n-7) -
 *            1287*a(n-8) + 715*a(n-9) - 286*a(n-10) + 78*a(n-11) -...
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 9, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* Coefficients of the recurrence.  Sign alternates starting with positive. */
static const int coeff[13] = {
    13, 78, 286, 715, 1287, 1716, 1716,
    1287, 715, 286, 78, 13, 1
};

/* Forward declaration of a helper that evaluates the polynomial for
 * the first 13 terms.  It uses __int128 (a common GCC/Clang extension)
 * so that we do not rely on 64‑bit overflow. */
static seq_t compute_base(idx_t n);

/* Cache of already computed terms.  Static for memoisation. */
static seq_t *cache = NULL;
static idx_t capacity = 0;
static idx_t computed = 0;

/* Initialise the cache with the first 13 terms evaluated by the
 * polynomial formula. */
static void init_cache(void)
{
    /* Allocate a bit more than needed so we can grow efficiently. */
    capacity = 32;
    cache = (seq_t *)malloc(capacity * sizeof(seq_t));
    if (!cache) return;            /* ignore failure, caller will crash */
    cache[0] = 0;                  /* unused, to keep indices simple */
    for (idx_t i = 1; i <= 13; ++i)
        cache[i] = compute_base(i);
    computed = 13;
}

/* Evaluate the polynomial expression for the initial terms.  The
 * calculation is performed in signed __int128 so that intermediate
 * values do not overflow until the final division. */
static seq_t compute_base(idx_t n)
{
    __int128 sn = (__int128)n;
    __int128 res = (__int128)280;           /* leading coefficient */

    res = res * sn + (__int128)1680;
    res = res * sn + (__int128)(-252);
    res = res * sn + (__int128)(-16660);
    res = res * sn + (__int128)(-13758);
    res = res * sn + (__int128)63408;
    res = res * sn + (__int128)68705;
    res = res * sn + (__int128)(-104265);
    res = res * sn + (__int128)(-111657);
    res = res * sn + (__int128)66997;
    res = res * sn + (__int128)56682;
    res = res * sn + (__int128)(-11160);

    /* The polynomial is n times the above expression. */
    res = res * sn;

    /* Divide by 45360, the result is guaranteed to be an integer. */
    seq_t result = (seq_t)(res / (__int128)45360);
    return result;
}

/* Main sequence function.  Uses memoisation and the recurrence
 * for n > 13. */
seq_t a(idx_t n)
{
    if (!cache) init_cache();

    /* Return cached value if already computed. */
    if (n <= computed)
        return cache[n];

    /* Grow the cache if necessary. */
    if ((idx_t)capacity <= n) {
        size_t new_cap = (size_t)capacity ? capacity : 1;
        while (new_cap <= (size_t)n)
            new_cap <<= 1;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        capacity = new_cap;
    }

    /* Compute new terms using the recurrence. */
    for (idx_t i = computed + 1; i <= n; ++i) {
        __int128 pos = 0;   /* sum of positive terms  */
        __int128 neg = 0;   /* sum of negative terms  */

        for (int k = 1; k <= 13; ++k) {
            seq_t val = cache[i - k];
            __int128 prod = (__int128)coeff[k - 1] * (__int128)val;
            if (k & 1)          /* odd k: positive sign   */
                pos += prod;
            else                /* even k: negative sign  */
                neg += prod;
        }
        cache[i] = (seq_t)(pos - neg);
    }
    computed = n;
    return cache[n];
}
