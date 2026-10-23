/*
 * A170723  Number of reduced words of length n in Coxeter group on 42
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^50 = I.
 * https://oeis.org/A170723
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 40 * Σ_{i=1}^{49} a(n-i) - 820 * a(n-50) + p(n) where
 *            p(0)=p(50)=1, p(1..49)=2, p(n)=0 otherwise
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* small‑integer multiplication; works for built‑in integers
   and for the arbitrary‑precision type used by seq_t */
static inline seq_t mul_small(seq_t x, unsigned int m)
{
    return x * (seq_t)m;
}

static seq_t *a_cache = NULL;   /* storage for computed terms */
static size_t a_cached = 0;     /* number of terms currently in cache (a(0) … a(n-1)) */
static size_t a_cap = 0;        /* current capacity of a_cache */

/* increase capacity to be able to store indices up to 'idx' */
static void ensure_capacity(size_t idx)
{
    if (a_cap <= idx)
    {
        size_t new_cap = (a_cap == 0) ? 64 : a_cap * 2;
        while (new_cap <= idx) new_cap <<= 1;
        seq_t *tmp = (seq_t*)realloc(a_cache, new_cap * sizeof(seq_t));
        a_cache = tmp;
        a_cap = new_cap;
    }
}

/* coefficient p(n) of the numerator polynomial */
static inline seq_t p_value(idx_t n)
{
    if (n == 0 || n == 50) return (seq_t)1;
    if (n <= 49)           return (seq_t)2;
    return (seq_t)0;
}

/* public sequence function */
seq_t a(idx_t n)
{
    size_t target = (size_t)n;
    ensure_capacity(target);

    /* compute missing values up to n */
    for (size_t k = a_cached; k <= target; ++k)
    {
        seq_t sum = (seq_t)0;

        /* 40 * a(k-1) + … + 40 * a(k-49) */
        for (int i = 1; i <= 49; ++i)
            if (k >= (size_t)i)
                sum += mul_small(a_cache[k - (size_t)i], 40);

        /* –820 * a(k-50) */
        seq_t sub = (k >= 50) ? mul_small(a_cache[k - 50], 820) : (seq_t)0;

        /* add the numerator coefficient */
        sum += p_value((idx_t)k);

        /* a(k) = sum – sub  (non‑negative because the sequence is positive) */
        a_cache[k] = sum - sub;
    }
    a_cached = target + 1;              /* all terms up to n are now cached */

    return a_cache[target];
}
