/*
 * A003459  Absolute primes (or permutable primes): every permutation of the
 *          digits is a prime.
 * https://oeis.org/A003459
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, nice, hard
 * Formula:   terms below 1000 are found by search (every digit permutation is
 *            prime); for n >= 23, a(n) = A004022(n-21), i.e. repunits R_k =
 *            (10^k-1)/9 with k in A004023 (19, 23, 317, 1031, ...). This part
 *            is the OEIS conjecture
 * Range:     u8 n <= 16, u16 n <= 22, u32 n <= 22, u64 n <= 23, u128 n <= 24,
 *            big any n
 * Validated: partial, pass, 24 terms through n = 24 (2026-11-03)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime_u64.h"

static bool a3459_absolute(uint64_t m) {
    int dg[3], k = 0;
    uint64_t t = m;
    while (t > 0) { dg[k++] = (int)(t % 10); t /= 10; }
    if (k == 1) return is_prime_u64(m);
    if (k == 2) {
        return is_prime_u64((uint64_t)(dg[0] * 10 + dg[1])) &&
               is_prime_u64((uint64_t)(dg[1] * 10 + dg[0]));
    }
    {
        static const int perm[6][3] = {
            {0,1,2},{0,2,1},{1,0,2},{1,2,0},{2,0,1},{2,1,0}
        };
        int p;
        for (p = 0; p < 6; p++) {
            uint64_t v = (uint64_t)(dg[perm[p][0]] * 100 + dg[perm[p][1]] * 10 + dg[perm[p][2]]);
            if (!is_prime_u64(v)) return false;
        }
    }
    return true;
}

/* R_k computed by binary doubling: R_{a+b} = R_a*10^b + R_b */
static seq_t a3459_repunit(idx_t k) {
    seq_t r = 0, p10 = 1;          /* result R_e and 10^e */
    seq_t br = 1, bp = 10;         /* R_{2^i} and 10^{2^i} */
    while (k > 0) {
        if (k & 1) {
            r = r * bp + br;
            p10 = p10 * bp;
        }
        k >>= 1;
        if (k > 0) {
            br = br * bp + br;
            bp = bp * bp;
        }
    }
    (void)p10;
    return r;
}

seq_t a(idx_t n) {
    static uint64_t small[64];
    static idx_t nsmall = 0;
    static bool init = false;
    static const idx_t rep_k[] = {19, 23, 317, 1031, 49081, 86453};

    if (!init) {
        uint64_t m;
        for (m = 2; m < 1000; m++)
            if (a3459_absolute(m)) small[nsmall++] = m;
        init = true;
    }
    if (n >= 1 && n <= nsmall) return (seq_t)small[n - 1];
    {
        idx_t j = n - nsmall - 1;
        if (j < (idx_t)(sizeof(rep_k) / sizeof(rep_k[0])))
            return a3459_repunit(rep_k[j]);
    }
    return 0;
}
