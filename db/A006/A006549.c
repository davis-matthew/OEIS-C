/*
 * A006549  Numbers k such that k and k+1 are prime powers.
 * https://oeis.org/A006549
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   search over exponents e: k = 2^e - 1 is a term iff e = 1 or 2^e
 *            - 1 is a Mersenne prime (Lucas-Lehmer test); k = 2^e is a term
 *            iff 2^e + 1 is a prime power (e = 1, 2, 3, 4, 8, 16). By
 *            Mihailescu's theorem these are the only consecutive prime-power
 *            pairs
 * Range:     u8 n <= 9, u16 n <= 11, u32 n <= 15, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   is_prime, powmod64, seq_is_fixed_width
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime.h"
#include "../utils/seq_is_fixed_width.h"
#include "../utils/powmod64.h"

#define MAXT 256

static seq_t terms[MAXT];
static idx_t nterms = 0;
static unsigned cur_e = 0;
static bool exhausted = false;

static unsigned value_bits(void) {
    if (seq_is_fixed_width()) return (unsigned)(sizeof(seq_t) * 8);
    return 1u << 30;
}

static bool mersenne_prime(unsigned p) {
    if (p == 2) return true;
    if (p < 2 || !is_prime(p)) return false;
    if (p < 64) return is_prime(((uint64_t)1 << p) - 1);
    /* trial factors q = 2kp+1, q = +-1 mod 8 */
    for (uint64_t k = 1; k < 200000; k++) {
        uint64_t q = 2 * k * (uint64_t)p + 1;
        if ((q & 7) != 1 && (q & 7) != 7) continue;
        if (powmod64(2, p, q) == 1) return false;
    }
    /* Lucas-Lehmer */
    seq_t one = 1;
    seq_t M = (one << p) - 1;
    seq_t s = 4;
    for (unsigned i = 0; i < p - 2; i++) {
        seq_t x = s * s + M - 2;
        while (x > M) x = (x & M) + (x >> p);
        if (x == M) x = 0;
        s = x;
    }
    return s == 0;
}

static void add_term(seq_t v) {
    if (nterms < MAXT) terms[nterms++] = v;
}

seq_t a(idx_t n) {
    unsigned bits = value_bits();
    while (nterms < n && !exhausted) {
        cur_e++;
        if (cur_e >= bits) { exhausted = true; break; }
        seq_t one = 1;
        seq_t p2 = one << cur_e;
        if (cur_e == 1 || mersenne_prime(cur_e)) add_term(p2 - 1);
        if (cur_e == 1 || cur_e == 2 || cur_e == 3 || cur_e == 4 ||
            cur_e == 8 || cur_e == 16)
            add_term(p2);
        if (nterms >= MAXT) exhausted = true;
    }
    if (n < 1 || n > nterms) return 0;
    return terms[n - 1];
}
