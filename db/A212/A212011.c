/*
 * A212011  Triangle read by rows: T(n,k) is the sum of all parts of the last
 *          k sections of the modular table of partitions of n.
 * https://oeis.org/A212011
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(row,col) where T(n,k)=A066186(n)-A066186(n-k), and the
 *            triangle is read by rows: a(1)=T(1,1), a(2)=T(2,1), a(3)=T(2,2),
 *            …
 * Range:     u8 n <= 41, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

/*   Static arrays holding intermediate values
    part[n]   = number of partitions of n
    sigma[n]  = sum of divisors of n   (sigma(0) unused)
    A[n]      = A066186(n) = sum of all parts of all partitions of n   (A[0] = 0)
*/
static seq_t *part = NULL;
static seq_t *sigma = NULL;
static seq_t *A    = NULL;
static size_t cap = 0;         /* current allocated size */

/* Recompute all data up to new_cap elements */
static void recompute(size_t new_cap)
{
    seq_t *p = (seq_t *)malloc(new_cap * sizeof(seq_t));
    seq_t *s = (seq_t *)malloc(new_cap * sizeof(seq_t));
    seq_t *a = (seq_t *)malloc(new_cap * sizeof(seq_t));
    if (!p || !s || !a) abort();

    for (size_t i = 0; i < new_cap; ++i) {
        p[i] = 0;
        s[i] = 0;
        a[i] = 0;
    }

    /* partitions :  p[0] = 1,  use unlimited coin change DP   */
    p[0] = 1;
    for (size_t i = 1; i < new_cap; ++i)
        for (size_t j = i; j < new_cap; ++j)
            p[j] += p[j - i];

    /* sigma : divisor sums */
    for (size_t k = 1; k < new_cap; ++k)
        for (size_t m = k; m < new_cap; m += k)
            s[m] += (seq_t)k;

    /* A066186 : convolution of sigma and partitions */
    for (size_t n = 1; n < new_cap; ++n) {
        seq_t tot = 0;
        for (size_t k = 1; k <= n; ++k) {
            tot += s[k] * p[n - k];
        }
        a[n] = tot;
    }

    /* free old arrays and replace */
    free(part);
    free(sigma);
    free(A);
    part = p;
    sigma = s;
    A    = a;
    cap  = new_cap;
}

/* Ensure arrays have size at least 'need' */
static void ensure_capacity(size_t need)
{
    if (need <= cap) return;
    size_t new_cap = (cap > 0) ? cap * 2 : 16;
    if (new_cap < need) new_cap = need;
    recompute(new_cap);
}

/* Main sequence function.  n is 1‑based index into the triangle
   read by rows.  */
seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* out of bounds, safe return */

    /* find row r such that r*(r+1)/2 >= n */
    idx_t r = 0;
    while ((unsigned long long)r * (r + 1) / 2 < (unsigned long long)n) ++r;

    /* column k = n - row*(row-1)/2 */
    idx_t k = n - (unsigned long long)r * (r - 1) / 2;

    ensure_capacity((size_t)r);        /* need data up to row r */

    /* T(r,k) = A066186(r) - A066186(r-k)  */
    return A[r] - A[r - k];
}
