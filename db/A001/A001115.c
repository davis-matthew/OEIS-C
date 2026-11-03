/*
 * A001115  Maximal number of pairwise relatively prime polynomials of degree
 *          n over GF(2).
 * https://oeis.org/A001115
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = P(n) + Sum_{i=1..floor(n/2)} P(i), where P(n) =
 *            A001037(n) = (1/n)*Sum_{d|n} mu(n/d)*2^d, a(0) = 1
 * Range:     u8 n <= 11, u16 n <= 20, u32 n <= 37, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Validated: partial, pass, 41 terms through n = 40 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static int a1115_mu(uint64_t m) {
    int s = 1;
    uint64_t p;
    for (p = 2; p * p <= m; p++) {
        if (m % p == 0) {
            m /= p;
            if (m % p == 0) return 0;
            s = -s;
        }
    }
    if (m > 1) s = -s;
    return s;
}

/* Arbitrary precision: exact sum then exact division */
static void a1115_big_term(idx_t n, idx_t d, seq_t *pos, seq_t *neg) {
    int m = a1115_mu(n / d);
    if (m == 0) return;
    seq_t pw = (seq_t)1;
    pw <<= d;
    if (m > 0) *pos += pw; else *neg += pw;
}

static seq_t a1115_P_big(idx_t n) {
    seq_t pos = 0, neg = 0;
    idx_t d;
    for (d = 1; d * d <= n; d++) {
        if (n % d == 0) {
            a1115_big_term(n, d, &pos, &neg);
            if (d != n / d) a1115_big_term(n, n / d, &pos, &neg);
        }
    }
    return (pos - neg) / (seq_t)n;
}

/* Fixed width: split 2^d = q*n + r, accumulate q modulo 2^w and r exactly */
static void a1115_fixed_term(idx_t n, idx_t d, seq_t *s, long long *rs) {
    int m = a1115_mu(n / d);
    if (m == 0) return;
    seq_t q = 0;
    uint64_t r = 0;
    idx_t i;
    for (i = 0; i <= d; i++) {
        r = r * 2 + (i == 0 ? 1 : 0);
        q = q + q;
        if (r >= n) {
            r -= n;
            q += 1;
        }
    }
    if (m > 0) {
        *s += q;
        *rs += (long long)r;
    } else {
        *s -= q;
        *rs -= (long long)r;
    }
}

static seq_t a1115_P_fixed(idx_t n) {
    seq_t s = 0;
    long long rs = 0;
    idx_t d;
    for (d = 1; d * d <= n; d++) {
        if (n % d == 0) {
            a1115_fixed_term(n, d, &s, &rs);
            if (d != n / d) a1115_fixed_term(n, n / d, &s, &rs);
        }
    }
    if (rs >= 0) s += (seq_t)(rs / (long long)n);
    else s -= (seq_t)((-rs) / (long long)n);
    return s;
}

static seq_t a1115_P(idx_t n, int big) {
    return big ? a1115_P_big(n) : a1115_P_fixed(n);
}

static int a1115_is_big(void) {
    seq_t x = 1;
    int bits = 0;
    while (x != 0 && bits < 300) {
        x = x + x;
        bits++;
    }
    return bits >= 300;
}

seq_t a(idx_t n) {
    if (n == 0) return 1;
    int big = a1115_is_big();
    seq_t sum = a1115_P(n, big);
    idx_t i;
    for (i = 1; i <= n / 2; i++) {
        sum += a1115_P(i, big);
    }
    return sum;
}
