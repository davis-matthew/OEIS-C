/*
 * A111464  a(1) = 12, a(n+1) = lcm{a(n), Digit Reversal(a(n))}.
 * https://oeis.org/A111464
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(1) = 12; a(n) = lcm(a(n-1), rev(a(n-1))) for n > 1
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 6, u128 n <= 7,
 *            big any n
 * Helpers:   gcd_any, rev
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/gcd_any.h"
#include "../utils/rev.h"

seq_t a(idx_t n) {
    /* static cache: a_arr[1] holds a(1) */
    static seq_t *a_arr = NULL;
    static idx_t  a_size = 0; /* highest index computed */
    static size_t a_cap  = 0; /* allocated capacity (>= a_size+1) */

    if (n == 0) return 0; /* undefined for n=0, return 0 */

    /* Allocate & init cache on first call */
    if (a_size == 0) {
        size_t ini_cap = 4;
        a_arr = (seq_t *)malloc(ini_cap * sizeof(seq_t));
        a_cap = ini_cap;
        /* a(1) = 12 */
        a_arr[1] = (seq_t)12;
        a_size = 1;
    }

    /* Ensure capacity for index n */
    if ((size_t)(n + 1) > a_cap) {
        size_t new_cap = a_cap * 2;
        while (new_cap < (size_t)(n + 1))
            new_cap <<= 1;
        a_arr = (seq_t *)realloc(a_arr, new_cap * sizeof(seq_t));
        a_cap = new_cap;
    }

    /* Compute missing terms up to n */
    for (idx_t i = a_size + 1; i <= n; ++i) {
        seq_t prev = a_arr[i - 1];
        seq_t rev_prev = rev(prev);
        seq_t g = gcd_any(prev, rev_prev);

        /* compute lcm = (prev / g) * rev_prev  using 128‑bit intermediate */
        seq_t tmp = prev / g;
        seq_t lcm = (seq_t)(((unsigned __int128)tmp) * rev_prev);

        a_arr[i] = lcm;
    }
    a_size = n;

    return a_arr[n];
}
