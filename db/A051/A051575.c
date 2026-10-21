/*
 * A051575  a(n) = LCM { Catalan(0), ..., Catalan(n) }.
 * https://oeis.org/A051575
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = LCM{ Catalan(0), Catalan(1), …, Catalan(n) }
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 13, u64 n <= 23, u128 n <= 48,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/gcd.h"

/* compute lcm of two seq_t values */
static seq_t lcm_seq(seq_t a, seq_t b)
{
    seq_t g = gcd(a, b);
    /* compute a/g * b  (exact division) */
    seq_t temp = a / g;
    return temp * b;
}

/* cache for computed lcm values */
static seq_t *lcm_cache = NULL;
static size_t lcm_cap   = 0;
static idx_t  lcm_len   = 0;   /* number of cached entries, lcm_cache[lcm_len-1] = a(lcm_len-1) */

/* ensure cache can hold at least n+1 entries */
static void ensure_cache(idx_t n)
{
    if (lcm_len > n) return;
    size_t need = (size_t)n + 1;
    if (need <= lcm_cap) return;
    size_t newcap = lcm_cap ? lcm_cap * 2 : 1;
    while (newcap < need) newcap *= 2;
    seq_t *p = (seq_t *)realloc(lcm_cache, newcap * sizeof(seq_t));
    if (!p) abort();                  /* allocation failed */
    lcm_cache = p;
    lcm_cap   = newcap;
}

/* main function for A051575 */
seq_t a(idx_t n)
{
    /* Handle zero directly */
    if (n == 0) return (seq_t)1;

    ensure_cache(n);

    /* compute from last cached index + 1 up to n */
    seq_t catalan = (seq_t)1;        /* Catalan(0) = 1 */
    seq_t current_lcm = lcm_cache[lcm_len-1];   /* if lcm_len>0 else 1 */

    /* if nothing cached yet, initialize */
    if (lcm_len == 0) {
        lcm_cache[0] = (seq_t)1;
        current_lcm  = (seq_t)1;
        catalan      = (seq_t)1;
        lcm_len      = 1;
    } else {
        /* set catalan to value at last cached index */
        catalan = (seq_t)1;
        for (idx_t i = 1; i < lcm_len; ++i) {
            /* Catalan(i) = Catalan(i-1) * (4*i-2)/(i+1) */
            catalan = catalan * (seq_t)(4 * i - 2) / (seq_t)(i + 1);
        }
    }

    for (idx_t i = lcm_len; i <= n; ++i) {
        if (i > 0) {
            /* compute Catalan(i) from previous value */
            catalan = catalan * (seq_t)(4 * i - 2) / (seq_t)(i + 1);
        }
        current_lcm = lcm_seq(current_lcm, catalan);
        lcm_cache[lcm_len++] = current_lcm;
    }

    return lcm_cache[n];
}
