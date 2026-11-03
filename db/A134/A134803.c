/*
 * A134803  Numbers n such that the sum of all numbers of the same parity <= n
 *          is equal to the sum of numbers of the opposite parity from n+1 to
 *          n+m, where m is odd and > 1.
 * https://oeis.org/A134803
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = the nth integer n such that there exists an odd integer m
 *            ≥ 1 for which the sum of all numbers of the same parity ≤ n
 *            equals the sum of all numbers of the opposite parity in the
 *            interval [n+1, n+m]
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Validated: partial, pass, 6 terms through n = 6 (2026-11-03)
 * Helpers:   ipow_i32, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/ipow_i32.h"
#include "../utils/mem.h"

/* Floor square root by binary search */
static inline seq_t floor_sqrt(seq_t x)
{
    seq_t lo = 0, hi = 1;
    while (ipow_i32(hi, 2) <= x)
        hi <<= 1;
    while (lo < hi)
    {
        seq_t mid = (lo + hi) >> 1;
        seq_t mid2 = mid * mid;
        if (mid2 <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

seq_t a(idx_t n)
{
    static seq_t *terms = NULL;
    static idx_t term_count = 0;      /* number of terms currently stored */
    static idx_t last_checked = 0;    /* largest n that has been tested */
    static size_t cap = 0;            /* allocated capacity for terms */

    while (term_count < n) {
        last_checked++;                               /* candidate n */

        seq_t N = (seq_t)last_checked;
        seq_t S;                                      /* sum same parity up to N */

        /* compute S: sum of odds or evens depending on parity of N */
        if (last_checked & 1) {                      /* N is odd → sum odds */
            seq_t k = (N + 1) / 2;                   /* number of odds up to N */
            S = k * k;                               /* sum 1+3+...+N = k^2 */
        } else {                                      /* N even → sum evens */
            seq_t half = N / 2;
            S = half * (half + 1);                   /* sum 2+4+...+N = half*(half+1) */
        }

        /* Quadratic discriminant: D = N^2 + 4*S */
        seq_t D = N * N + (seq_t)4 * S;
        seq_t sqrtD = floor_sqrt(D);

        /* Check if D is a perfect square and yields an integer k = (sqrtD - N)/2 */
        if (sqrtD * sqrtD == D) {
            seq_t diff = sqrtD - N;
            if ((diff & 1) == 0) {                   /* diff even → k integer */
                seq_t k_val = diff >> 1;             /* k = (sqrtD - N)/2 */
                if (k_val >= 1) {                    /* k≥1 corresponds to odd m≥1 */
                    /* resize storage if necessary */
                    if (term_count == cap) {
                        size_t new_cap = cap ? (cap << 1) : 16;
                        seq_t *tmp = mem_resize(seq_t, terms, new_cap);
                        if (!tmp) return 0;           /* allocation failure – unlikely */
                        terms = tmp;
                        cap = new_cap;
                    }
                    terms[term_count++] = N;
                }
            }
        }
    }

    return terms[n - 1];
}
