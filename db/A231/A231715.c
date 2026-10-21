/*
 * A231715  For n with a unique factorial base representation n = du*u! + ...
 *          + d2*2! + d1*1! (each di in range 0..i, cf. A007623), a(n) =
 *          Product_{i=1..u} (gcd(d_i,i+1) mod i+1), where u is given by
 *          A084558(n).
 * https://oeis.org/A231715
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Π_{i=1..u} (gcd(d_i,i+1) mod (i+1)) where d_i are the
 *            factorial‑base digits of n
 * Range:     u8 n <= 9719, u16 n <= 9719, u32 n <= 9719, u64 n <= 9719,
 *            u128 n <= 9719, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

static idx_t factorials[21];
static int factorials_init = 0;

static void init_factorials(void) {
    factorials[0] = 1;
    for (int i = 1; i < 21; ++i) {
        factorials[i] = factorials[i-1] * (idx_t)i;
    }
    factorials_init = 1;
}

seq_t a(idx_t n) {
    if (!factorials_init) init_factorials();
    if (n == 0) return 0;          /* offset starts at 1, but defined for n≥1 */
    idx_t temp = n;

    /* Find the largest u such that u! ≤ n */
    int max_i = 0;
    for (int i = 1; factorials[i] <= temp; ++i) {
        max_i = i;
    }

    seq_t prod = 1;
    for (int i = max_i; i >= 1; --i) {
        idx_t fac = factorials[i];
        idx_t digit = temp / fac;
        temp %= fac;
        seq_t mod = gcd((seq_t)digit, (seq_t)(i+1)) % (seq_t)(i+1);
        prod *= mod;
    }
    return prod;
}
