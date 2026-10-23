/*
 * A008773  Expansion of (1+x^12)/((1-x)*(1-x^2)*(1-x^3)*(1-x^4)).
 * https://oeis.org/A008773
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = coefficient of x^n in
 *            (1+x^12)/((1-x)(1-x^2)(1-x^3)(1-x^4)) Equivalently, a(n) = p(n)
 *            + p(n-12) where p(k) is the number of partitions of k into parts
 *            1,2,3,4 (p(k)=0 for k<0). The function is memoised with a static
 *            table of the partition numbers up to the maximum n requested so
 *            far
 * Range:     u8 n <= 26, u16 n <= 168, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *f = NULL;   /* table of partitions into 1..4 */
static idx_t max_n = 0;   /* highest n for which f[] is valid */

/* Re‑compute the table f[0..n] if a larger n is required. */
static void recompute_f(idx_t n)
{
    if (f != NULL && n <= max_n)          /* enough data already stored */
        return;

    /* Allocate a new table large enough for 0..n. */
    size_t sz = (size_t)(n + 1);
    seq_t *new_f = (seq_t *)malloc(sz * sizeof(seq_t));
    if (!new_f) { /* memory allocation failed */
        free(f);
        f = NULL;
        max_n = 0;
        return;
    }

    /* Initialise and fill the DP table. */
    for (size_t i = 0; i <= sz - 1; ++i)
        new_f[i] = 0;
    new_f[0] = 1;                 /* one way to partition 0 */

    for (idx_t part = 1; part <= 4; ++part)
        for (idx_t i = part; i <= n; ++i)
            new_f[i] += new_f[i - part];

    /* Replace the old table. */
    free(f);
    f = new_f;
    max_n = n;
}

/* Main function requested by the problem. */
seq_t a(idx_t n)
{
    recompute_f(n);
    seq_t res = f[n];
    if (n >= 12)
        res += f[n - 12];
    return res;
}
