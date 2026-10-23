/*
 * A170628  Number of reduced words of length n in Coxeter group on 43
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^48 = I.
 * https://oeis.org/A170628
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

/* Sequence A170628 – reduced words in a Coxeter group on 43 generators.
   Recurrence derived from the rational generating function:
      a(0) = 1
      for n >= 1
         a(n) = c_n + 41 * Σ_{k = 1}^{min(47, n)} a(n - k)
                - (n >= 48 ? 861 * a(n - 48) : 0)
   where c_n = 1 if n = 0 or 48,
             2 if 1 <= n <= 47,
             0 otherwise. */

#include <stddef.h>
#include <stdlib.h>

static void ensure_capacity(idx_t n, seq_t **p, size_t *cap)
{
    size_t need = n + 1;            /* must hold indices 0 … n */
    if (*cap < need)
    {
        size_t new_cap = (*cap > 0) ? *cap : 1;
        while (new_cap < need)
            new_cap <<= 1;          /* grow geometrically */
        seq_t *newp = (seq_t *)realloc(*p, new_cap * sizeof(seq_t));
        if (!newp) abort();        /* allocation failed */
        *p = newp;
        *cap = new_cap;
    }
}

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cap = 0;
    static idx_t last = (idx_t)-1;   /* last computed index, -1 means none */

    /* Make space for indices 0 … n */
    ensure_capacity(n, &cache, &cap);

    /* Initialize first value if this is the first call */
    if (last == (idx_t)-1)
    {
        cache[0] = 1;
        last = 0;
    }

    /* Compute all missing values up to n */
    for (idx_t i = last + 1; i <= n; ++i)
    {
        /* Sum of the previous up to 47 terms */
        seq_t sum = 0;
        idx_t jmax = (i < 47) ? i : 47;
        for (idx_t j = 1; j <= jmax; ++j)
            sum += cache[i - j];

        /* Apply the recurrence */
        seq_t val = sum * 41;
        if (i >= 48)
            val -= 861 * cache[i - 48];

        /* Add the numerator coefficient for this n */
        seq_t cn = 0;
        if (i == 0)
            cn = 1;
        else if (i <= 47)
            cn = 2;
        else if (i == 48)
            cn = 1;
        val += cn;

        cache[i] = val;
    }

    last = n;
    return cache[n];
}
