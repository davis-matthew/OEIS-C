/*
 * A075449  Numbers k such that A075443(k) == 0 (mod A000010(k)).
 * https://oeis.org/A075449
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th k >= 1 with A075443(k) == 0 (mod A000010(k)); found
 *            by incremental search over k, with A075443 summed directly from
 *            modular inverses
 * Range:     u8 n <= 19, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Validated: partial, pass, 54 terms through n = 54 (2026-11-07)
 * Helpers:   gcd_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/gcd_idx.h"

static idx_t A075449_inv(idx_t a, idx_t mod)
{
    int64_t t = 0, newt = 1;
    int64_t r = (int64_t)mod, newr = (int64_t)a;
    while (newr != 0) {
        int64_t q = r / newr;
        int64_t tmp = t - q * newt;
        t = newt; newt = tmp;
        tmp = r - q * newr;
        r = newr; newr = tmp;
    }
    if (t < 0) t += (int64_t)mod;
    return (idx_t)t;
}

static seq_t A075449_A075443(idx_t n)
{
    seq_t sum = 0;
    for (idx_t h = 1; h < n; ++h) {
        if (gcd_idx(h, n) != 1) continue;
        idx_t inv = A075449_inv(h, n);
        idx_t diff = (h > inv) ? h - inv : inv - h;
        sum += (seq_t)diff;
    }
    return sum / 2;
}

static seq_t A075449_phi(idx_t n)
{
    seq_t result = 1;
    idx_t m = n;
    idx_t p;
    for (p = 2; p <= m / p; p++) {
        if (m % p == 0) {
            m /= p;
            result *= (seq_t)(p - 1);
            while (m % p == 0) {
                m /= p;
                result *= (seq_t)p;
            }
        }
    }
    if (m > 1)
        result *= (seq_t)(m - 1);
    return result;
}

#define A075449_CACHE 4096
static idx_t A075449_terms[A075449_CACHE];
static idx_t A075449_count = 0;   /* terms found so far */
static idx_t A075449_next = 1;    /* next k to test */

seq_t a(idx_t n)
{
    if (n >= 1 && n <= A075449_count)
        return (seq_t)A075449_terms[n - 1];

    while (A075449_count < n) {
        idx_t k = A075449_next++;
        seq_t s = A075449_A075443(k);
        seq_t ph = A075449_phi(k);
        if (s % ph == 0) {
            if (A075449_count < A075449_CACHE)
                A075449_terms[A075449_count] = k;
            A075449_count++;
            if (A075449_count == n)
                return (seq_t)k;
        }
    }
    return 0;
}
