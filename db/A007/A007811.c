/*
 * A007811  Numbers k for which 10k+1, 10k+3, 10k+7 and 10k+9 are primes.
 * https://oeis.org/A007811
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   search over k = 21j + {1,10,19} (the only residues mod 21
 *            allowed by divisibility by 3 and 7), keeping k when 10k+1,
 *            10k+3, 10k+7, 10k+9 are all prime (Miller-Rabin); results
 *            memoized
 * Range:     u8 n <= 7, u16 n <= 124, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static uint64_t mulmod_u(uint64_t a, uint64_t b, uint64_t m) {
    return (uint64_t)((unsigned __int128)a * b % m);
}

static uint64_t powmod_u(uint64_t b, uint64_t e, uint64_t m) {
    uint64_t r = 1 % m;
    b %= m;
    while (e) {
        if (e & 1) r = mulmod_u(r, b, m);
        b = mulmod_u(b, b, m);
        e >>= 1;
    }
    return r;
}

static bool mr_prime(uint64_t n) {
    static const uint64_t bases[12] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37};
    int i;
    if (n < 2) return false;
    for (i = 0; i < 12; i++) {
        if (n == bases[i]) return true;
        if (n % bases[i] == 0) return false;
    }
    uint64_t d = n - 1;
    int s = 0;
    while ((d & 1) == 0) { d >>= 1; s++; }
    for (i = 0; i < 12; i++) {
        uint64_t x = powmod_u(bases[i], d, n);
        if (x == 1 || x == n - 1) continue;
        bool comp = true;
        int r;
        for (r = 1; r < s; r++) {
            x = mulmod_u(x, x, n);
            if (x == n - 1) { comp = false; break; }
        }
        if (comp) return false;
        if (n < 3215031751ULL && i >= 3) break; /* bases 2,3,5,7 suffice */
    }
    return true;
}

static uint64_t *terms = NULL;
static idx_t cnt = 0, cap = 0;
static uint64_t next_j = 0;

seq_t a(idx_t n) {
    static const uint64_t res[3] = {1, 10, 19};
    while (cnt < n) {
        int t;
        for (t = 0; t < 3; t++) {
            uint64_t k = 21 * next_j + res[t];
            uint64_t b = 10 * k;
            if (mr_prime(b + 1) && mr_prime(b + 3) &&
                mr_prime(b + 7) && mr_prime(b + 9)) {
                if (cnt == cap) {
                    cap = cap ? cap * 2 : 1024;
                    terms = (uint64_t *)realloc(terms, cap * sizeof(uint64_t));
                }
                terms[cnt++] = k;
            }
        }
        next_j++;
    }
    return (seq_t)terms[n - 1];
}
