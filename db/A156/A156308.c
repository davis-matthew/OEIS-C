/*
 * A156308  Inverse of triangle S(n,m) defined by sequence A156290, n >= 1, 1
 *          <= m <= n.
 * https://oeis.org/A156308
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) = (r/c) * C(r+c-1, 2c-1) * where (r,c) is the row and
 *            column of the n‑th entry of the triangle * S(n,m) = (n/m) *
 *            binomial(n+m-1, 2m-1) (OEIS A156290)
 * Range:     u8 n <= 23, u16 n <= 83, u32 n <= 309, u64 n <= 1194,
 *            u128 n <= 1275, big any n
 * Helpers:   rc_of_index
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/rc_of_index.h"

#define MAX_N 20000            /* maximum N = r + c - 1 for n ≤ 10000 */

/* Pre‑computed smallest prime factor table for 1 … MAX_N */
static int spf[MAX_N + 1];
static bool spf_init = false;

static void init_spf(void)
{
    if (spf_init)
        return;
    for (int i = 0; i <= MAX_N; ++i)
        spf[i] = i;
    for (int i = 2; i * i <= MAX_N; ++i) {
        if (spf[i] == i) {
            for (int j = i * i; j <= MAX_N; j += i)
                if (spf[j] == j)
                    spf[j] = i;
        }
    }
    spf_init = true;
}

/* Factorize x (x > 1) into primes using the spf table and adjust exponents
   stored in exp[ ].  sign = +1 for numerator, –1 for denominator. */
static void add_factor(idx_t x, int sign, int exp[ ])
{
    if (x <= 1)
        return;
    while (x > 1) {
        int p = spf[x];
        int cnt = 0;
        while (x % p == 0) {
            x /= p;
            ++cnt;
        }
        exp[p] += sign * cnt;
    }
}

/* ------------------------------------------------------------------ */

seq_t a(idx_t n)
{
    init_spf();

    idx_t r, c;
    rc_of_index(n, &r, &c);

    idx_t N = r + c - 1;
    idx_t K = 2 * c - 1;          /* upper limit for the binomial */

    int exp[MAX_N + 1];
    memset(exp, 0, sizeof(exp));

    /* numerator factors of C(N,K) */
    for (idx_t i = 1; i <= K; ++i)
        add_factor(N + 1 - i, +1, exp);

    /* denominator factors of C(N,K) */
    for (idx_t i = 1; i <= K; ++i)
        add_factor(i, -1, exp);

    /* extra factors for r/m */
    add_factor(r, +1, exp);
    add_factor(c, -1, exp);

    /* build the result from the exponent array */
    seq_t result = 1;
    for (int p = 2; p <= MAX_N; ++p) {
        int e = exp[p];
        if (e > 0) {
            for (int k = 0; k < e; ++k) {
                /* use 128‑bit intermediary to avoid overflow before cast */
                result = (seq_t)((unsigned __int128)result * p);
            }
        } else if (e < 0) {
            /* theoretically should not happen: cancel during factorisation */
            for (int k = 0; k < -e; ++k)
                result /= (seq_t)p;
        }
    }
    return result;
}
