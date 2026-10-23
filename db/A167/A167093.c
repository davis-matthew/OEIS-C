/*
 * A167093  Number of reduced words of length n in Coxeter group on 40
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^13 = I.
 * https://oeis.org/A167093
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=40, a(2)=1560, a(3)=60840, a(4)=2372760,
 *            a(5)=92537640, a(6)=3608967960, a(7)=140749750440,
 *            a(8)=5489240267160, a(9)=214080370419240,
 *            a(10)=8349134446350360, a(11)=325616243407664040,
 *            a(12)=12699033492898897560. For n ≥ 14: a(n) =
 *            38·(a(n−1)+a(n−2)+…+a(n−12)) − 741·a(n−13). The value...
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* The first 13 terms (indices 0 .. 12). */
static const seq_t first13[13] =
{
    1ULL,
    40ULL,
    1560ULL,
    60840ULL,
    2372760ULL,
    92537640ULL,
    3608967960ULL,
    140749750440ULL,
    5489240267160ULL,
    214080370419240ULL,
    8349134446350360ULL,
    325616243407664040ULL,
    12699033492898897560ULL
};

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static idx_t cached_upto = 0;

    /* Initialise the cache with the first 13 known terms. */
    if (!cache)
    {
        cache_cap = 13;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        for (idx_t i = 0; i < 13; ++i)
            cache[i] = first13[i];
        cached_upto = 12;  /* last pre‑computed index */
    }

    /* Grow the cache if the requested index is beyond current capacity. */
    if ((size_t)n + 1 > cache_cap)
    {
        size_t new_cap = cache_cap ? cache_cap : 1;
        while (new_cap <= (size_t)n)
            new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Return the value if it has already been computed. */
    if (n <= cached_upto)
        return cache[(size_t)n];

    /* Compute missing terms up to n. */
    for (idx_t i = cached_upto + 1; i <= n; ++i)
    {
        seq_t val;
        if (i < 13)
        {
            /* The first 13 terms are already supplied. */
            val = cache[(size_t)i];
        }
        else if (i == 13)
        {
            /* Special case for a(13) (extra +1 from numerator). */
            seq_t sum = 0;
            for (idx_t k = 1; k <= 12; ++k)
                sum += cache[(size_t)(i - k)];
            val = sum * 38;
            val -= cache[0] * 741;
            val += 1;  /* extra term from the numerator */
        }
        else
        {
            /* Recurrence for n ≥ 14: a(n) = 38*sum - 741*a(n-13). */
            seq_t sum = 0;
            for (idx_t k = 1; k <= 12; ++k)
                sum += cache[(size_t)(i - k)];
            val = sum * 38;
            val -= cache[(size_t)(i - 13)] * 741;
        }
        cache[(size_t)i] = val;
    }

    cached_upto = n;
    return cache[(size_t)n];
}
