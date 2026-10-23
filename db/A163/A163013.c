/*
 * A163013  Number of n X 4 binary arrays with all 1s connected and a path of
 *          1s from top row to lower right corner.
 * https://oeis.org/A163013
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   * a(n) = 15*a(n-1) -59*a(n-2) +97*a(n-3) -19*a(n-4) -210*a(n-5)
 *            * +222*a(n-6) +22*a(n-7) -113*a(n-8) +7*a(n-9) * -71*a(n-10)
 *            +13*a(n-11) +a(n-12) * for n ≥ 13, with the initial values
 *            listed below
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 9, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *seq_cache = NULL;      /* array storing computed terms, index 1.. */
static idx_t  seq_cache_size = 0;    /* allocated size (≥ needed index + 1) */
static idx_t  seq_cache_computed = 0;/* highest index computed */

/* ensure the cache is large enough for indices up to n (plus index 0 unused) */
static void seq_ensure_capacity(idx_t n)
{
    if (n + 1 <= seq_cache_size) return;
    idx_t new_cap = seq_cache_size ? seq_cache_size * 2 : 16;
    if (new_cap < (idx_t)(n + 1)) new_cap = n + 1;
    seq_t *new_cache = (seq_t *)realloc(seq_cache, (size_t)(new_cap + 1) * sizeof(seq_t));
    if (!new_cache) exit(EXIT_FAILURE);
    seq_cache = new_cache;
    seq_cache_size = new_cap;
}

/* initialize cache with first 12 terms */
static void seq_cache_init(void)
{
    seq_ensure_capacity(12);
    seq_cache[1]  = (seq_t)4;
    seq_cache[2]  = (seq_t)44;
    seq_cache[3]  = (seq_t)469;
    seq_cache[4]  = (seq_t)4776;
    seq_cache[5]  = (seq_t)48047;
    seq_cache[6]  = (seq_t)482895;
    seq_cache[7]  = (seq_t)4854639;
    seq_cache[8]  = (seq_t)48809845;
    seq_cache[9]  = (seq_t)490753552;
    seq_cache[10] = (seq_t)4934223120;
    seq_cache[11] = (seq_t)49610514652;
    seq_cache[12] = (seq_t)498802509247;
    seq_cache_computed = 12;
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;          /* below offset: unspecified */

    if (!seq_cache) seq_cache_init();

    if (n <= seq_cache_computed) return seq_cache[n];

    seq_ensure_capacity(n);

    for (idx_t i = seq_cache_computed + 1; i <= n; ++i)
    {
        seq_t pos = 0;   /* sum of terms with positive coefficients */
        seq_t neg = 0;   /* sum of terms with negative coefficients */

        pos += seq_cache[i-1] * (seq_t)15;   /* 15*a(n-1)   */
        pos += seq_cache[i-3] * (seq_t)97;   /* 97*a(n-3)   */
        pos += seq_cache[i-6] * (seq_t)222;  /* 222*a(n-6)  */
        pos += seq_cache[i-7] * (seq_t)22;   /* 22*a(n-7)   */
        pos += seq_cache[i-9] * (seq_t)7;    /* 7*a(n-9)    */
        pos += seq_cache[i-11] * (seq_t)13;  /* 13*a(n-11)  */
        pos += seq_cache[i-12];              /* a(n-12)     */

        neg += seq_cache[i-2] * (seq_t)59;   /* -59*a(n-2)  */
        neg += seq_cache[i-4] * (seq_t)19;   /* -19*a(n-4)  */
        neg += seq_cache[i-5] * (seq_t)210;  /* -210*a(n-5) */
        neg += seq_cache[i-8] * (seq_t)113;  /* -113*a(n-8) */
        neg += seq_cache[i-10] * (seq_t)71;  /* -71*a(n-10) */

        seq_cache[i] = pos - neg;
    }

    seq_cache_computed = n;
    return seq_cache[n];
}
