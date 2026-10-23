/*
 * A179575  Number of permutations of 1..n+8 with the number moved left
 *          exceeding the number moved right by n.
 * https://oeis.org/A179575
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A062866(n+8,n) (linear recurrence of order 16)
 * Range:     u8 none, u16 none, u32 n <= 8, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>


/* First 16 terms of the sequence (a(1) … a(16)) */
static const seq_t base_terms[16] = {
    (seq_t)85831ull,
    (seq_t)482272ull,
    (seq_t)2425600ull,
    (seq_t)11370998ull,
    (seq_t)50869482ull,
    (seq_t)220378172ull,
    (seq_t)933464692ull,
    (seq_t)3891019939ull,
    (seq_t)16033227477ull,
    (seq_t)65516894644ull,
    (seq_t)266104371580ull,
    (seq_t)1076056450400ull,
    (seq_t)4337364627584ull,
    (seq_t)17442468207248ull,
    (seq_t)70026476198736ull,
    (seq_t)280798779858421ull
};

/* Recurrence coefficients q1 … q16 of the denominator Q(x) */
static const int64_t coeff[17] = {
    0LL,                     /* unused index 0 */
   -30LL,                    /* q1   */
    415LL,                   /* q2   */
   -3514LL,                  /* q3   */
    20386LL,                 /* q4   */
   -85924LL,                 /* q5   */
    272198LL,                /* q6   */
   -661180LL,                /* q7   */
    1244717LL,               /* q8   */
   -1822478LL,               /* q9   */
    2068955LL,               /* q10  */
   -1802474LL,               /* q11  */
    1181760LL,               /* q12  */
   -563888LL,                /* q13  */
    184752LL,                /* q14  */
   -37152LL,                 /* q15  */
    3456LL                    /* q16  */
};

/* Cache for computed terms: cache[0] = a(1), cache[1] = a(2), … */
static seq_t *cache = NULL;
static size_t cache_len = 0;

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;   /* sequence is not defined for n = 0 */

    /* Ensure the cache can hold at least n terms (or at least 16 for the recurrence) */
    size_t need = (n > 16 ? (size_t)n : 16);

    if (cache_len < need) {
        /* Allocate or grow the cache */
        size_t new_len = need;
        seq_t *new_cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!new_cache)
            return 0;  /* allocation failed – return 0 */

        cache = new_cache;

        /* Copy the known first 16 terms if they are not already present */
        if (cache_len < 16) {
            for (size_t i = cache_len; i < 16; ++i)
                cache[i] = base_terms[i];
        }

        /* Compute missing terms using the linear recurrence */
        /* Computation starts from the first index that definitely satisfies the recurrence */
        size_t start = (cache_len >= 16 ? cache_len : 16);
        for (size_t k = start + 1; k <= new_len; ++k) {
            seq_t sum = 0;
            /* a(k) = -(q1*a(k-1) + ... + q16*a(k-16)) */
            /* The coefficients are stored in coeff[1..16] */
            for (int j = 1; j <= 16; ++j)
                sum += ((seq_t)coeff[j]) * cache[k - j - 1];
            cache[k - 1] = -sum;
        }

        cache_len = new_len;
    }

    return cache[(size_t)n - 1];
}
