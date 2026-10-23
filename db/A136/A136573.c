/*
 * A136573  Triangle read by rows: (A000012 * A136572 + A136572 * A000012) -
 *          A000012.
 * https://oeis.org/A136573
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = r! + k! - 1, where r = floor((sqrt(8*n+1)-1)/2) is the
 *            complete row count, and k = n - r(r+1)/2 is the position within
 *            that row
 * Range:     u8 n <= 20, u16 n <= 43, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <math.h>

seq_t a(idx_t n)
{
    /* --- determine the row r ---------------------------------------- */
    idx_t r = (idx_t)((sqrt((long double)8 * n + 1) - 1) / 2);
    /* correct possible rounding errors */
    while ((__uint128_t)r * (r + 1) / 2 > n)
        --r;
    while ((__uint128_t)(r + 1) * (r + 2) / 2 <= n)
        ++r;

    /* --- compute start of the row and column index k ---------------- */
    __uint128_t start128 = (__uint128_t)r * (r + 1) / 2;
    idx_t start = (idx_t)start128;
    idx_t k = n - start;

    /* --- cache for factorials --------------------------------------- */
    static seq_t *fact   = NULL;   /* factorial values */
    static idx_t fact_cap  = 0;    /* currently allocated slots */
    static idx_t fact_used = 0;    /* number of initialized values */

    /* need factorials up to max(r,k) */
    idx_t need = (r > k ? r : k) + 1;
    if (need > fact_cap) {
        idx_t new_cap = fact_cap ? fact_cap * 2 : 1;
        while (new_cap < need) new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(fact, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;            /* allocation failed */
        fact = tmp;
        fact_cap = new_cap;
    }

    /* compute missing factorial values */
    for (idx_t i = fact_used; i < need; ++i) {
        if (i == 0)
            fact[i] = (seq_t)1;               /* 0! = 1 */
        else
            fact[i] = fact[i-1] * (seq_t)i;   /* i! = (i-1)! * i */
    }
    fact_used = need;

    /* --- final result ---------------------------------------------- */
    return fact[r] + fact[k] - (seq_t)1;
}
