/*
 * A023560  Convolution of A023531 and composite numbers (A002808).
 * https://oeis.org/A023560
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=1}^{n} A023531(k) * A002808(n+1−k) * where
 *            A023531(k) = 1 if k+1 is a triangular number, otherwise 0 * and
 *            A002808 is the sequence of composite numbers (offset 1)
 * Range:     u8 n <= 39, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

static bool is_prime_uint64(idx_t x)
{
    if (x < 2) return false;
    if (x == 2) return true;
    if ((x & 1) == 0) return false;
    for (idx_t d = 3; d <= x / d; d += 2)
        if (x % d == 0) return false;
    return true;
}

static idx_t isqrt_u64(idx_t x)
{
    idx_t lo = 0, hi = x, res = 0;
    while (lo <= hi)
    {
        idx_t mid = (lo + hi) >> 1;
        idx_t sq  = mid * mid;
        if (sq == x) return mid;
        if (sq < x)
        {
            lo = mid + 1;
            res = mid;
        }
        else
            hi = mid - 1;
    }
    return res;
}

seq_t a(idx_t n)
{
    static seq_t *comps = NULL;  /* 1‑based array of composite numbers */
    static bool *ind   = NULL;   /* 1‑based indicator of A023531 */
    static idx_t size  = 0;      /* current allocated size (max index) */
    static idx_t last_checked = 1;   /* last integer tested for compositeness */
    static idx_t comp_cnt = 0;       /* number of composites stored */

    if (n == 0) return 0;

    if (n > size)
    {
        /* grow storage */
        size_t new_size = (size_t)n + 1;   /* +1 for 1‑based indexing */
        comps = (seq_t *)realloc(comps, new_size * sizeof(seq_t));
        ind   = (bool   *)realloc(ind,   new_size * sizeof(bool));

        /* generate remaining composite numbers */
        while (comp_cnt < n)
        {
            ++last_checked;
            if (!is_prime_uint64(last_checked))
            {
                ++comp_cnt;
                comps[comp_cnt] = (seq_t)last_checked;
            }
        }

        /* compute indicators for new positions */
        for (idx_t i = size + 1; i <= n; ++i)
        {
            idx_t x = i + 1;          /* value n+1 for this index */
            idx_t m = 8 * x + 1;      /* (2k+1)^2 if x is triangular */
            idx_t s = isqrt_u64(m);
            ind[i] = (s * s == m);
        }
        size = n;
    }

    /* convolution */
    seq_t sum = 0;
    for (idx_t k = 1; k <= n; ++k)
        if (ind[k])
            sum += comps[n + 1 - k];

    return sum;
}
