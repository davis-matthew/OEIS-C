/*
 * A212372  Nonprime numbers with distinct digits in descending order.
 * https://oeis.org/A212372
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 41, u32 n <= 934, u64 n <= 935, u128 n <= 935,
 *            big any n
 * Helpers:   cmp_seq_t, is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Nonprime numbers with distinct digits in descending order (OEIS A212372) */
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/is_prime_small.h"
#include "../utils/cmp_seq_t.h"

/* a(1) = 1, a(2) = 4, a(3) = 6, …  (nonprime numbers with digits in strictly decreasing order) */
seq_t a(idx_t n)
{
    static bool initialized = false;
    static seq_t list[1024];
    static size_t len = 0;

    if (!initialized) {
        const size_t MAX_MASK = 1U << 10;           /* digits 0‑9 */
        for (size_t mask = 1; mask < MAX_MASK; ++mask) {
            /* Skip the single‑digit zero (mask == 1 → number 0) */
            if (mask == 1)
                continue;
            seq_t val = 0;
            for (int d = 9; d >= 0; --d)            /* build lowest → highest digit, then multiply */
                if (mask & (1U << d))
                    val = val * (seq_t)10 + (seq_t)d;
            if (!is_prime_small(val))
                list[len++] = val;
        }
        qsort(list, len, sizeof(seq_t), cmp_seq_t);
        initialized = true;
    }

    if (n == 0 || n > len)
        return (seq_t)0;
    return list[n - 1];
}
