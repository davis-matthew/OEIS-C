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
 * Helpers:   a3459_absolute, a3459_isprime, a3459_repunit
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/a3459_absolute.h"
#include "../utils/a3459_isprime.h"
#include "../utils/a3459_repunit.h"

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
