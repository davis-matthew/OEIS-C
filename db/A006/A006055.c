/*
 * A006055  Primes with consecutive (ascending) digits.
 * https://oeis.org/A006055
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   search, not a closed formula. For each length L = 1, 2, ... and
 *            each leading digit d = 1..9, build the number whose digits are
 *            d, d+1, ... taken mod 10. Candidates come in increasing order,
 *            so a(n) is the n-th prime among them. Primality: deterministic
 *            for 64-bit values, Miller-Rabin above that
 * Range:     u8 n <= 7, u16 n <= 8, u32 n <= 12, u64 n <= 14, u128 n <= 17,
 *            big any n
 * Helpers:   build128, buildseq, is_prime, mm128, pm128, pmbig, seq_is_big,
 *            u128_to_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/is_prime.h"
#include "../utils/seq_is_big.h"
#include "../utils/u128_to_seq.h"
#include "../utils/build128.h"
#include "../utils/buildseq.h"
#include "../utils/mm128.h"
#include "../utils/pm128.h"
#include "../utils/pmbig.h"

static int *found_L = NULL;
static int *found_d = NULL;
static idx_t found_cnt = 0, found_cap = 0;
static int cur_L = 1, cur_d = 1;

static const int small_pr[] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47};

static bool prime128(unsigned __int128 v) {
    if (v < 2) return false;
    if ((v >> 64) == 0) return is_prime((uint64_t)v);
    for (int i = 0; i < 15; i++)
        if (v % (unsigned)small_pr[i] == 0) return false;
    unsigned __int128 d = v - 1;
    int s = 0;
    while ((d & 1) == 0) { d >>= 1; s++; }
    for (int i = 0; i < 15; i++) {
        unsigned __int128 x = pm128((unsigned __int128)small_pr[i], d, v);
        if (x == 1 || x == v - 1) continue;
        bool ok = false;
        for (int j = 1; j < s; j++) {
            x = mm128(x, x, v);
            if (x == v - 1) { ok = true; break; }
        }
        if (!ok) return false;
    }
    return true;
}

static bool prime_big(seq_t v) {
    for (int p = 2; p < 2000; p++) {
        bool pp = true;
        for (int q = 2; q * q <= p; q++)
            if (p % q == 0) { pp = false; break; }
        if (pp && v % p == 0) return false;
    }
    seq_t d = v - 1;
    int s = 0;
    while (d % 2 == 0) { d /= 2; s++; }
    for (int i = 0; i < 4; i++) {
        seq_t x = pmbig((seq_t)small_pr[i], d, v);
        if (x == 1 || x == v - 1) continue;
        bool ok = false;
        for (int j = 1; j < s; j++) {
            x = (x * x) % v;
            if (x == v - 1) { ok = true; break; }
        }
        if (!ok) return false;
    }
    return true;
}

static bool test_cand(int L, int d) {
    if (L == 1) return d == 2 || d == 3 || d == 5 || d == 7;
    int last = (d + L - 1) % 10;
    if (last % 2 == 0 || last == 5) return false;
    int sum = 0;
    for (int i = 0; i < L; i++) sum += (d + i) % 10;
    if (sum % 3 == 0) return false;
    if (L <= 38) return prime128(build128(L, d));
    return prime_big(buildseq(L, d));
}

seq_t a(idx_t n) {
    bool big = seq_is_big() != 0;
    while (found_cnt < n) {
        if (!big && cur_L > 38) return 0;
        if (test_cand(cur_L, cur_d)) {
            if (found_cnt == found_cap) {
                found_cap = found_cap ? found_cap * 2 : 64;
                found_L = (int *)realloc(found_L, found_cap * sizeof(int));
                found_d = (int *)realloc(found_d, found_cap * sizeof(int));
            }
            found_L[found_cnt] = cur_L;
            found_d[found_cnt] = cur_d;
            found_cnt++;
        }
        cur_d++;
        if (cur_d > 9) { cur_d = 1; cur_L++; }
    }
    int L = found_L[n - 1], d = found_d[n - 1];
    if (L <= 38) return u128_to_seq(build128(L, d));
    return buildseq(L, d);
}
