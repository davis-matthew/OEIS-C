/*
 * A230603  Generalized Fibonacci word. Binary complement of A221150.
 * https://oeis.org/A230603
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 80, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   init_len_tab
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A230603: Generalized Fibonacci word (binary complement of A221150).
   The infinite word S(∞) is obtained by the recurrence
     S(0) = "1"
     S(1) = "110"
     S(n) = S(n-1)S(n-2)  for n ≥ 2
   The term a(n) is the nth character of S(∞) (0‑based).
   This implementation returns the character as a seq_t value (0 or 1)
   and uses only integer arithmetic and small look‑ups. */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/init_len_tab.h"

seq_t a(idx_t n)
{
    /* Grow a table of lengths up to required n */
    static idx_t len[93];
    static size_t len_cnt = 0;
    init_len_tab(n, len, &len_cnt);

    /* Find the smallest k with n < len[k] */
    size_t k = len_cnt - 1;
    while (n >= len[k])
        /* safety guard: if n exceeds precomputed, extend table */
        init_len_tab(n, len, &len_cnt), k = len_cnt - 1;

    /* Descend the recurrence tree */
    while (k > 1) {
        if (n < len[k - 1]) {
            /* in the first part S(k-1) */
            k -= 1;
        } else {
            /* in the second part S(k-2) */
            n -= len[k - 1];
            k -= 2;
        }
    }

    /* k is 0 or 1 now */
    if (k == 0) {
        /* S(0) = "1"                  */
        return (seq_t)1;
    } else { /* k == 1 */
        static const seq_t bits[3] = { (seq_t)1, (seq_t)1, (seq_t)0 };
        /* S(1) = "110"                */
        return bits[(size_t)n];
    }
}
