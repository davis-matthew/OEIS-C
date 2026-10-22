/*
 * A005478  Prime Fibonacci numbers.
 * https://oeis.org/A005478
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   a(n) = A000045(A001605(n)), i.e. Fibonacci numbers F(k) that are
 *            prime. Search: walk k upward, test F(k) only for k = 4 or k
 *            prime. Composite F(k) are mostly ruled out by trial division
 *            with divisors q == +-1 (mod 2k), q == 1 (mod 4), computed in
 *            native modular arithmetic. Survivors get a strong...
 * Range:     u8 n <= 6, u16 n <= 8, u32 n <= 11, u64 n <= 12, u128 n <= 14,
 *            big any n
 * Helpers:   addmod, fibmod, grow_arr, has_small_divisor, is_prime,
 *            seq_is_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/is_prime.h"
#include "../utils/seq_is_big.h"
#include "../utils/grow_arr.h"
#include "../utils/addmod.h"
#include "../utils/fibmod.h"
#include "../utils/has_small_divisor.h"

static seq_t mulmod(seq_t x, seq_t y, seq_t m) {
    if (seq_is_big()) return (x * y) % m;
    seq_t r = 0;
    x %= m;
    y %= m;
    while (y > 0) {
        if ((y & 1) != 0) r = addmod(r, x, m);
        x = addmod(x, x, m);
        y >>= 1;
    }
    return r;
}

static bool sprp(seq_t m, seq_t d, int s, unsigned b, unsigned char *bits) {
    int nb = 0;
    seq_t e = d;
    while (e > 0) {
        bits[nb++] = (unsigned char)((e & 1) != 0);
        e >>= 1;
    }
    seq_t x = (seq_t)b % m;
    for (int i = nb - 1; i-- > 0;) {
        x = mulmod(x, x, m);
        if (bits[i]) x = mulmod(x, (seq_t)b, m);
    }
    if (x == 1 || x == m - 1) return true;
    for (int r = 1; r < s; r++) {
        x = mulmod(x, x, m);
        if (x == m - 1) return true;
    }
    return false;
}

static bool is_prime_fib(seq_t m, uint64_t k) {
    static const unsigned smallp[11] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31};
    if (m < 2) return false;
    for (int i = 0; i < 11; i++) {
        if (m == smallp[i]) return true;
        if (m % smallp[i] == 0) return false;
    }
    if (m < 1024) return true;
    if (k > 100 && has_small_divisor(k)) return false;
    seq_t d = m - 1;
    int s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        s++;
    }
    unsigned char *bits = (unsigned char *)malloc((size_t)k + 140);
    static const unsigned bases[12] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37};
    int nbases = (k < 200) ? 12 : 1;
    bool ok = true;
    for (int i = 0; i < nbases && ok; i++)
        ok = sprp(m, d, s, bases[(nbases == 1) ? 1 : i], bits);
    free(bits);
    return ok;
}

seq_t a(idx_t n) {
    static seq_t *found = NULL;
    static idx_t nfound = 0, cap = 0;
    static seq_t fk = 0, fk1 = 1;   /* F(k), F(k+1) */
    static uint64_t k = 0;
    static bool ovf = false;

    if (n == 0) return 0;
    while (nfound < n) {
        if (ovf) return 0;
        seq_t t = fk + fk1;
        fk = fk1;
        fk1 = t;
        k++;
        if (!seq_is_big() && fk1 < fk) ovf = true;
        if ((k == 4 || is_prime(k)) && is_prime_fib(fk, k)) {
            if (nfound == cap) {
                idx_t nc = cap ? cap * 2 : 16;
                found = grow_arr(found, cap, nc);
                cap = nc;
            }
            found[nfound++] = fk;
        }
    }
    return found[n - 1];
}
