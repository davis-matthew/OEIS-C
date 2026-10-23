/*
 * A152968  a(n) = A139251(n+1)/2.
 * https://oeis.org/A152968
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A139251(n+1)/2 Computed via the binary decomposition n =
 *            2^i + j (0 ≤ j < 2^i): a(n) = Σ_{k=0}^{2^i-j-1} 2^(pop(j+k)-k) *
 *            C(pop(j+k), k), except a(2^r-1) = 2^(r-1)
 * Range:     u8 n <= 64, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* small binomial implementation for 0 ≤ k ≤ n ≤ 63.
   Uses only integer arithmetic – works for any seq_t type. */
static seq_t binomial_small(idx_t n, idx_t k)
{
    if (k > n) return 0;
    if (k > n - k) k = n - k;
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i)
    {
        res = (res * (n - k + i)) / i;
    }
    return res;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    /* Special case: n = 2^r – 1 */
    idx_t np1 = n + 1;
    idx_t r = 63 - __builtin_clzll(np1);      /* floor(log2(np1)) */
    if (((idx_t)1 << r) == np1)
        return ((seq_t)1 << (r - 1));

    /* General case */
    idx_t i = 63 - __builtin_clzll(n);        /* floor(log2(n)) */
    idx_t base = (idx_t)1 << i;
    idx_t j = n - base;
    idx_t limit = base - j;                  /*k runs 0 … limit‑1*/

    seq_t sum = 0;
    for (idx_t k = 0; k < limit; ++k)
    {
        idx_t sum_jk = j + k;
        int w = __builtin_popcountll(sum_jk);
        int ki = (int)k;
        if (ki > w) continue;                /* binomial = 0 */

        seq_t bin = binomial_small((idx_t)w, (idx_t)ki);

        /* compute 2^(w-ki) safely (without shifting more than the width) */
        seq_t pow_term = 1;
        int exp = w - ki;
        for (int m = 0; m < exp; ++m)
            pow_term *= 2;

        sum += pow_term * bin;
    }
    return sum;
}
