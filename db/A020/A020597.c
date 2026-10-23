/*
 * A020597  Smallest nonempty set S containing prime divisors of 5k+3 for each
 *          k in S.
 * https://oeis.org/A020597
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) is the nth element of the smallest nonempty set S such that
 *            for every k in S the prime divisors of 5k+3 belong to S.
 *            Computed by iteratively expanding the set until it is closed
 * Range:     u8 n <= 9, u16 n <= 9, u32 n <= 9, u64 n <= 9, u128 n <= 9,
 *            big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    /* static data for the closed set S */
    static uint64_t S[9];
    static size_t S_len = 0;
    if (S_len == 0) {
        /* Build the closure of the set S. */
        uint64_t cur[16];   /* temporary list of primes being processed */
        size_t cur_len = 1;
        cur[0] = 2;         /* start with 2 */
        bool changed = true;

        while (changed) {
            changed = false;
            for (size_t i = 0; i < cur_len; ++i) {
                uint64_t k = cur[i];
                uint64_t v = 5 * k + 3;

                /* factor v into prime factors */
                uint64_t primes_buf[16];
                uint64_t exps_buf[16];
                size_t cnt;
                factor_u64(v, primes_buf, exps_buf, &cnt);

                /* add any new prime factors to cur */
                for (size_t j = 0; j < cnt; ++j) {
                    uint64_t p = primes_buf[j];
                    /* check if p is already present */
                    bool present = false;
                    for (size_t m = 0; m < cur_len; ++m) {
                        if (cur[m] == p) {
                            present = true;
                            break;
                        }
                    }
                    if (!present) {
                        cur[cur_len++] = p;
                        changed = true;
                    }
                }
            }
        }

        /* cur now contains the minimal closed set S, sort it */
        S_len = cur_len;
        for (size_t i = 0; i < S_len; ++i) {
            for (size_t j = i + 1; j < S_len; ++j) {
                if (cur[j] < cur[i]) {
                    uint64_t tmp = cur[i];
                    cur[i] = cur[j];
                    cur[j] = tmp;
                }
            }
        }
        /* copy to static S array */
        for (size_t i = 0; i < S_len; ++i)
            S[i] = cur[i];
    }

    if (n == 0) return (seq_t)0;           /* undefined for n<1, return 0 */
    if (n <= S_len) return (seq_t)S[n - 1];
    return (seq_t)0;                       /* beyond the finite set return 0 */
}
