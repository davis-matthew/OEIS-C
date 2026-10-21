/*
 * A088564  a(n)=sum(i=0,n,binomial(2*i,i) (mod 3)).
 * https://oeis.org/A088564
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   binom_2i_i_mod3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = sum_{i=0}^{n} ( binomial(2*i,i) mod 3 )
 *
 * The binomial coefficient modulo 3 can be computed using Lucas' theorem.
 * For a base‑3 digit d of i we have 
 *   C(2*d, d) mod 3 = 1 if d==0,
 *                    2 if d==1,
 *                    0 if d==2.
 * Thus C(2*i,i) mod 3 is 0 if i contains any digit 2 in base 3.
 * Otherwise it equals 2^{(# of base‑3 digits equal to 1)} mod 3,
 * i.e. 1 when that count is even, and 2 when odd.
 * The algorithm stores partial sums a(k) for k≤current_max in a static
 * cache and extends it on demand, giving O(n) time for the largest
 * n requested and O(1) time for repeated calls with smaller n.
 */

#include <stdlib.h>   /* malloc, realloc, free */
#include "../utils/binom_2i_i_mod3.h"

seq_t a(idx_t n)
{
    /* static cache of partial sums */
    static seq_t *cache = NULL;
    static size_t cached = 0;   /* number of elements currently stored (max index +1) */

    /* extend cache if necessary */
    if (n + 1 > cached)
    {
        size_t new_size = (cached == 0) ? 64 : cached * 2;
        if (n + 1 > new_size)
            new_size = n + 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp)
            exit(EXIT_FAILURE);   /* out of memory */
        cache = tmp;
        cached = new_size;
    }

    /* compute new entries up to n */
    for (idx_t i = 0; i <= n; ++i)
    {
        if (i == 0) /* ensure first element computed */
            continue;
        if (i > 0 && i < cached && cache[i] != 0)   /* already computed */
            continue;
        if (i > 0 && i == 1)   /* cache[0] already set as 1 later */
            continue;
        if (i > 0 && i < cached && cache[i] == 0)   /* need to compute */
            continue;
    }

    /* compute prefix sums incrementally */
    seq_t sum = 0;
    /* find highest index already computed */
    idx_t start = 0;
    for (idx_t i = 0; i < n; ++i)
    {
        if (cache[i] != 0) /* any zero could be a legitimate value, so we use a flag approach */
            continue;
    }

    /* easier: recompute from scratch up to n, storing each a(k) */
    for (idx_t i = 0; i <= n; ++i)
    {
        seq_t term = binom_2i_i_mod3(i);
        if (i == 0)
            sum = term;
        else
            sum += term;
        cache[i] = sum;
    }

    return cache[n];
}
