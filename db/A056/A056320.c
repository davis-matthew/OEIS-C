/*
 * A056320  Number of primitive (aperiodic) reversible strings with n beads
 *          using exactly four different colors.
 * https://oeis.org/A056320
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{d|n} μ(d) * B(n/d) B(m) = (T(m) + Pal(m)) / 2 T(m) =
 *            Σ_{i=0}^4 (-1)^i * C(4,i) * (4-i)^m Pal(m) = { T(m/2), if m is
 *            even { 4*T((m-1)/2) + 4*T3((m-1)/2), if m is odd T3(x) =
 *            Σ_{i=0}^3 (-1)^i * C(3,i) * (3-i)^x
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 16, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   mu_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/mu_of.h"

/* Auxiliary: fast exponentiation for seq_t */
static inline seq_t pow_seq(seq_t base, idx_t exp) {
    seq_t result = 1;
    while (exp) {
        if (exp & 1) result = result * base;
        base = base * base;
        exp >>= 1;
    }
    return result;
}

/* Number of length exp strings over 4 colors that use all 4 colors */
static inline seq_t T4(idx_t exp) {
    static const int C[5] = {1,4,6,4,1};
    seq_t sum = 0;
    for (int i = 0; i <= 4; ++i) {
        seq_t term = pow_seq((seq_t)(4 - i), exp);
        if (i & 1) /* odd */ sum -= (seq_t)C[i] * term;
        else       /* even */ sum += (seq_t)C[i] * term;
    }
    return sum;
}

/* Number of length exp strings over 3 colors that use all 3 colors */
static inline seq_t T3(idx_t exp) {
    static const int C[4] = {1,3,3,1};
    seq_t sum = 0;
    for (int i = 0; i <= 3; ++i) {
        seq_t term = pow_seq((seq_t)(3 - i), exp);
        if (i & 1) sum -= (seq_t)C[i] * term;
        else       sum += (seq_t)C[i] * term;
    }
    return sum;
}

/* Number of palindromic strings of length n using all 4 colors */
static inline seq_t Pal(idx_t n) {
    if (n % 2 == 0) {          /* even n = 2m */
        idx_t m = n / 2;
        return T4(m);
    } else {                   /* odd n = 2m+1 */
        idx_t m = (n - 1) / 2;
        seq_t case1 = T4(m);   /* center in the set of 4 colors */
        seq_t case2 = T3(m);   /* center is the missing color */
        return 4 * case1 + 4 * case2;
    }
}

/* Number of reversible strings of length n using all 4 colors */
static inline seq_t B(idx_t n) {
    seq_t total = T4(n);
    seq_t pal   = Pal(n);
    return (total + pal) / 2;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;          /* offset starts at 1, but safe guard */

    seq_t sum_pos = 0;
    seq_t sum_neg = 0;

    /* iterate over all divisors d of n */
    idx_t sqrt_n = 0;
    while ((sqrt_n + 1) * (sqrt_n + 1) <= n) ++sqrt_n;
    for (idx_t i = 1; i <= sqrt_n; ++i) {
        if (n % i != 0) continue;
        idx_t d1 = i;
        idx_t d2 = n / i;

        int mu1 = mu_of(d1);
        if (mu1 == 1) sum_pos += B(n / d1);
        else if (mu1 == -1) sum_neg += B(n / d1);

        if (d2 != d1) {
            int mu2 = mu_of(d2);
            if (mu2 == 1) sum_pos += B(n / d2);
            else if (mu2 == -1) sum_neg += B(n / d2);
        }
    }

    if (sum_pos >= sum_neg) return sum_pos - sum_neg;
    else return 0;                 /* should not happen for this sequence */
}
