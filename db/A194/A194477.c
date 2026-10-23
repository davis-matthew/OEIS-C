/*
 * A194477  Number of ways to arrange 5 indistinguishable points on an n X n X
 *          n triangular grid so that no three points are in the same row or
 *          diagonal.
 * https://oeis.org/A194477
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/3840)*n^10 + (1/768)*n^9 - (7/384)*n^8 + (37/1920)*n^7
 *            * + (737/3840)*n^6 - (2347/3840)*n^5 + (101/192)*n^4 +
 *            (93/320)*n^3 * - (7/10)*n^2 + (3/10)*n
 * Range:     u8 none, u16 n <= 3, u32 n <= 9, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* Convert index to the sequence type before any arithmetic. */
    seq_t sn = (seq_t)n;

    /* Pre-compute powers of n up to n^10. */
    seq_t pow[11];
    pow[0] = (seq_t)1;
    for (idx_t i = 1; i <= 10; ++i)
        pow[i] = pow[i - 1] * sn;

    /* Accumulate absolute contributions to avoid underflow. */
    seq_t pos_sum = (seq_t)0;
    seq_t neg_sum = (seq_t)0;

    /* Each term: integer numerator after multiplying by 3840. */
    struct term { long long coeff; int exp; };
    static const struct term terms[] = {
        {   1LL, 10 },
        {   5LL,  9 },
        {  -70LL,  8 },
        {   74LL,  7 },
        {  737LL,  6 },
        { -2347LL, 5 },
        {  2020LL, 4 },
        {  1116LL, 3 },
        { -2688LL, 2 },
        {  1152LL, 1 }
    };
    const size_t nc_terms = sizeof(terms) / sizeof(terms[0]);

    for (size_t i = 0; i < nc_terms; ++i) {
        long long c = terms[i].coeff;
        int e = terms[i].exp;
        seq_t term_val = pow[e] * (seq_t)(c >= 0 ? c : -c);
        if (c >= 0)
            pos_sum += term_val;
        else
            neg_sum += term_val;
    }

    /* All intermediate values are exact; the final division yields an integer. */
    seq_t diff = pos_sum - neg_sum;
    return diff / (seq_t)3840;
}
