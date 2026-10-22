/*
 * A000331  Related to zeros of Bessel function.
 * https://oeis.org/A000331
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn
 * Formula:   Rayleigh functions sigma_n(nu) = Sum_k j_{nu,k}^(-2n) satisfy
 *            (nu+n)*sigma_n = Sum_{k=1..n-1} sigma_k*sigma_{n-k}, sigma_1 =
 *            1/(4(nu+1)). With P_n(nu) = sigma_n * 2^(2n) * Prod_{j=1..n}
 *            (nu+j)^floor(n/j): P_1 = 1, P_n = Sum_{k=1..n-1}
 *            P_k*P_{n-k}*Prod_{j<n, e_j=1} (nu+j), where e_j =
 *            floor(n/j)-f...
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 11, u64 n <= 15, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

#define A331_MAX 2048

static seq_t a331_c[A331_MAX + 1]; /* constant coefficient of P_n */
static seq_t a331_l[A331_MAX + 1]; /* linear coefficient of P_n */
static idx_t a331_done = 0;

static void a331_step(idx_t n)
{
    seq_t sc = 0, sl = 0;
    idx_t k, j;
    for (k = 1; k <= n - k; k++) {
        idx_t m = n - k;
        seq_t q0 = 1, q1 = 0;
        for (j = 2; j < n; j++) {
            if (n / j - k / j - m / j == 1) {
                q1 = q1 * (seq_t)j + q0;
                q0 = q0 * (seq_t)j;
            }
        }
        seq_t pc = a331_c[k] * a331_c[m];
        seq_t pl = a331_c[k] * a331_l[m] + a331_l[k] * a331_c[m];
        seq_t tc = pc * q0;
        seq_t tl = pc * q1 + pl * q0;
        if (k != m) {
            tc = tc * 2;
            tl = tl * 2;
        }
        sc += tc;
        sl += tl;
    }
    a331_c[n] = sc;
    a331_l[n] = sl;
}

seq_t a(idx_t n)
{
    if (n < 1 || n > A331_MAX) return 0;
    if (a331_done == 0) {
        a331_c[1] = 1;
        a331_l[1] = 0;
        a331_done = 1;
    }
    while (a331_done < n) {
        a331_done++;
        a331_step(a331_done);
    }
    return a331_l[n];
}
