/*
 * A049964  a(n) = a(1) + a(2) + ... + a(n-1) + a(m) for n >= 4, where m = 2*n
 *          - 3 - 2^(p+1) and p is the unique integer such that 2^p < n-1 <=
 *          2^(p+1), with a(1) = 1, a(2) = 3, and a(3) = 1.
 * https://oeis.org/A049964
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=1, a(2)=3, a(3)=1 a(n) = sum_{i=1}^{n-1} a(i) +
 *            a(2*n-3-2^ceil(log2(n-1))) for n >= 4 Sequence A049964
 * Range:     u8 n <= 9, u16 n <= 16, u32 n <= 32, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Memoised array of values and prefix sums */
static seq_t *a_val = NULL;
static seq_t *pref_sum = NULL;
static size_t alloc_size = 0;

/* Ensure that a_val[1..n] and pref_sum[1..n] are computed. */
static void ensure(idx_t n)
{
    if ((idx_t)alloc_size > n) return;            /* already computed */

    size_t new_size = (size_t)n + 1;              /* need indices 0..n */
    seq_t *new_a  = (seq_t *)realloc(a_val, new_size * sizeof(seq_t));
    seq_t *new_p  = (seq_t *)realloc(pref_sum, new_size * sizeof(seq_t));
    if (!new_a || !new_p) abort();               /* allocation failure */

    a_val = new_a;
    pref_sum = new_p;

    if (alloc_size == 0)                      /* first time -> zero prefix[0] */
        pref_sum[0] = (seq_t)0;

    /* Compute from the first missing index up to n */
    for (size_t i = (alloc_size == 0 ? 1 : alloc_size); i <= new_size - 1; ++i) {
        if (i == 1) {
            a_val[1] = (seq_t)1;
            pref_sum[1] = a_val[1];
            continue;
        }
        if (i == 2) {
            a_val[2] = (seq_t)3;
            pref_sum[2] = pref_sum[1] + a_val[2];            /* 1 + 3 = 4 */
            continue;
        }
        if (i == 3) {
            a_val[3] = (seq_t)1;
            pref_sum[3] = pref_sum[2] + a_val[3];            /* 4 + 1 = 5 */
            continue;
        }

        /* Compute m = 2*i - 3 - 2^ceil(log2(i-1)) */
        idx_t ii = (idx_t)i;
        idx_t val = ii - 1;                  /* n-1 */
        int l = 63 - __builtin_clzll((unsigned long long)val); /* floor log2 */
        idx_t pow;
        if ( (val & (val-1)) == 0 )          /* val is a power of two */
            pow = ((idx_t)1) << l;
        else
            pow = ((idx_t)1) << (l + 1);     /* ceil log2 */

        idx_t m = (idx_t)2*ii - 3 - pow;

        seq_t pref = pref_sum[i-1];           /* sum_{1..i-1} */
        seq_t am   = a_val[(size_t)m];       /* a(m) is already computed */

        a_val[i] = pref + am;                /* a(i) = sum_{<i} + a(m) */
        pref_sum[i] = pref + a_val[i];       /* new prefix sum */
    }

    alloc_size = new_size;
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;                /* undefined for n<1 */
    ensure(n);
    return a_val[(size_t)n];
}
