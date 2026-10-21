/*
 * A068963  a(n) = Sum_{d|n} phi(d^3).
 * https://oeis.org/A068963
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, mult
 * Formula:   a(n) = Sum_{d|n} d^2 * φ(d) (equivalently, Sum_{d|n} φ(d^3))
 * Range:     u8 n <= 6, u16 n <= 40, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   factor_num
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/factor_num.h"

static void dfs_div(int idx, seq_t cur, seq_t phi_cur,
                    const idx_t *pr, const int *exp, int cnt, seq_t *sum)
{
    if (idx == cnt) {
        *sum += cur * cur * phi_cur;
        return;
    }

    idx_t p = pr[idx];
    int max_e = exp[idx];

    /* case e = 0: prime not present in this divisor */
    dfs_div(idx + 1, cur, phi_cur, pr, exp, cnt, sum);

    seq_t p_pow = 1;      /* p^e */
    seq_t cur_local = cur;
    for (int e = 1; e <= max_e; e++) {
        p_pow *= p;                           /* p^e */
        cur_local = cur * p_pow;              /* current divisor */
        seq_t phi_pe = p_pow - p_pow / p;    /* φ(p^e) = p^e – p^{e-1} */
        seq_t new_phi = phi_cur * phi_pe;     /* φ(cur * p^e) = φ(cur)·φ(p^e) because coprime */
        dfs_div(idx + 1, cur_local, new_phi, pr, exp, cnt, sum);
    }
}

seq_t a(idx_t n)
{
    idx_t primes[32];
    int exps[32];
    int cnt = 0;

    factor_num(n, primes, exps, &cnt);

    seq_t total = 0;
    dfs_div(0, (seq_t)1, (seq_t)1, primes, exps, cnt, &total);
    return total;
}
