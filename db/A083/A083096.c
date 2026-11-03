/*
 * A083096  Numbers k such that 3 divides Sum_{j=1..k} binomial(2*j,j).
 * https://oeis.org/A083096
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 320, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Validated: standard, pass, 64 terms through n = 6347 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static inline int binom2_mod3(idx_t j)
{
    static const int C[3][3] = {
        {1,0,0},
        {1,1,0},
        {1,2,1}
    };
    idx_t two_j = j << 1;        /* 2*j */
    int prod = 1;
    while (j || two_j) {
        int j_digit  = j % 3;
        int two_digit = two_j % 3;
        if (j_digit > two_digit)
            return 0;
        prod = (prod * C[two_digit][j_digit]) % 3;
        j     /= 3;
        two_j /= 3;
    }
    return prod;
}

/* Test that C(2j,k) = C(2j,j) mod 3 can be computed by Lucas’ theorem.
   We generate the sequence by accumulating the partial sums modulo 3
   and outputting the k’s for which the sum is 0 modulo 3. */

seq_t a(idx_t n)
{
    if (n == 1)            /* a(1) = 0 */
        return 0;

    idx_t desired = n;      /* we need the n‑th term, counting a(1)=0 */
    idx_t count   = 1;      /* already have the first term */
    int   sum_mod = 0;      /* partial sum modulo 3, starts at 0 (k=0) */

    for (idx_t k = 1; ; ++k) {
        int b = binom2_mod3(k);
        sum_mod += b;
        if (sum_mod >= 3) sum_mod -= 3;

        if (sum_mod == 0) {
            ++count;
            if (count == desired)
                return (seq_t)k;
        }
    }
    /* unreachable, but keeps the compiler quiet */
    return 0;
}
