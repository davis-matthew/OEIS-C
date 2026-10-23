/*
 * A160756  Triangle read by rows, infinite lower triangular Toeplitz matrix
 *          with A078008 in every column convolved with A001333.
 * https://oeis.org/A160756
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = c_{r-k} * q_k where (r,k) are obtained from n by the
 *            mapping r = floor((sqrt(8*n+1)-1)/2) (row index) k = n -
 *            r*(r+1)/2 (column index) c_m = (2^m + 2*(-1)^m) / 3 (A078008)
 *            q_0 = 1 q_k = A001333(k-1) for k>0 A001333 sequence: a_0 = 1,
 *            a_1 = 1, a_n = 2*a_{n-1} + a_{n-2} (n>=2)
 * Range:     u8 n <= 35, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>

/* === utility to compute 2^exp in seq_t === */
static seq_t pow_two(idx_t exp)
{
    seq_t result = (seq_t)1;
    seq_t base   = (seq_t)2;
    idx_t e = exp;
    while (e)
    {
        if (e & 1) result *= base;
        base *= base;
        e >>= 1;
    }
    return result;
}

/* === cache for A001333 sequence === */
static seq_t *A001333_cache = NULL;
static size_t A001333_cap   = 0;
/* ensure cache computed up to at least index 'need' (inclusive) */
static void ensure_A001333(size_t need)
{
    /* initial allocation if needed */
    if (A001333_cache == NULL)
    {
        size_t init = 2;
        A001333_cache = (seq_t*)malloc(init * sizeof(seq_t));
        if (!A001333_cache) return;                 /* out of memory – caller will fail */
        A001333_cache[0] = (seq_t)1;
        A001333_cache[1] = (seq_t)1;
        A001333_cap = init;
    }
    /* grow cache if necessary */
    if (A001333_cap <= need)
    {
        size_t new_cap = A001333_cap;
        while (new_cap <= need) new_cap <<= 1;      /* double until large enough */
        seq_t *tmp = (seq_t*)realloc(A001333_cache, new_cap * sizeof(seq_t));
        if (!tmp) return;                           /* out of memory – caller will fail */
        A001333_cache = tmp;
        /* compute new elements */
        for (size_t i = A001333_cap; i < new_cap; ++i)
        {
            if (i == 1)                           /* value already set */
                continue;
            A001333_cache[i] = (A001333_cache[i - 1] << 1) + A001333_cache[i - 2];
        }
        A001333_cap = new_cap;
    }
}

/* === main sequence function === */
seq_t a(idx_t n)
{
    /* ---- find row r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2 ---- */
    idx_t lo = 0, hi = 1;
    /* grow hi until it overshoots */
    while (((__int128)hi * (hi + 1) / 2) <= n)
    {
        if (hi >= (UINT64_MAX >> 1)) break;       /* avoid overflow of hi<<=1 */
        hi <<= 1;
    }
    while (lo < hi)
    {
        idx_t mid = lo + (hi - lo + 1) / 2;
        if (((__int128)mid * (mid + 1) / 2) <= n)
            lo = mid;
        else
            hi = mid - 1;
    }
    idx_t r = lo;

    /* ---- column index ---- */
    idx_t offset = (idx_t)((__int128)r * (r + 1) / 2);
    idx_t k = n - offset;          /* 0 <= k <= r   */
    idx_t j = r - k;                /* index for A078008 */

    /* ---- compute A078008(j) = (2^j + 2*(-1)^j)/3 ---- */
    seq_t pow2j = pow_two(j);
    seq_t num   = (j % 2 == 0) ? (pow2j + (seq_t)2) : (pow2j - (seq_t)2);
    seq_t cn    = num / (seq_t)3;   /* guaranteed integral */

    /* ---- compute q_k ---- */
    seq_t qk;
    if (k == 0)
        qk = (seq_t)1;
    else
    {
        ensure_A001333(k - 1);
        qk = A001333_cache[k - 1];
    }

    return cn * qk;
}
