/*
 * A056349  Number of primitive (period n) bracelets using exactly three
 *          different colored beads.
 * https://oeis.org/A056349
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{d|n} μ(d) · ( B(3,n/d) – 3·B(2,n/d) + 3 ) where B(k,m)
 *            is the number of bracelets of length m with at most k colors.
 *            The implementation uses the standard Polya‑Burnside formula for
 *            B(k,m)
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   mu_value
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/mu_value.h"

/* ---------- helper: Euler φ(n) ----------------------------------- */
static seq_t phi_value(idx_t n)
{
    seq_t result = n;
    for (idx_t p = 2; p * p <= n; ++p)
        if (n % p == 0) {
            while (n % p == 0) n /= p;
            result -= result / p;
        }
    if (n > 1) result -= result / n;
    return result;
}

/* ---------- helper: power (base^exp) with big integers ----------- */
static seq_t pow_seq_t(seq_t base, idx_t exp)
{
    seq_t result = 1;
    while (exp) {
        if (exp & 1) result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

/* ---------- number of bracelets with at most k colors ------------- */
static seq_t bracelet_k(idx_t n, seq_t k)
{
    seq_t sum = 0;
    for (idx_t d = 1; d * d <= n; ++d) if (n % d == 0) {
        seq_t phi_d = phi_value(d);
        sum += phi_d * pow_seq_t(k, n / d);
        idx_t e = n / d;
        if (e != d) {
            seq_t phi_e = phi_value(e);
            sum += phi_e * pow_seq_t(k, n / e);
        }
    }

    seq_t t;
    if (n & 1) /* odd n */
        t = (seq_t)n * pow_seq_t(k, (n + 1) / 2);
    else { /* even n */
        t = (seq_t)(n / 2) *
            (pow_seq_t(k, n / 2 + 1) + pow_seq_t(k, n / 2));
    }

    return (sum + t) / (seq_t)(2 * n);
}

/* ---------- cache for A056343 ----------------------------------- */
static seq_t *A343_cache   = NULL;
static unsigned char *A343_valid = NULL;
static size_t A343_cap   = 0;

static void ensure_A343_cache(idx_t n)
{
    if (n < A343_cap) return;
    size_t newcap = (A343_cap > 0) ? A343_cap : 128;
    while (newcap <= (size_t)n) newcap <<= 1;
    A343_cache   = (seq_t*)realloc(A343_cache,   newcap * sizeof(seq_t));
    A343_valid  = (unsigned char*)realloc(A343_valid, newcap * sizeof(unsigned char));
    memset(A343_cache + A343_cap, 0,            (newcap - A343_cap) * sizeof(seq_t));
    memset(A343_valid + A343_cap, 0,            (newcap - A343_cap) * sizeof(unsigned char));
    A343_cap = newcap;
}

static seq_t A056343(idx_t n)
{
    if (n == 0) return 0;
    ensure_A343_cache(n);
    if (A343_valid[n]) return A343_cache[n];

    seq_t B3 = bracelet_k(n, 3);
    seq_t B2 = bracelet_k(n, 2);
    seq_t val = B3 - (seq_t)3 * B2 + (seq_t)3;
    A343_cache[n]     = val;
    A343_valid[n]     = 1;
    return val;
}

/* ---------- main sequence function -------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset starts at 1 */

    seq_t pos = 0;
    seq_t neg = 0;

    for (idx_t d = 1; d * d <= n; ++d) if (n % d == 0) {
        int mu_d = mu_value(d);
        if (mu_d != 0) {
            seq_t sub = A056343(n / d);
            if (mu_d > 0) pos += sub;
            else          neg += sub;
        }
        idx_t e = n / d;
        if (e != d) {
            int mu_e = mu_value(e);
            if (mu_e != 0) {
                seq_t sub = A056343(n / e);
                if (mu_e > 0) pos += sub;
                else          neg += sub;
            }
        }
    }

    /* pos is guaranteed >= neg for this sequence */
    return pos - neg;
}
