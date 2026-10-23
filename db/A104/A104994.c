/*
 * A104994  Least k(n)>1 such that n*k(n)^2+n*k(n)+1 is a prime >
 *          (n-1)*k(n-1)^2+(n-1)*k(n-1)+1 or 0 if n=4 as no prime possible.
 * https://oeis.org/A104994
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 35, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   is_prime_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computed by search: find the least k(n)>1 such that
   n*k(n)^2 + n*k(n) + 1 is a prime larger than the previous
   prime value; a(4)=0 because no such prime exists. */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_seq.h"

seq_t a(idx_t n)
{
    /* memoised k‑values, 1‑based (stored at index n–1) */
    static seq_t *k_vals = NULL;
    static idx_t cap   = 0;   /* allocated size */
    static idx_t sz    = 0;   /* number of terms already computed */
    static seq_t prev_prime = 0;   /* largest prime seen so far */

    if (n == 0) return 0;     /* offset is 1 */

    /* grow the array if necessary */
    if (n > cap) {
        idx_t new_cap = cap ? cap * 2 : 16;
        while (new_cap < n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(k_vals, new_cap * sizeof(seq_t));
        if (!tmp) abort();          /* allocation failure */
        k_vals = tmp;
        cap = new_cap;
    }

    /* compute missing terms up to n */
    for (idx_t m = sz + 1; m <= n; ++m) {
        seq_t k_val = 0;

        if (m == 1) {
            /* k(1) = 2, first prime 7 */
            k_val = 2;
            prev_prime = (seq_t)m * 4 + (seq_t)m * 2 + 1; /* 1*4+1*2+1 */
        }
        else if (m == 4) {
            /* no prime possible */
            k_val = 0;
        }
        else {
            seq_t cand = 2;
            while (true) {
                seq_t cand_sq = cand * cand;
                seq_t M = (seq_t)m * cand_sq + (seq_t)m * cand + 1;
                if (M > prev_prime && is_prime_seq(M)) {
                    k_val = cand;
                    prev_prime = M;
                    break;
                }
                ++cand;
            }
        }
        k_vals[m - 1] = k_val;
    }

    sz = n;
    return k_vals[n - 1];
}
