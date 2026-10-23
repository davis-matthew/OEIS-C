/*
 * A045697  Number of ternary words of length n (beginning 0) with
 *          autocorrelation function 2^(n-1)+3.
 * https://oeis.org/A045697
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 14, u32 n <= 24, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Recurrence:  a(1)=0, a(2)=0, a(3)=1, a(4)=0, a(5)=2,
 * a(2*n)   = 3*a(2*n-1) + 2*a(n) - a(n+1)   (n>=3),
 * a(2*n-1) = 3*a(2*n-2) + 2*a(n)            (n>=3)
 */
#include <stdlib.h>

static seq_t *cache = NULL;          /* cached values (1‑based index)       */
static unsigned char *flag = NULL;   /* 1 = value computed, 0 = not yet     */
static size_t cap = 0;               /* current allocated size (in indices) */

/* enlarge cache to store terms up to at least idx (inclusive) */
static void ensure_cache(idx_t idx)
{
    if ((size_t)idx <= cap) return;
    size_t newcap = cap ? cap : 16;
    while (newcap < (size_t)idx) newcap <<= 1;

    seq_t *newc = (seq_t *)realloc(cache, (newcap + 1) * sizeof(seq_t));
    unsigned char *newf = (unsigned char *)realloc(flag,
                                                   (newcap + 1) * sizeof(unsigned char));
    if (!newc || !newf) abort();                /* allocation failure */

    /* initialize newly allocated slots */
    for (size_t i = cap + 1; i <= newcap; ++i) {
        newc[i] = 0;
        newf[i] = 0;
    }

    cache = newc;
    flag  = newf;
    cap   = newcap;
}

/* OEIS A045697 */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n<1, return 0 */

    ensure_cache(n);
    if (flag[n]) return cache[n];  /* already computed */

    seq_t val;
    if (n == 1) {
        val = 0;
    } else if (n == 2) {
        val = 0;
    } else if (n == 3) {
        val = 1;
    } else if (n == 4) {
        val = 0;
    } else if (n == 5) {
        val = 2;
    } else if ((n & 1U) == 0) { /* even n = 2*k, k>=3 */
        idx_t k = n >> 1;                      /* k = n/2 */
        seq_t a_n_1 = a(n - 1);                /* a(2*k-1)    */
        seq_t a_k    = a(k);                   /* a(k)        */
        seq_t a_k1   = a(k + 1);               /* a(k+1)      */
        val = 3 * a_n_1 + 2 * a_k - a_k1;
    } else { /* odd n = 2*k-1, k>=3 */
        idx_t k = (n + 1) >> 1;                /* k = (n+1)/2 */
        seq_t a_n_1 = a(n - 1);                /* a(2*k-2)    */
        seq_t a_k    = a(k);                   /* a(k)        */
        val = 3 * a_n_1 + 2 * a_k;
    }

    cache[n] = val;
    flag[n]  = 1;
    return val;
}
