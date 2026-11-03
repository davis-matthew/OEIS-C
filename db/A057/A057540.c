/*
 * A057540  Birthday set of order 8: i.e., numbers congruent to +- 1 modulo 2,
 *          3, 4, 5, 6, 7 and 8.
 * https://oeis.org/A057540
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 1249, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 62 terms through n = 9296 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes terms of A057540 by brute‑force search.
   A number belongs to the sequence iff it is congruent to ±1
   modulo every integer k with 2 ≤ k ≤ 8.  */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* Helper: test whether a candidate satisfies the modular conditions. */
static bool ok_candidate(seq_t x)
{
    for (int k = 2; k <= 8; ++k) {
        seq_t r = x % k;
        if (r != 1 && r != (k - 1))
            return false;
    }
    return true;
}

/* Main function requested by the problem statement. */
seq_t a(idx_t n)
{
    /* The sequence offset is 1, so n must be ≥ 1.  The function
       ignores any calls with n == 0; it simply returns 0 in that case. */
    if (n == 0) return 0;

    /* Cached array of already computed terms. */
    static seq_t *tab = NULL;
    static size_t cap = 0;     /* capacity of tab */
    static size_t len = 0;     /* number of terms stored */
    static seq_t nextCand = 1; /* next candidate to test */

    /* Ensure that we have at least n terms in tab. */
    if (n > len) {
        /* Grow the table if necessary. */
        if (cap == 0) {
            cap = 64;
            tab = mem_new(seq_t, cap);
        }
        while (cap < n) {
            size_t newcap = cap * 2;
            tab = mem_resize(seq_t, tab, newcap);
            cap = newcap;
        }

        /* Generate terms until we reach the n‑th one. */
        while (len < n) {
            if (ok_candidate(nextCand))
                tab[len++] = nextCand;
            ++nextCand;
        }
    }

    return tab[n - 1];
}
