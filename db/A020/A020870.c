/*
 * A020870  Number of strong single-component edge-subgraphs in Moebius ladder
 *          M_n.
 * https://oeis.org/A020870
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n-1 + [n(17-√17)/34+1]*((5+√17)/2)^n +
 *            [n(17+√17)/34+1]*((5-√17)/2)^n Recurrence (derived from the
 *            generating function denominator (1-x)^2(1-5x+2x^2)^2): a(n) =
 *            12·a(n-1) - 50·a(n-2) + 88·a(n-3) - 73·a(n-4) + 28·a(n-5) -
 *            4·a(n-6) for n ≥ 8 Initial values: a(2)=38, a(3)=205, a(4)=1092,
 *            a(5)...
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 27, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;      /* cache[0] and cache[1] unused/0, cache[n] holds a(n) */
static idx_t  cached_until = 0;  /* largest n for which cache[n] is valid */

static void
ensure_cache(idx_t n)
{
    if (n <= cached_until)
        return;

    /* enlarge cache to hold indices up to n */
    seq_t *new_cache = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
    if (!new_cache && n + 1 > 0)  /* allocation failed */
        exit(EXIT_FAILURE);
    cache = new_cache;

    /* initialise freshly allocated positions */
    for (idx_t i = cached_until + 1; i <= n; ++i)
    {
        if (i == 2)
            cache[i] = (seq_t)38;
        else if (i == 3)
            cache[i] = (seq_t)205;
        else if (i == 4)
            cache[i] = (seq_t)1092;
        else if (i == 5)
            cache[i] = (seq_t)5719;
        else if (i == 6)
            cache[i] = (seq_t)29486;
        else if (i == 7)
            cache[i] = (seq_t)150049;
        else   /* compute via recurrence for i >= 8 */
        {
            seq_t term = ((seq_t)12   * cache[i-1])
                       - ((seq_t)50   * cache[i-2])
                       + ((seq_t)88   * cache[i-3])
                       - ((seq_t)73   * cache[i-4])
                       + ((seq_t)28   * cache[i-5])
                       - ((seq_t)4    * cache[i-6]);
            cache[i] = term;
        }
    }
    cached_until = n;
}

seq_t a(idx_t n)
{
    if (n < 2)
        return (seq_t)0;            /* undefined below offset, return 0 */
    ensure_cache(n);
    return cache[n];
}
