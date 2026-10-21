/*
 * A223573  Number of nX5 0..2 arrays with antidiagonals unimodal.
 * https://oeis.org/A223573
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = U(min(n,5))^(max(n,5)-min(n,5)+1) * product{U(i)^2 ,
 *            i=1..(min(n,5)-1)} where U(z) = (z^4+6*z^3+23*z^2+18*z+24)/24.
 *            For n>4, a(n) = 86 * a(n-1)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 9, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Helper: fast exponentiation for seq_t with idx_t exponent. */
static seq_t pow_seq(seq_t base, idx_t exp) {
    seq_t result = 1;
    while (exp > 0) {
        if (exp & 1)
            result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n) {
    /* U values for z = 1..5 */
    static const seq_t U[6] = {0, 3, 9, 22, 46, 86};

    if (n == 0) /* undefined for the offset 1; return 0 */
        return 0;

    /* Handle n <= 5 directly */
    if (n <= 5) {
        seq_t prod = 1;
        for (idx_t i = 1; i < n; ++i)
            prod *= U[i] * U[i];  /* product of U(i)^2 */
        seq_t base = U[n];
        idx_t exp = 6 - n;     /* min(n,5) = n, max(n,5) = 5 */
        seq_t power = pow_seq(base, exp);
        return prod * power;
    }

    /* For n > 5 use a(n) = 86^(n-4) * a(4) */
    static seq_t a4 = 0;
    static bool init_a4 = false;
    if (!init_a4) {
        a4 = 1;
        for (idx_t i = 1; i <= 4; ++i)
            a4 *= U[i] * U[i];  /* product of U(i)^2 i=1..4 */
        init_a4 = true;
    }

    seq_t pow86 = pow_seq(U[5], n - 4); /* U[5] = 86 */
    return a4 * pow86;
}
