/*
 * A178605  a(n) = number of 6-digit primes with digit sum n, where n runs
 *          through the non-multiples of 3 in the range [2..53].
 * https://oeis.org/A178605
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full
 * Formula:   number of 6‑digit primes with digit sum equal to the nth
 *            non‑multiple of 3 between 2 and 53 (offset 1)
 * Range:     u8 none, u16 none, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   build_primes_upto, digit_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/build_primes_upto.h"
#include "../utils/digit_sum.h"

seq_t a(idx_t n)
{
    /* mapping of the 35 non‑multiples of 3 in [2..53] */
    static const int sumlist[35] = {
        2,4,5,7,8,10,11,13,14,16,17,19,20,22,23,25,26,28,29,
        31,32,34,35,37,38,40,41,43,44,46,47,49,50,52,53
    };

    /* cache of counts for each possible digit sum (0..54) */
    static seq_t counts[55];
    static bool init = false;

    if (!init) {
        uint64_t *primes = NULL;
        size_t count_primes = 0;
        build_primes_upto(999999, &primes, &count_primes);

        for (size_t i = 0; i < count_primes; ++i) {
            uint64_t p = primes[i];
            if (p < 100000) continue;          /* not a 6‑digit number */
            int sum = (int)digit_sum((seq_t)p);
            if (sum <= 54) counts[sum]++;      /* 6‑digit primes only */
        }
        free(primes);
        init = true;
    }

    if (n < 1 || n > 35) return 0;              /* out of defined range */
    int target = sumlist[n - 1];
    return counts[target];
}
