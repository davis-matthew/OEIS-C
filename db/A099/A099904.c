/*
 * A099904  Numerator of sum of all matrix elements of N X N matrix M(i,j) =
 *          i^3+j^3, (i,j = 1..n) divided by n!.
 * https://oeis.org/A099904
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Numerator[(n^3*(n+1)^2)/(2*n!)]
 * Range:     u8 n <= 9, u16 n <= 57, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   exp_in_factorial, factor_uint64, pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow_seq.h"
#include "../utils/exp_in_factorial.h"
#include "../utils/factor_uint64.h"

/* sequence value */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* offset starts at 1 */

    seq_t result = 1;

    /* factor n */
    uint64_t fac_n[20];
    int exp_n[20];
    int cnt_n;
    factor_uint64((uint64_t)n, fac_n, exp_n, &cnt_n);

    for (int i = 0; i < cnt_n; ++i) {
        uint64_t p   = fac_n[i];
        int exp_N    = 3 * exp_n[i];                       /* exponent in n^3 */
        int exp_D    = exp_in_factorial((uint64_t)n, p) + (p == 2 ? 1 : 0); /* exponent in 2*n! */
        int exp_res  = exp_N - exp_D;
        if (exp_res > 0)
            result *= pow_seq((seq_t)p, exp_res);
    }

    /* factor n+1 */
    uint64_t fac_np1[20];
    int exp_np1[20];
    int cnt_np1;
    factor_uint64((uint64_t)(n + 1), fac_np1, exp_np1, &cnt_np1);

    for (int i = 0; i < cnt_np1; ++i) {
        uint64_t p   = fac_np1[i];
        int exp_N    = 2 * exp_np1[i];                     /* exponent in (n+1)^2 */
        int exp_D    = exp_in_factorial((uint64_t)n, p) + (p == 2 ? 1 : 0);
        int exp_res  = exp_N - exp_D;
        if (exp_res > 0)
            result *= pow_seq((seq_t)p, exp_res);
    }

    return result;
}
