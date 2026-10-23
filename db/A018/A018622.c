/*
 * A018622  Divisors of 742.
 * https://oeis.org/A018622
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = n-th divisor of 742 in ascending order
 * Range:     u8 n <= 6, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* Comparator for sorting seq_t values */
static int cmp_divs(const void *a, const void *b)
{
    seq_t va = *(const seq_t *)a;
    seq_t vb = *(const seq_t *)b;
    return (va < vb) ? -1 : (va > vb) ? 1 : 0;
}

seq_t a(idx_t n)
{
    /* static state: the list of divisors of 742 */
    static seq_t *divs = NULL;
    static size_t  divcnt = 0;
    static int    init   = 0;

    if (!init) {
        const uint64_t N = 742ULL;   /* the fixed number */

        /* find integer square root of N */
        uint64_t sqrt_n = 0;
        while ((sqrt_n + 1) * (sqrt_n + 1) <= N)
            ++sqrt_n;

        /* allocate space for the divisors (max 8 for this N) */
        size_t cap = 8;
        divs = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!divs) return 0;               /* allocation failed */

        /* collect divisors up to sqrt(N) and their complements */
        for (uint64_t i = 1; i <= sqrt_n; ++i) {
            if (N % i == 0) {
                if (divcnt >= cap) {           /* grow array if needed */
                    cap *= 2;
                    seq_t *tmp = (seq_t *)realloc(divs, cap * sizeof(seq_t));
                    if (!tmp) return 0;
                    divs = tmp;
                }
                divs[divcnt++] = (seq_t)i;

                uint64_t j = N / i;
                if (j != i) {
                    if (divcnt >= cap) {
                        cap *= 2;
                        seq_t *tmp = (seq_t *)realloc(divs, cap * sizeof(seq_t));
                        if (!tmp) return 0;
                        divs = tmp;
                    }
                    divs[divcnt++] = (seq_t)j;
                }
            }
        }

        /* sort the divisors in ascending order */
        qsort(divs, divcnt, sizeof(seq_t), cmp_divs);
        init = 1;
    }

    /* handle out of range indices gracefully */
    if (n == 0) return 0;
    if ((idx_t)(n - 1) >= (idx_t)divcnt) return 0;

    return divs[n - 1];
}
