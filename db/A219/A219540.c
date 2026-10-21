/*
 * A219540  a(n) = (n^2 + 2*(Sum_{j = 1..n} j^n)) (mod n^3).
 * https://oeis.org/A219540
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n^2 + 2 * sum_{j=1..n} j^n) mod n^3
 * Range:     u8 n <= 5, u16 n <= 32, u32 n <= 40, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t mod_pow_local(seq_t base, idx_t exp, seq_t mod) {
    seq_t result = 1;
    base %= mod;
    while (exp) {
        if (exp & 1) {
            result = (result * base) % mod;
        }
        exp >>= 1;
        if (exp) {
            base = (base * base) % mod;
        }
    }
    return result;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;

    seq_t n_seq = (seq_t)n;
    seq_t mod = n_seq * n_seq * n_seq;   /* n^3 */

    /* Compute the sum Σ j^n mod n^3 */
    seq_t sum = 0;
    for (idx_t j = 1; j <= n; ++j) {
        seq_t term = mod_pow_local((seq_t)j, n, mod);
        sum += term;
        if (sum >= mod) sum -= mod;     /* Keep sum < 2*mod */
    }

    /* Final result: (n^2 + 2*sum) mod n^3 */
    seq_t n2 = n_seq * n_seq;            /* n^2 < mod */
    seq_t two_sum = sum << 1;            /* 2*sum < 2*mod */
    seq_t result = n2 + two_sum;
    if (result >= mod) result -= mod;   /* result < 3*mod, so at most one subtraction needed */
    if (result >= mod) result -= mod;
    return result;
}
