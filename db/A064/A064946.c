/*
 * A064946  a(n) = Sum_{i|n, j|n, j>i} j.
 * https://oeis.org/A064946
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{i|n, j|n, j>i} j
 * Range:     u8 n <= 23, u16 n <= 839, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Compare two idx_t values for qsort */
static int cmp_idx(const void *a, const void *b)
{
    idx_t va = *(const idx_t *)a;
    idx_t vb = *(const idx_t *)b;
    return (va < vb) ? -1 : ((va > vb) ? 1 : 0);
}

/* Compute OEIS sequence A064946 */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* safety; sequence is defined for n≥1 */

    /* Dynamically allocate array for divisors */
    size_t cap = 16;
    size_t sz  = 0;
    idx_t *divs = (idx_t *)malloc(cap * sizeof(idx_t));
    if (!divs) return 0;           /* out‑of‑memory → zero */

    /* Find all divisors of n.  Stop at floor(sqrt(n)). */
    for (idx_t d = 1; d <= n / d; ++d)
    {
        if (n % d != 0) continue;          /* d is not a divisor */

        /* Add divisor d */
        if (sz == cap) {
            cap <<= 1;
            divs = (idx_t *)realloc(divs, cap * sizeof(idx_t));
            if (!divs) return 0;
        }
        divs[sz++] = d;

        /* Add the paired divisor, unless d equals its pair. */
        idx_t e = n / d;
        if (e != d) {
            if (sz == cap) {
                cap <<= 1;
                divs = (idx_t *)realloc(divs, cap * sizeof(idx_t));
                if (!divs) return 0;
            }
            divs[sz++] = e;
        }
    }

    /* Sort divisors in ascending order */
    qsort(divs, sz, sizeof(idx_t), cmp_idx);

    /* Compute the desired sum: each divisor multiplied by the
       number of smaller divisors preceding it. */
    seq_t sum = 0;
    for (size_t k = 0; k < sz; ++k)
        sum += (seq_t)divs[k] * (seq_t)k;

    free(divs);
    return sum;
}
