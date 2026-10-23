/*
 * A131847  Positive integers n which each have at least one "isolated
 *          isolated divisor". An isolated isolated divisor is an isolated
 *          divisor that is not adjacent to any other isolated divisors in the
 *          list of all positive divisors of n. A divisor, k, of n is isolated
 *          if neither (k-1) nor (k+1) divides n.
 * https://oeis.org/A131847
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = nth positive integer having at least one isolated
 *            isolated divisor. An isolated divisor k of n satisfies neither
 *            (k-1) nor (k+1) divides n. An isolated isolated divisor is an
 *            isolated divisor that is not adjacent (in the sorted list of all
 *            divisors) to any other isolated divisor
 * Range:     u8 n <= 14, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

static int cmp_uint64(const void *a, const void *b) {
    const uint64_t *x = (const uint64_t *)a;
    const uint64_t *y = (const uint64_t *)b;
    return (*x < *y) ? -1 : (*x > *y) ? 1 : 0;
}

/* Check whether an integer n has at least one isolated isolated divisor. */
static bool has_isolated_isolated(uint64_t n) {
    /* Small static buffers are reused for each call. */
    static uint64_t divs[4096];
    static bool iso[4096];
    size_t dcnt = 0;

    /* Enumerate all divisors (unsorted). */
    for (uint64_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            divs[dcnt++] = d;
            uint64_t other = n / d;
            if (other != d) divs[dcnt++] = other;
        }
    }

    /* Sort the divisor list. */
    qsort(divs, dcnt, sizeof(uint64_t), cmp_uint64);

    /* Mark isolated divisors. */
    for (size_t i = 0; i < dcnt; ++i) {
        uint64_t d = divs[i];
        bool isolated_flag;
        if (d == 1) {
            isolated_flag = (n % 2 != 0);
        } else {
            bool left = (n % (d - 1) == 0);
            bool right = (d != UINT64_MAX) && (n % (d + 1) == 0);
            isolated_flag = !(left || right);
        }
        iso[i] = isolated_flag;
    }

    /* Look for an isolated isolated divisor. */
    for (size_t i = 0; i < dcnt; ++i) {
        if (!iso[i]) continue;
        bool pred = (i > 0 && iso[i - 1]);
        bool succ = (i + 1 < dcnt && iso[i + 1]);
        if (!pred && !succ) return true;
    }
    return false;
}

/* Memoised sequence generation. */
seq_t a(idx_t n) {
    /* Validate positive index. */
    if (n == 0) return 0;

    /* Static storage for already computed terms. */
    static seq_t *terms = NULL;
    static size_t terms_cap = 0;
    static size_t terms_count = 0;
    static uint64_t current_candidate = 1;

    /* Extend the sequence until the nth term is known. */
    idx_t target = n - 1;               /* zero‑based index */
    while (terms_count <= (size_t)target) {
        if (has_isolated_isolated(current_candidate)) {
            /* Grow array if needed. */
            if (terms_count >= terms_cap) {
                terms_cap = (terms_cap == 0) ? 64 : terms_cap * 2;
                terms = (seq_t *)realloc(terms, terms_cap * sizeof(seq_t));
            }
            terms[terms_count++] = (seq_t)current_candidate;
        }
        ++current_candidate;
    }
    return terms[n - 1];
}
