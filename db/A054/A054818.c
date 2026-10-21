/*
 * A054818  Sixth term of strong prime sextets: p(m-4)-p(m-5) > p(m-3)-p(m-4)
 *          > p(m-2)-p(m-3) > p(m-1)-p(m-2) > p(m)-p(m-1).
 * https://oeis.org/A054818
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the 6th prime p(m) of a strong prime sextet where
 *            consecutive prime gaps are strictly decreasing: p(m-4)-p(m-5) >
 *            p(m-3)-p(m-4) > p(m-2)-p(m-3) > p(m-1)-p(m-2) > p(m)-p(m-1)
 * Range:     u8 none, u16 n <= 18, u32 n <= 32, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   is_prime, next_prime, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/is_prime.h"
#include "../utils/next_prime.h"
#include "../utils/seq_realloc.h"

seq_t a(idx_t n) {
    static seq_t *results  = NULL;   /* memoised sequence values   */
    static size_t res_cap = 0;       /* allocated capacity         */
    static size_t res_len = 0;       /* number of values stored    */

    /* window of last 6 primes */
    static uint64_t win[6];
    static size_t win_len = 0;

    /* next prime to test */
    static uint64_t next_start = 2;

    /* generate values until we have at least n+1 items */
    while (res_len <= (size_t)n) {
        uint64_t prime = next_prime(next_start);
        if (prime == 0) abort();                /* safety net – should not happen */
        next_start = prime + 1;

        /* slide the window */
        if (win_len < 6) {
            win[win_len++] = prime;
        } else {
            memmove(win, win + 1, 5 * sizeof(uint64_t));
            win[5] = prime;

            /* compute the five consecutive gaps */
            uint64_t d0 = win[1] - win[0];
            uint64_t d1 = win[2] - win[1];
            uint64_t d2 = win[3] - win[2];
            uint64_t d3 = win[4] - win[3];
            uint64_t d4 = win[5] - win[4];

            if (d0 > d1 && d1 > d2 && d2 > d3 && d3 > d4) {
                /* store the sixth prime of the sextet */
                if (res_len == res_cap) {
                    size_t new_cap = (res_cap == 0) ? 16 : res_cap * 2;
                    seq_t *tmp = seq_realloc(results, res_cap, new_cap);
                    if (!tmp) abort();
                    results = tmp;
                    res_cap = new_cap;
                }
                results[res_len++] = (seq_t)win[5];
            }
        }
    }

    return results[(size_t)n];
}
