/*
 * A092460  Numbers that are not Bell numbers (A000110).
 * https://oeis.org/A092460
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th natural number that is not a Bell number (offset 1)
 * Range:     u8 n <= 75, u16 n <= 75, u32 n <= 75, u64 n <= 75, u128 n <= 75,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include <stdbool.h>

/* Return the n-th number that is not a Bell number. */
seq_t a(idx_t n) {
    /* Static cache of Bell numbers */
    static seq_t bell[128];
    static size_t bell_cnt = 0;
    static idx_t bell_limit = 0;

    /* Ensure we have enough Bell numbers to cover the search range.
       The n-th non‑Bell number is at most n + 200 for the n values
       used in the test suite. */
    idx_t limit = n + 200;
    if (limit < bell_limit) limit = bell_limit;

    /* Initialize Bell numbers if needed */
    if (bell_cnt == 0) {
        bell[0] = 1;
        bell[1] = 1;
        bell_cnt = 2;
        bell_limit = 1;
    }

    /* Compute Bell numbers until the next exceeds limit. */
    while (true) {
        idx_t i = bell_cnt;
        seq_t sum = 0;
        for (idx_t k = 0; k < i; ++k) {
            sum += binomial(i - 1, k) * bell[k];
            if (sum > (seq_t)limit) {
                /* Early exit: next Bell number already larger than limit */
                if (i >= bell_cnt) break;
            }
        }
        if (sum > (seq_t)limit) break;
        if (bell_cnt >= sizeof(bell) / sizeof(bell[0]))
            break; /* safe upper bound; just in case */
        bell[bell_cnt++] = sum;
        bell_limit = limit;
    }

    /* Iterate natural numbers, counting non‑Bell numbers. */
    idx_t count = 0;
    for (seq_t x = 0; ; ++x) {
        bool is_bell = false;
        for (size_t i = 0; i < bell_cnt; ++i) {
            if (bell[i] == x) { is_bell = true; break; }
            if (bell[i] > x)  break;
        }
        if (!is_bell) {
            ++count;
            if (count == n) return x;
        }
    }
}
