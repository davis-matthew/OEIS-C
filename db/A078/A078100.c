/*
 * A078100  1/6 of the number of ways of 3-coloring a 4 X n grid.
 * https://oeis.org/A078100
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   * a(n) = 9*a(n-1) - 15*a(n-2) + 6*a(n-3) * with a(1)=4, a(2)=27,
 *            a(3)=187 * The sequence starts at offset 1 – a(1) is the first
 *            term. * This recurrence is derived from the rational generating
 *            function * A(x) = x*(9*x-4-4*x^2) / (6*x^3-15*x^2+9*x-1)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 23, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* for size_t, if needed */

static seq_t cache[20001]; /* supports n up to 20000 (adjust if larger n are needed) */
static idx_t max_computed = 0;

seq_t a(idx_t n)
{
    if (n < 1)                 /* offset starts at 1; below that sequence is undefined */
        return (seq_t)0;

    if (n <= max_computed)     /* already computed */
        return cache[n];

    /* Compute all missing terms up to n */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        if (i == 1)
            cache[i] = (seq_t)4;
        else if (i == 2)
            cache[i] = (seq_t)27;
        else if (i == 3)
            cache[i] = (seq_t)187;
        else {
            seq_t val = cache[i-1] * ((seq_t)9);   /* 9*a(i-1)   */
            val += cache[i-3] * ((seq_t)6);       /* +6*a(i-3)   */
            val -= cache[i-2] * ((seq_t)15);      /* -15*a(i-2)  */
            cache[i] = val;
        }
    }
    max_computed = n;
    return cache[n];
}
