/*
 * A156068  The slowest increasing sequence such that there is no common digit
 *          between any two integers from {a(n), a(n-1), a(n-2),
 *          c=a(n)+a(n-1)+a(n-2)}.
 * https://oeis.org/A156068
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, fini, full, nonn
 * Formula:   a(n) is the slowest increasing sequence such that there is no
 *            common digit between any two integers from {a(n), a(n-1),
 *            a(n-2), c=a(n)+a(n-1)+a(n-2)}. Compute by iteratively searching
 *            for the smallest integer that satisfies the pairwise
 *            digit‑disjointness constraint
 * Range:     u8 n <= 16, u16 n <= 24, u32 n <= 24, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Build a 10‑digit bitmask for a number. */
static uint16_t digit_mask(seq_t x)
{
    uint16_t mask = 0;
    if (x == 0) return 1u << 0;
    while (x > 0) {
        int d = (int)(x % (seq_t)10);
        mask |= 1u << d;
        x /= (seq_t)10;
    }
    return mask;
}

/* External linkage required function */
seq_t a(idx_t n)
{
    /* 1‑based indexing: a(1) is first term. */
    static idx_t seq_max = 0;      /* maximum index already computed */
    static seq_t *seq_arr   = NULL;/* array holding the terms, index 1.. */
    static size_t seq_cap  = 0;    /* allocated capacity */

    /* Allocate and initialise first five terms on first call */
    if (seq_arr == NULL) {
        seq_cap = (size_t)n + 1;
        if (seq_cap < 6) seq_cap = 6; /* must hold indices 1..5 */
        seq_arr = (seq_t *)malloc(seq_cap * sizeof(seq_t));
        if (seq_arr == NULL) return (seq_t)0; /* safety, unlikely to happen */
        seq_arr[1] = (seq_t)1;
        seq_arr[2] = (seq_t)2;
        seq_arr[3] = (seq_t)3;
        seq_arr[4] = (seq_t)4;
        seq_arr[5] = (seq_t)5;
        seq_max = 5;
    }

    /* Increase capacity if required */
    if ((size_t)n + 1 > seq_cap) {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(seq_arr, new_cap * sizeof(seq_t));
        if (tmp != NULL) {
            seq_arr = tmp;
            seq_cap = new_cap;
        }
    }

    /* Return pre‑computed term */
    if (n <= seq_max) return seq_arr[n];

    /* Compute terms from seq_max+1 up to n */
    for (idx_t idx = seq_max + 1; idx <= n; ++idx) {
        seq_t prev1 = seq_arr[idx - 1]; /* a(n-1) */
        seq_t prev2 = seq_arr[idx - 2]; /* a(n-2) */
        seq_t prev3 = seq_arr[idx - 3]; /* a(n-3) – unused but kept for completeness */

        uint16_t mask_prev1 = digit_mask(prev1);
        uint16_t mask_prev2 = digit_mask(prev2);

        seq_t cand = prev1 + (seq_t)1; /* start searching from previous term + 1 */

        while (true) {
            uint16_t mask_cand = digit_mask(cand);

            /* Candidate must be digit‑disjoint from prev1 and prev2 */
            if ((mask_cand & mask_prev1) || (mask_cand & mask_prev2)) {
                ++cand;
                continue;
            }

            /* Sum of the three terms */
            seq_t sum = cand + prev1 + prev2;
            uint16_t mask_sum = digit_mask(sum);

            /* Check all pairwise disjointness */
            if ((mask_cand & mask_sum) || (mask_prev1 & mask_sum) || (mask_prev2 & mask_sum)) {
                ++cand;
                continue;
            }

            /* Valid term found */
            seq_arr[idx] = cand;
            break;
        }
    }

    seq_max = n;
    return seq_arr[n];
}
