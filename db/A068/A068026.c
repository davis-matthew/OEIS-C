/*
 * A068026  Z(S_m; sigma[1](n), sigma[2](n),..., sigma[m](n)) where Z(S_m;
 *          x_1,x_2,...,x_m) is the cycle index of the symmetric group S_m and
 *          sigma[k](n) is the sum of k-th powers of divisors of n; m=9.
 * https://oeis.org/A068026
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 2, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   pow_uint32, sigma_k
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 *  Formula: a(n) = (1/9!) * sum over all partitions λ of 9
 *                of product_{i=1..9} sigma_i(n)^{λ_i}
 *                * (9! / (i^{λ_i} λ_i!))
 *  where sigma_k(n) is the sum of the k‑th powers of the divisors of n.
 *
 *  This implementation builds all partitions of 9, precomputes the needed
 *  powers of sigma_k, and accumulates the term
 *  P = sum (product * coeff).  Finally a(n) = P / 9!.
 */

#include <stdint.h>
#include "../utils/sigma_k.h"
#include "../utils/pow_uint32.h"

static /* factorials up to 9! */ const unsigned int fact[10] = {
    1,1,2,6,24,120,720,5040,40320,362880  /* 0! .. 9!  */
};

/* forward declaration of recursive generator */
static void gen_partitions(int idx, int rem, int *c,
                           const seq_t pow_sigma[10][10],
                           seq_t *P);

/* recursive enumeration of partitions of length 9 */
static void gen_partitions(int idx, int rem, int *c,
                           const seq_t pow_sigma[10][10],
                           seq_t *P)
{
    if (idx == 10) { /* processed lengths 1..9 */
        if (rem != 0) return;
        /* process the partition stored in c[1..9] */
        unsigned int denom = 1;
        for (int i = 1; i <= 9; ++i) {
            int cnt = c[i];
            if (cnt) {
                denom *= pow_uint32((unsigned int)i, (unsigned int)cnt);
                denom *= fact[cnt];
            }
        }
        unsigned int coeff = fact[9] / denom;  /* 9! / denom */

        seq_t prod = 1;
        for (int i = 1; i <= 9; ++i) {
            int cnt = c[i];
            if (cnt) prod *= pow_sigma[i][cnt];
        }

        *P += prod * coeff;
        return;
    }

    for (int cnt = 0; cnt <= rem / idx; ++cnt) {
        c[idx] = cnt;
        gen_partitions(idx + 1, rem - cnt * idx, c, pow_sigma, P);
    }
}

/* public sequence generator */
seq_t a(idx_t n)
{
    /* pre‑compute sigma_k(n) for k = 1..9 */
    seq_t sigma[10];
    for (int k = 1; k <= 9; ++k)
        sigma[k] = sigma_k(n, (uint8_t)k);

    /* pre‑compute powers of sigma: pow_sigma[i][e] = sigma[i]^e */
    seq_t pow_sigma[10][10];
    for (int i = 1; i <= 9; ++i) {
        pow_sigma[i][0] = 1;
        for (int e = 1; e <= 9; ++e)
            pow_sigma[i][e] = pow_sigma[i][e - 1] * sigma[i];
    }

    /* accumulate P = sum (product * coeff) over all partitions of 9 */
    seq_t P = 0;
    int c[10] = {0};
    gen_partitions(1, 9, c, pow_sigma, &P);

    /* final result a(n) = P / 9! */
    return P / fact[9];
}
