/*
 * A007247  McKay-Thompson series of class 4B for the Monster group.
 * https://oeis.org/A007247
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{i=0..n} s(i)*s(n-i), where Sum s(n) x^n = E(x) /
 *            (eta(x)*eta(x^2))^2 (q-power factor dropped), E(x) = 1 +
 *            24*Sum_{m>=1} sigma_odd(m) x^m (sigma_odd = sum of odd
 *            divisors), i.e. a(n) are the coefficients of 4*q^(1/2)*(k'^4 +
 *            4*k^2)/(k'^2*k), which equals x^(-1/2)*(E(x)/(eta(x)*eta(x^2)...
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 16, u64 n <= 59, u128 n <= 221,
 *            big any n
 * Helpers:   alloc_zero, release, seq_is_fixed_width
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/alloc_zero.h"
#include "../utils/release.h"
#include "../utils/seq_is_fixed_width.h"

static seq_t *S_tab = 0;
static idx_t S_len = 0;

static void build_S(idx_t len)
{
    uint64_t *so = (uint64_t *)calloc((size_t)len, sizeof(uint64_t));
    uint64_t *c = (uint64_t *)calloc((size_t)len, sizeof(uint64_t));
    seq_t *q = alloc_zero(len);
    seq_t *s = alloc_zero(len);
    idx_t d, k, i, m;

    for (d = 1; d < len; d++) {
        uint64_t bd = (d % 2 == 0) ? 4 : 2;
        for (k = d; k < len; k += d) {
            if (d % 2 == 1) so[k] += d;
            c[k] += d * bd;
        }
    }

    q[0] = 1;
    if (seq_is_fixed_width()) {
        /* multiply by 1/(1-x^m) b_m times; no division needed */
        for (m = 1; m < len; m++) {
            int reps = (m % 2 == 0) ? 4 : 2;
            int r;
            for (r = 0; r < reps; r++)
                for (i = m; i < len; i++)
                    q[i] += q[i - m];
        }
    } else {
        for (i = 1; i < len; i++) {
            seq_t acc = 0;
            for (k = 1; k <= i; k++)
                acc += q[i - k] * (seq_t)c[k];
            q[i] = acc / (seq_t)i;
        }
    }

    for (i = 0; i < len; i++) {
        seq_t acc = q[i];
        for (m = 1; m <= i; m++)
            acc += q[i - m] * (seq_t)(24 * so[m]);
        s[i] = acc;
    }

    free(so);
    free(c);
    release(q, len);

    if (S_tab) release(S_tab, S_len);
    S_tab = s;
    S_len = len;
}

seq_t a(idx_t n)
{
    idx_t i;
    seq_t r = 0;
    if (n >= S_len) {
        idx_t len = n + 1;
        if (len < 2 * S_len) len = 2 * S_len;
        if (len < 64) len = 64;
        build_S(len);
    }
    for (i = 0; 2 * i < n; i++)
        r += S_tab[i] * S_tab[n - i];
    r *= 2;
    if (n % 2 == 0)
        r += S_tab[n / 2] * S_tab[n / 2];
    return r;
}
