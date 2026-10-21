/*
 * A224445  Numerators of certain rationals approximating sqrt(3).
 * https://oeis.org/A224445
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = numerator of r(n) where * r(n) = 2*(1 - 2*Σ_{k=1..n}
 *            Catalan(k-1)/4^(2*k)). * The numerators are kept in lowest terms
 *            as integers p(n) with * r(n) = p(n) / 2^e(n). r(0)=2, and for
 *            n≥1: * r(n) = r(n‑1) - Catalan(n‑1)/4^(2*n‑1). * Catalan numbers
 *            are generated incrementally
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache */
    static seq_t *nums = NULL;
    static idx_t *exps = NULL;
    static idx_t max_computed = 0;
    static idx_t capacity = 0;
    static seq_t catalan = 0;   /* Catalan(0) = 1 */
    static int initialized = 0;

    if (!initialized) {
        capacity = 1;
        nums = (seq_t*)malloc(sizeof(seq_t) * capacity);
        exps = (idx_t*)malloc(sizeof(idx_t) * capacity);
        nums[0] = (seq_t)2;          /* r(0) = 2/1 */
        exps[0] = (idx_t)0;
        catalan = (seq_t)1;           /* C_0 = 1 */
        max_computed = 0;
        initialized = 1;
    }

    if (n <= max_computed)
        return nums[n];

    /* grow arrays if needed */
    idx_t need = n + 1;
    if (need > capacity) {
        idx_t new_cap = capacity ? capacity : 1;
        while (new_cap < need)
            new_cap <<= 1;
        nums = (seq_t*)realloc(nums, sizeof(seq_t) * new_cap);
        exps = (idx_t*)realloc(exps, sizeof(idx_t) * new_cap);
        capacity = new_cap;
    }

    /* compute terms up to n */
    for (idx_t m = max_computed + 1; m <= n; ++m) {
        /* m corresponds to r(m).  Use Catalan(m-1). */
        idx_t k = m - 1;                 /* current Catalan index k */
        seq_t C_prev = catalan;          /* C_k */
        idx_t e_prev = exps[m - 1];
        seq_t p_prev = nums[m - 1];

        /* denominator exponent of the added term: 4^(2*m-1) = 2^(4*m-2) */
        idx_t t_den_exp = 4 * m - 2;

        /* common denominator exponent */
        idx_t e_cur   = (e_prev > t_den_exp) ? e_prev : t_den_exp;
        idx_t shift_p = e_cur - e_prev;
        idx_t shift_t = e_cur - t_den_exp;

        /* shift operands to common denominator */
        seq_t p_shifted = p_prev << shift_p;
        seq_t t_shifted = C_prev << shift_t;

        seq_t p_new = p_shifted - t_shifted;

        /* remove powers of 2 to keep lowest terms */
        idx_t tz = 0;
        while ((p_new & (seq_t)1) == 0) {
            p_new >>= 1;
            ++tz;
        }
        e_cur -= tz;

        nums[m] = p_new;
        exps[m] = e_cur;

        /* update Catalan to next value: C_{k+1} = C_k * 2*(2k+1)/(k+2) */
        seq_t tmp = C_prev * ( (seq_t)(4 * k + 2) );
        seq_t nextCatalan = tmp / (k + 2);
        catalan = nextCatalan;

        max_computed = m;
    }

    return nums[n];
}
