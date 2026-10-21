/*
 * A098057  Number of peakless Motzkin paths with no U H^j U, no D H^j D and
 *          no D H^jU (j>0), where U=(1,1), D=(1,-1) and H=(1,0) (can be
 *          easily expressed using RNA secondary structure terminology).
 * https://oeis.org/A098057
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 15, u32 n <= 32, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:
   (n+2)*a(n)+(-5*n-7)*a(n-1)+2*(4*n+5)*a(n-2)+(-2*n-13)*a(n-3)
   +3*(-2*n+5)*a(n-4)+18*a(n-5)+(17*n-101)*a(n-6)+(-25*n+154)*a(n-7)
   +2*(8*n-53)*a(n-8)+4*(-n+7)*a(n-9)=0
   with a(0)=1,a(1)=1,a(2)=1,a(3)=2,a(4)=4,a(5)=8,a(6)=15,a(7)=27,a(8)=48
   The function implements this relation iteratively, memoizing the results.
*/

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;

/* ensure capacity for at least 'need' elements */
static void ensure_cache(size_t need)
{
    if (cache_cap < need)
    {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        while (new_cap < need) new_cap *= 2;
        cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    }
}

/* initialise base terms a(0) … a(8) */
static void init_cache(void)
{
    if (cache_len != 0) return;
    ensure_cache(9);
    cache[0] = (seq_t)1;
    cache[1] = (seq_t)1;
    cache[2] = (seq_t)1;
    cache[3] = (seq_t)2;
    cache[4] = (seq_t)4;
    cache[5] = (seq_t)8;
    cache[6] = (seq_t)15;
    cache[7] = (seq_t)27;
    cache[8] = (seq_t)48;
    cache_len = 9;
}

seq_t a(idx_t n)
{
    init_cache();

    /* if already computed */
    if ((size_t)n < cache_len) return cache[n];

    /* compute up to n */
    ensure_cache((size_t)n + 1);
    for (size_t i = cache_len; i <= (size_t)n; ++i)
    {
        idx_t idx = (idx_t)i;
        seq_t num = 0;

        /* (5*n+7)*a(n-1) */
        num += ((seq_t)(5*idx + 7)) * cache[i - 1];

        /* -2*(4*n+5)*a(n-2) */
        num -= ((seq_t)(2 * (4*idx + 5))) * cache[i - 2];

        /* +(2*n+13)*a(n-3) */
        num += ((seq_t)(2*idx + 13)) * cache[i - 3];

        /* +(6*n-15)*a(n-4) */
        num += ((seq_t)(6*idx - 15)) * cache[i - 4];

        /* -18*a(n-5) */
        num -= ((seq_t)18) * cache[i - 5];

        /* -(17*n-101)*a(n-6) */
        num -= ((seq_t)(17*idx - 101)) * cache[i - 6];

        /* +(25*n-154)*a(n-7) */
        num += ((seq_t)(25*idx - 154)) * cache[i - 7];

        /* -(16*n-106)*a(n-8) */
        num -= ((seq_t)(16*idx - 106)) * cache[i - 8];

        /* +(4*n-28)*a(n-9) */
        num += ((seq_t)(4*idx - 28)) * cache[i - 9];

        cache[i] = num / ((seq_t)(idx + 2));
    }
    cache_len = (size_t)n + 1;
    return cache[n];
}
