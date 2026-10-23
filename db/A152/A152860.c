/*
 * A152860  Scaled row sum zero vector recursion:s=3;
 *          v(n)={s^(n+1),s^(n+1)-Sum[s^i,{i,2,n}],s^n,...,-1}.
 * https://oeis.org/A152860
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) is the n-th element in the concatenation of the vectors *
 *            v(k) defined for s = 3: * v(0) = {1} * for k ≥ 1: * v[0] =
 *            s^(k-1) * v[1] = -(s^(k-1) + 1) / 2 * v[j] = -s^(k-j) for j ≥ 2
 *            * * The sequence a(n) is obtained by listing v(0), v(1), v(2), …
 *            in order
 * Range:     i8 n <= 20, i16 n <= 55, i32 n <= 55, i64 n <= 55, i128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <math.h>

static seq_t pow_seq_int(seq_t base, idx_t exp)
{
    seq_t result = 1;
    while (exp > 0)
    {
        if (exp & 1) result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    /* Find the block k such that n ∈ [k(k+1)/2 , (k+1)(k+2)/2) */
    idx_t k = (idx_t)((sqrt((long double)(1 + 8.0L * n)) - 1.0L) / 2.0L);
    idx_t tri_prev = k * (k + 1) / 2;          /* k(k+1)/2 */
    idx_t j = n - tri_prev;                    /* position within v(k) */

    if (k == 0)               /* the special vector {1} */
        return 1;

    /* For k ≥ 1 */
    if (j == 0) {                 /* first element: s^(k-1) */
        return pow_seq_int((seq_t)3, k - 1);
    }
    if (j == 1) {                 /* second element: -(s^(k-1)+1)/2 */
        seq_t p = pow_seq_int((seq_t)3, k - 1);
        return -(p + 1) / 2;
    }
    /* j ≥ 2: v[j] = -s^(k-j) */
    idx_t exp = k - j;
    seq_t p = pow_seq_int((seq_t)3, exp);
    return -p;
}
