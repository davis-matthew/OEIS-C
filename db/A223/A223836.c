/*
 * A223836  Number of n X 6 0..1 arrays with rows, diagonals and antidiagonals
 *          unimodal and columns nondecreasing.
 * https://oeis.org/A223836
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 7*a(n-1) - 21*a(n-2) + 35*a(n-3) * - 35*a(n-4) +
 *            21*a(n-5) - 7*a(n-6) + a(n-7) (n > 10) * Initial values:
 *            a(1)=22, a(2)=148, a(3)=503, a(4)=1286, * a(5)=2884, a(6)=5992,
 *            a(7)=11749, a(8)=21912, * a(9)=39064, a(10)=66854
 * Range:     u8 n <= 2, u16 n <= 9, u32 n <= 67, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Use a static cache – enough for all requested n up to about 10000. */
#include <stdbool.h>
#include <stddef.h>   /* size_t */

#define CACHE_SIZE 20001            /* Index 0 is unused, so valid n <=10000 OK */
static seq_t cache[CACHE_SIZE];
static idx_t max_cached = 0;

/* Pre‑computation of the first 10 terms (index 1..10).  Do it once. */
static void init_base_terms(void)
{
    cache[1] = (seq_t)22;
    cache[2] = (seq_t)148;
    cache[3] = (seq_t)503;
    cache[4] = (seq_t)1286;
    cache[5] = (seq_t)2884;
    cache[6] = (seq_t)5992;
    cache[7] = (seq_t)11749;
    cache[8] = (seq_t)21912;
    cache[9] = (seq_t)39064;
    cache[10] = (seq_t)66854;
    max_cached = 10;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* undefined, but return 0 for safety */

    /* initialise once */
    if (max_cached == 0) init_base_terms();

    /* fast return if already computed */
    if (n <= max_cached) return cache[n];

    /* compute missing terms up to n in ascending order */
    for (idx_t i = max_cached + 1; i <= n; ++i)
    {
        if (i <= 10)
            cache[i] = cache[i];    /* redundant, keeps code uniform */
        else
        {
            seq_t term = 0;
            term += (seq_t)7   * cache[i-1];
            term -= (seq_t)21  * cache[i-2];
            term += (seq_t)35  * cache[i-3];
            term -= (seq_t)35  * cache[i-4];
            term += (seq_t)21  * cache[i-5];
            term -= (seq_t)7   * cache[i-6];
            term +=          cache[i-7];
            cache[i] = term;
        }
    }
    max_cached = n;
    return cache[n];
}
