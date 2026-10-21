/*
 * A078427  Sum of all the decimal digits of numbers from 1 to 10^n.
 * https://oeis.org/A078427
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 45*n*10^(n-1) + 1 (a(1)=46)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 17, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* extern seq_t a(idx_t n) is defined in another translation unit */
seq_t a(idx_t n)
{
    /* The sequence is defined for n ≥ 1; return 0 for n = 0 as a guard. */
    if (n == 0) {
        return (seq_t)0;
    }

    /* Cache of powers of ten: pow10[i] = 10^i  for i ≥ 0. */
    static seq_t *pow10 = NULL;
    static idx_t pow10_len = 0;      /* number of elements stored, i.e. max i+1 */

    /* Initialise the cache on first use. */
    if (pow10_len == 0) {
        pow10_len = 1;
        pow10 = (seq_t *)malloc(pow10_len * sizeof(seq_t));
        pow10[0] = (seq_t)1;          /* 10^0 = 1 */
    }

    /* Extend the cache up to index (n-1). */
    while (pow10_len <= n - 1) {
        /* Allocate one more slot */
        seq_t *tmp = (seq_t *)realloc(pow10, (pow10_len + 1) * sizeof(seq_t));
        if (!tmp) {
            /* Allocation failed – abort (undefined behaviour for testing). */
            free(pow10);
            pow10 = NULL;
            pow10_len = 0;
            return (seq_t)0;
        }
        pow10 = tmp;
        /* Compute the new power of ten */
        pow10[pow10_len] = pow10[pow10_len - 1] * (seq_t)10;
        ++pow10_len;
    }

    /* Compute 45 * n * 10^(n-1) + 1 */
    seq_t factor = (seq_t)45 * (seq_t)n;
    return pow10[n - 1] * factor + (seq_t)1;
}
