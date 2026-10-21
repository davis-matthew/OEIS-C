/*
 * A211060  Number of 2 X 2 matrices having all terms in {1,...,n} and
 *          determinant >n.
 * https://oeis.org/A211060
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 19, u32 n <= 305, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm for A211060: Count 2×2 integer matrices with entries in {1,…,n}
   whose determinant ad – bc exceeds n.
   We compute, for each product u = a·d, the number of pairs (b,c) with
   b·c ≤ u – n (using the prefix sums of product multiplicities). */
#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* maximum product value is n*n */
    uint32_t Nmax = (uint32_t)(n * n);      /* fits in 32 bits for n ≤ 10000 */
    size_t arr_size = (size_t)Nmax + 1;     /* array indices 0..Nmax */

    /* frequency array of length Nmax+1, initially zero */
    uint32_t *freq = (uint32_t *)calloc(arr_size, sizeof(uint32_t));
    if (!freq) return (seq_t)0;             /* allocation failure */

    /* Count occurrences of each product u = a*d (1 ≤ a,d ≤ n) */
    for (idx_t a = 1; a <= n; ++a)
        for (idx_t d = 1; d <= n; ++d)
            ++freq[(size_t)(a * d)];

    /* Convert freq to prefix sums: freq[i] = number of products ≤ i */
    for (uint32_t i = 1; i <= Nmax; ++i)
        freq[i] += freq[i - 1];

    /* Accumulate result: for each u = a*d, add number of (b,c) with b*c ≤ u−n */
    seq_t res = 0;
    for (idx_t a = 1; a <= n; ++a)
        for (idx_t d = 1; d <= n; ++d) {
            uint32_t prod = (uint32_t)(a * d);
            if (prod > n) {
                uint32_t idx = prod - n - 1;   /* v ≤ prod−n−1 */
                res += (seq_t)freq[idx];
            }
        }

    free(freq);
    return res;
}
