/*
 * A206243  In base n, you must write the first A206242(n) positive multiples
 *          of a(n) in order to write every base-n digit.
 * https://oeis.org/A206243
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = n^k/A079277(n), k least making the quotient an integer;
 *            computed via prime exponents
 * Range:     u8 n <= 13, u16 n <= 33, u32 n <= 81, u64 n <= 513,
 *            u128 n <= 4097, big any n
 * Validated: standard, pass, 74 terms through n = 9685 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

static idx_t A206243_A079277(idx_t n, idx_t *primes, int pcount)
{
    for (idx_t k = n - 1; k > 0; --k) {
        idx_t v = k;
        for (int i = 0; i < pcount; ++i) {
            idx_t p = primes[i];
            while (v % p == 0) v /= p;
        }
        if (v == 1) return k;
    }
    return 1;
}

seq_t a(idx_t n)
{
    if (n < 2) return (seq_t)0;
    idx_t primes[64];
    int exps[64];
    int pc = 0;
    idx_t tmp = n;
    for (idx_t p = 2; p <= tmp / p; ++p) {
        if (tmp % p == 0) {
            int e = 0;
            while (tmp % p == 0) { tmp /= p; ++e; }
            primes[pc] = p;
            exps[pc++] = e;
        }
    }
    if (tmp > 1) { primes[pc] = tmp; exps[pc++] = 1; }

    idx_t m = A206243_A079277(n, primes, pc);

    int f[64];
    int k = 0;
    for (int i = 0; i < pc; ++i) {
        int c = 0;
        while (m % primes[i] == 0) { m /= primes[i]; ++c; }
        f[i] = c;
        int need = (c + exps[i] - 1) / exps[i];
        if (need > k) k = need;
    }

    seq_t res = 1;
    for (int i = 0; i < pc; ++i) {
        int ex = exps[i] * k - f[i];
        for (int j = 0; j < ex; ++j) res *= (seq_t)primes[i];
    }
    return res;
}
