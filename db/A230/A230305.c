/*
 * A230305  Iterate A080670 starting at n; a(n) = number of steps to reach a
 *          prime, or -1 if no prime is ever reached.
 * https://oeis.org/A230305
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, base, more, hard
 * Formula:   a(n) = number of iterations of A080670 (concatenate prime
 *            factors, exponents >1) from n to reach a prime; -1 if never.
 *            Computed by iteration with trial division + Pollard rho on
 *            128-bit values
 * Range:     u8 n <= 19, u16 n <= 19, u32 n <= 19, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Validated: partial, pass, 18 terms through n = 19 (2026-11-07)
 * Helpers:   gcd_u128, is_prime_u128, mulmod_u128
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime_u128.h"
#include "../utils/mulmod_u128.h"
#include "../utils/gcd_u128.h"

typedef unsigned __int128 A230305_u128;

static bool A230305_rho(A230305_u128 n, A230305_u128 *out)
{
    for (unsigned c = 1; c <= 4; ++c) {
        A230305_u128 x = 2, y = 2, d = 1;
        for (unsigned it = 0; it < 300000 && d == 1; ++it) {
            x = (mulmod_u128(x, x, n) + c) % n;
            y = (mulmod_u128(y, y, n) + c) % n;
            y = (mulmod_u128(y, y, n) + c) % n;
            A230305_u128 diff = x > y ? x - y : y - x;
            d = gcd_u128(diff, n);
        }
        if (d != 1 && d != n) { *out = d; return true; }
    }
    return false;
}

static bool A230305_factor_rec(A230305_u128 n, A230305_u128 *arr, int *cnt)
{
    if (n == 1) return true;
    if (is_prime_u128(n)) { arr[(*cnt)++] = n; return true; }
    A230305_u128 d;
    if (!A230305_rho(n, &d)) return false;
    return A230305_factor_rec(d, arr, cnt) && A230305_factor_rec(n / d, arr, cnt);
}

static bool A230305_append(A230305_u128 *res, A230305_u128 num)
{
    A230305_u128 p = 1, t = num;
    A230305_u128 lim = ~(A230305_u128)0 / 10 / 10;
    do {
        if (p > lim) return false;
        p *= 10; t /= 10;
    } while (t);
    if (*res > (~(A230305_u128)0 - num) / p) return false;
    *res = *res * p + num;
    return true;
}

static bool A230305_next(A230305_u128 v, A230305_u128 *out)
{
    A230305_u128 f[130];
    int cnt = 0;
    A230305_u128 m = v;
    for (A230305_u128 p = 2; p < 100000 && p * p <= m; p += (p == 2 ? 1 : 2)) {
        while (m % p == 0) { f[cnt++] = p; m /= p; }
    }
    if (m > 1) {
        if (!A230305_factor_rec(m, f, &cnt)) return false;
    }
    for (int i = 1; i < cnt; ++i) {
        A230305_u128 k = f[i];
        int j = i - 1;
        while (j >= 0 && f[j] > k) { f[j + 1] = f[j]; --j; }
        f[j + 1] = k;
    }
    A230305_u128 res = 0;
    int i = 0;
    while (i < cnt) {
        int j = i;
        while (j < cnt && f[j] == f[i]) ++j;
        if (!A230305_append(&res, f[i])) return false;
        if (j - i > 1 && !A230305_append(&res, (A230305_u128)(j - i))) return false;
        i = j;
    }
    *out = res;
    return true;
}

seq_t a(idx_t n)
{
    A230305_u128 v = (A230305_u128)n;
    for (int steps = 0; steps < 1000; ++steps) {
        if (is_prime_u128(v)) return (seq_t)steps;
        A230305_u128 nx;
        if (!A230305_next(v, &nx)) break;
        if (nx == v) break;
        v = nx;
    }
    return (seq_t)0 - 1;
}
