/*
 * A158085  Starting at a(1)=2, a(n) is the smallest prime larger than a(n-1)
 *          such that the sum of odd digits of a(n) is not smaller than the
 *          sum of odd digits of a(n-1).
 * https://oeis.org/A158085
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, less
 * Formula:   a(n) = smallest prime > a(n‑1) with sum of odd digits >= sum of
 *            odd digits of a(n‑1); a(1)=2
 * Range:     u8 n <= 13, u16 n <= 40, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   is_prime_big, sum_odd_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"
#include "../utils/sum_odd_digits.h"

/* OEIS sequence A158085 */
seq_t a(idx_t n) {
    static seq_t *seq = NULL;
    static size_t seq_cap = 0;
    static size_t seq_len = 0;

    if (n == 0) return 0; /* not used, but guard */

    /* Build sequence up to requested index */
    while (seq_len < n) {
        /* Ensure capacity */
        if (seq_len >= seq_cap) {
            seq_cap = seq_cap ? seq_cap * 2 : 8;
            seq = (seq_t *)realloc(seq, seq_cap * sizeof(seq_t));
        }

        if (seq_len == 0) {                 /* first term */
            seq[0] = (seq_t)2;
            seq_len = 1;
            continue;
        }

        seq_t prev = seq[seq_len - 1];
        seq_t prevSum = sum_odd_digits(prev);

        seq_t cand = prev + 1;
        seq_t candSum;
        for (;;) {
            if (!is_prime_big(cand)) { cand++; continue; }

            candSum = sum_odd_digits(cand);
            if (candSum >= prevSum) {
                seq[seq_len] = cand;
                seq_len++;
                break;
            }
            cand++;
        }
    }

    return seq[n - 1];
}
