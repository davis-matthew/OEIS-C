/*
 * A048104  If n = Product p_i^e_i (e_i >= 1) then for some i, p_i > e_i and
 *          for some j, p_j < e_j.
 * https://oeis.org/A048104
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 26, u16 n <= 8845, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_spf
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth term of OEIS A048104: 
   integers n such that among its prime power factors p^e,
   there is at least one with p > e and at least one with p < e. */
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/build_spf.h"

seq_t a(idx_t n) {
    if (n < 1) return 0;          /* safety; n is guaranteed >=1 */

    /* Rough upper bound: density ≈ 0.135, so n*8 suffices. */
    idx_t limit = n * 8 + 100;

    idx_t *spf = NULL;
    build_spf(limit, &spf);

    idx_t count = 0;
    for (idx_t i = 2; i <= limit; ++i) {
        idx_t temp = i;
        bool have_greater = false, have_lesser = false;

        while (temp > 1) {
            idx_t p = spf[temp];
            idx_t e = 0;
            while (temp % p == 0) {
                temp /= p;
                ++e;
            }
            if (p > e) have_greater = true;
            if (p < e) have_lesser = true;
            if (have_greater && have_lesser) break;
        }

        if (have_greater && have_lesser) {
            ++count;
            if (count == n) {
                free(spf);
                return (seq_t)i;
            }
        }
    }

    free(spf);
    return 0;  /* should never be reached */
}
