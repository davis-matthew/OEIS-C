/*
 * A169439  Number of reduced words of length n in Coxeter group on 42
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^32 = I.
 * https://oeis.org/A169439
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A169439: number of reduced words of length n in a Coxeter group
 * with 42 generators and relations S_i^2 = (S_i S_j)^32 = I.
 * Recurrence:
 *   a(n) = 40 * Σ_{k=1}^{31} a(n-k) - 820 * a(n-32) + b(n)
 *   with b(0)=1, b(1..31)=2, b(32)=1, otherwise 0.
 * a(0)=1, and a(n)=0 for n<0.  The recurrence holds for all n≥0.
 */

#include <stdlib.h>
#include <stdint.h>

static seq_t *cache = NULL;      /* memoisation array */
static size_t cache_cap = 0;     /* allocated capacity */
static size_t computed_until = 0;/* largest index for which a(i) is known */

/* ensure array has capacity for at least n+1 elements */
static void ensure_capacity(idx_t n)
{
    size_t need = (size_t)(n + 1);
    if (cache_cap >= need) return;

    size_t new_cap = cache_cap ? cache_cap : 1;
    while (new_cap < need) new_cap <<= 1;

    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) { /* allocation failed – return 0, caller may handle */ return; }
    /* initialise newly allocated part to 0 (valid sentinel for memoisation) */
    for (size_t i = cache_cap; i < new_cap; ++i) tmp[i] = 0;
    cache = tmp;
    cache_cap = new_cap;
}

/* compute a(n) with memoisation and the recurrence above */
seq_t a(idx_t n)
{
    if (n == (idx_t)-1) return 0;            /* guard against accidental large value */

    /* initialisation: ensure a(0)=1 is stored */
    if (!cache)
    {
        ensure_capacity(0);
        cache[0] = 1;
        computed_until = 0;
    }

    if ((size_t)n > computed_until)
    {
        /* extend cache if needed */
        ensure_capacity(n);

        /* compute values from computed_until+1 up to n */
        for (size_t i = computed_until + 1; i <= (size_t)n; ++i)
        {
            /* sum of the previous up to 31 terms */
            size_t start = i > 31 ? i - 31 : 0;
            seq_t sum = 0;
            for (size_t j = start; j < i; ++j) sum += cache[j];

            /* compute recurrence value */
            seq_t val = (seq_t)((unsigned __int128)40 * sum);
            if (i >= 32)
                val -= (seq_t)((unsigned __int128)820 * cache[i - 32]);

            /* add b(i) */
            if (i == 0)            val += 1;
            else if (i <= 31)      val += 2;
            else if (i == 32)      val += 1;

            cache[i] = val;
        }
        computed_until = (size_t)n;
    }

    return cache[n];
}
