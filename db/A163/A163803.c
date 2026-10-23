/*
 * A163803  Number of reduced words of length n in Coxeter group on 47
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^5 = I.
 * https://oeis.org/A163803
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 45*a(n-1)+45*a(n-2)+45*a(n-3)+45*a(n-4)-1035*a(n-5) for n
 *            ≥ 6, with a(0)=1, a(1)=47, a(2)=2162, a(3)=99452, a(4)=4574792,
 *            a(5)=210439351 (these initial values already satisfy the same
 *            recurrence with the additional constant terms coming from the
 *            generating function)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

static const seq_t INIT[6] = {1, 47, 2162, 99452, 4574792, 210439351};
static const seq_t C45 = 45;
static const seq_t C1035 = 1035;

static seq_t *cache = NULL;      /* cached sequence values */
static size_t cap = 0;           /* current allocation size */
static size_t computed = 0;      /* largest index computed (initially 5) */

static void ensure_cache(idx_t n)
{
    if (cache == NULL) {
        cap = 6;
        if ((size_t)n + 1 > cap) cap = (size_t)n + 1;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) abort();           /* allocation failure */
        for (size_t i = 0; i < 6; ++i) cache[i] = INIT[i];
        computed = 5;
    } else if ((size_t)n + 1 > cap) {
        size_t new_cap = cap;
        while ((size_t)n + 1 > new_cap) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cap = new_cap;
    }
}

seq_t a(idx_t n)
{
    if (n < 6) return INIT[n];

    ensure_cache(n);

    for (size_t i = computed + 1; i <= (size_t)n; ++i) {
        seq_t val = C45 * cache[i - 1];
        val += C45 * cache[i - 2];
        val += C45 * cache[i - 3];
        val += C45 * cache[i - 4];
        val -= C1035 * cache[i - 5];
        cache[i] = val;
    }
    computed = (size_t)n;
    return cache[n];
}
