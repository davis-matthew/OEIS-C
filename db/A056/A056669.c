/*
 * A056669  Number of equivalence classes of n-valued Post functions of 3
 *          variables under action of symmetric group S_3.
 * https://oeis.org/A056669
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n^{n^3} + 3 * n^{n^2(n+1)/2} + 2 * n^{(n^3+2n)/3}) / 6
 * Range:     u8 none, u16 n <= 2, u32 n <= 2, u64 n <= 3, u128 n <= 3,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Helper: exponentiation by squaring on seq_t */
static seq_t pow_seq(seq_t base, idx_t exp)
{
    seq_t result = 1;
    while (exp)
    {
        if (exp & 1ULL)
            result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    if (n < 2)   /* offset is 2; behavior below is irrelevant */
        return 0;

    seq_t n_seq = n;

    /* n^{n^3} */
    seq_t term1 = pow_seq(n_seq, n * n * n);

    /* n^{n^2(n+1)/2} */
    seq_t term2 = pow_seq(n_seq, (n * n * (n + 1)) / 2);

    /* n^{(n^3 + 2n)/3} */
    seq_t term3 = pow_seq(n_seq, (n * n * n + 2 * n) / 3);

    seq_t sum = term1 + 3 * term2 + 2 * term3;
    return sum / 6;
}
