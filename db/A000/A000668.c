/*
 * A000668  Mersenne primes (primes of the form 2^n - 1).
 * https://oeis.org/A000668
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, hard
 * Formula:   a(n) = 2^A000043(n) - 1; finds the exponents by searching primes
 *            p, trial-factoring 2^p-1 and then running the Lucas-Lehmer test
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 8, u64 n <= 9, u128 n <= 12,
 *            big any n
 * Validated: standard, pass, 18 terms through n = 18 (2026-11-01)
 * Helpers:   is_prime_u32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/is_prime_u32.h"

static uint64_t pow2_mod(unsigned e, uint64_t q) {
    uint64_t r = 1 % q, b = 2 % q;
    while (e) {
        if (e & 1) r = r * b % q;
        b = b * b % q;
        e >>= 1;
    }
    return r;
}

/* returns 1 if a proper factor of 2^p-1 is found (p odd prime) */
static int has_small_factor(unsigned p) {
    uint64_t k;
    for (k = 1; k <= 20000; k++) {
        uint64_t q = 2 * k * (uint64_t)p + 1;
        if (q >= ((uint64_t)1 << 32)) break;
        if ((q & 7) != 1 && (q & 7) != 7) continue;
        if (pow2_mod(p, q) == 1) {
            if (p < 63 && q == (((uint64_t)1 << p) - 1)) return 0;
            return 1;
        }
    }
    return 0;
}

static int lucas_lehmer(unsigned p) {
    int L, w, b, i, j, k, prime;
    unsigned it;
    uint64_t *s, *t, *M, topmask;
    if (p == 2) return 1;
    L = (int)((p + 63) / 64);
    w = (int)(p / 64);
    b = (int)(p % 64);
    topmask = b ? ((((uint64_t)1) << b) - 1) : ~(uint64_t)0;
    s = (uint64_t *)calloc((size_t)L, sizeof(uint64_t));
    t = (uint64_t *)calloc((size_t)(2 * L + 2), sizeof(uint64_t));
    M = (uint64_t *)calloc((size_t)L, sizeof(uint64_t));
    for (i = 0; i < L; i++) M[i] = ~(uint64_t)0;
    M[L - 1] = topmask;
    s[0] = 4;
    for (it = 0; it + 2 < p; it++) {
        int topset;
        memset(t, 0, sizeof(uint64_t) * (size_t)(2 * L + 2));
        for (i = 0; i < L; i++) {
            uint64_t carry = 0;
            if (s[i] == 0) continue;
            for (j = 0; j < L; j++) {
                unsigned __int128 cur = (unsigned __int128)s[i] * s[j] + t[i + j] + carry;
                t[i + j] = (uint64_t)cur;
                carry = (uint64_t)(cur >> 64);
            }
            t[i + L] = carry;
        }
        /* s = (t mod 2^p) + (t >> p) */
        {
            uint64_t carry = 0;
            for (k = 0; k < L; k++) {
                uint64_t lo = t[k], hi;
                unsigned __int128 sum;
                if (k == L - 1) lo &= topmask;
                if (b == 0) hi = t[k + w];
                else hi = (t[k + w] >> b) | (t[k + w + 1] << (64 - b));
                sum = (unsigned __int128)lo + hi + carry;
                s[k] = (uint64_t)sum;
                carry = (uint64_t)(sum >> 64);
            }
            /* fold any bits at position >= p */
            for (;;) {
                uint64_t over;
                if (b == 0) { over = carry; carry = 0; }
                else { over = s[L - 1] >> b; s[L - 1] &= topmask; }
                topset = over != 0;
                if (!topset) break;
                carry = 0;
                for (k = 0; k < L && over; k++) {
                    uint64_t old = s[k];
                    s[k] += over;
                    over = s[k] < old ? 1 : 0;
                }
                if (b == 0) carry = over;
            }
        }
        /* s -= 2 mod M */
        {
            int small = s[0] < 2;
            for (k = 1; k < L && small; k++)
                if (s[k]) small = 0;
            if (small) {
                uint64_t v = s[0];
                for (k = 0; k < L; k++) s[k] = M[k];
                s[0] -= (2 - v);
            } else {
                uint64_t borrow = 2;
                for (k = 0; k < L && borrow; k++) {
                    uint64_t old = s[k];
                    s[k] -= borrow;
                    borrow = old < borrow ? 1 : 0;
                }
            }
        }
    }
    prime = 1;
    for (k = 0; k < L; k++)
        if (s[k]) { prime = 0; break; }
    if (!prime) {
        prime = 1;
        for (k = 0; k < L; k++)
            if (s[k] != M[k]) { prime = 0; break; }
    }
    free(s);
    free(t);
    free(M);
    return prime;
}

static unsigned exps[128];
static idx_t found = 0;
static unsigned next_p = 2;

seq_t a(idx_t n) {
    unsigned p, i;
    seq_t r;
    while (found < n) {
        p = next_p++;
        if (!is_prime_u32(p)) continue;
        if (p > 2 && has_small_factor(p)) continue;
        if (lucas_lehmer(p)) exps[found++] = p;
    }
    p = exps[n - 1];
    r = 1;
    for (i = 0; i < p; i++) r += r;
    return r - 1;
}
