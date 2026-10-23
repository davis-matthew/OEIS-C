/*
 * A107835  Slowest increasing sequence where a(n)-(first digit of a(n)) is
 *          prime.
 * https://oeis.org/A107835
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = the smallest integer > a(n-1) such that a(n) -
 *            first_digit(a(n)) is prime. The sequence is defined for n ≥ 1
 *            with offset 1. This implementation memoizes the terms and
 *            expands the table on demand
 * Range:     u8 n <= 49, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   ensure_seq_array, first_digit, isprime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include "../utils/ensure_seq_array.h"
#include "../utils/isprime_big.h"
#include "../utils/first_digit.h"

seq_t a(idx_t n)
{
    static seq_t *tbl  = NULL;   /* 1‑based array of computed terms   */
    static size_t cap = 0;       /* allocated capacity                */
    static size_t sz  = 0;       /* number of terms stored            */

    /* Expand the table until we have at least n terms. */
    while (sz < n) {
        seq_t val;
        if (sz == 0) {                 /* a(1) = 12 */
            val = (seq_t)12;
        } else {
            seq_t cand = tbl[sz - 1] + (seq_t)1;
            for (;;) {
                seq_t fd   = first_digit(cand);
                seq_t diff = cand - fd;
                if (isprime_big(diff)) {
                    val = cand;
                    break;
                }
                cand += (seq_t)1;
            }
        }

        /* Ensure the array is large enough for the new element. */
        ensure_seq_array(&tbl, &cap, sz + 1);
        tbl[sz++] = val;
    }

    return tbl[n - 1];
}
