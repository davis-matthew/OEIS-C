/*
 * A003680  Smallest number with 2n divisors.
 * https://oeis.org/A003680
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 16, u32 n <= 36, u64 n <= 66, u128 n <= 130,
 *            big any n
 * Helpers:   seq_is_fixed_width
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: a(n) = min over multiplicative partitions 2n = f1*f2*...*fk (f1>=f2>=...>=fk>=2)
   of 2^(f1-1)*3^(f2-1)*...*p_k^(fk-1), i.e. smallest m with exactly 2n divisors */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/seq_is_fixed_width.h"

static const uint64_t small_primes[] = {
    2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 59, 61, 67, 71,
    73, 79, 83, 89, 97, 101, 103, 107, 109, 113
};
#define NPRIMES ((int)(sizeof(small_primes) / sizeof(small_primes[0])))

static seq_t best_val;
static bool have_best;
static bool fixed_w;
static seq_t max_val;

/* multiply cur by p^e; return false on overflow (fixed width only) */
static bool mul_pow(seq_t cur, uint64_t p, uint64_t e, seq_t *out) {
    seq_t r = cur;
    seq_t sp = (seq_t)p;
    for (uint64_t j = 0; j < e; j++) {
        if (fixed_w && r > max_val / sp) return false;
        r *= sp;
        if (have_best && r >= best_val) { *out = r; return true; }
    }
    *out = r;
    return true;
}

static void search(uint64_t d, uint64_t maxf, int pi, seq_t cur) {
    if (d == 1) {
        if (!have_best || cur < best_val) {
            best_val = cur;
            have_best = true;
        }
        return;
    }
    if (pi >= NPRIMES) return;
    uint64_t top = maxf < d ? maxf : d;
    for (uint64_t f = top; f >= 2; f--) {
        if (d % f != 0) continue;
        seq_t nxt;
        if (!mul_pow(cur, small_primes[pi], f - 1, &nxt)) continue;
        if (have_best && nxt >= best_val) continue;
        search(d / f, f, pi + 1, nxt);
    }
}

seq_t a(idx_t n) {
    uint64_t d = 2 * (uint64_t)n;
    fixed_w = seq_is_fixed_width();
    if (fixed_w) max_val = (seq_t)-1;
    have_best = false;
    best_val = 0;
    search(d, d, 0, (seq_t)1);
    return have_best ? best_val : (seq_t)0;
}
