/*
 * A169738  Base 10 numbers d_1 d_2 ... d_k such that the digits d_i are
 *          distinct and not zero, and Sum_{i=1..k-1} d_i^i = d_k^k.
 * https://oeis.org/A169738
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full
 * Formula:   a(n) is computed by exhaustive search of all permutations of the
 *            digits 1‑9, selecting those for which d₁¹ + d₂² + … +
 *            d_{k‑1}^{k‑1} = d_k^k. The results are stored in ascending
 *            numeric order and returned by index
 * Range:     u8 none, u16 n <= 6, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   add_result
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/add_result.h"

/* Recursive generator that builds numbers of a fixed length.
   Parameters:
     target_len - desired length of the number being built
     depth      - current depth in the recursion
     num        - number built so far
     digits     - buffer to hold the digits (1‑based positions)
     used_mask  - bitmask of digits already used
     pow_tbl    - pre‑computed powers d^e (1‑indexed)
     seqp,len,cap - dynamic array to collect valid numbers    */
static void gen_len(int target_len, int depth,
                    seq_t num, uint8_t *digits, uint32_t used_mask,
                    seq_t pow_tbl[10][10],
                    seq_t **seqp, size_t *len, size_t *cap)
{
    if (depth == target_len) {            /* number fully constructed */
        seq_t sum = 0;
        for (int i = 0; i < target_len - 1; ++i)
            sum += pow_tbl[digits[i]][i + 1];
        seq_t rhs = pow_tbl[digits[target_len - 1]][target_len];
        if (sum == rhs)
            add_result(seqp, len, cap, num);
        return;
    }

    for (uint8_t d = 1; d <= 9; ++d) {
        if (used_mask & (1U << d)) continue;   /* digit already used */
        digits[depth] = d;
        gen_len(target_len, depth + 1,
                num * 10 + d,
                digits,
                used_mask | (1U << d),
                pow_tbl,
                seqp, len, cap);
    }
}

/* Comparator for qsort on seq_t */
static int cmp_seq(const void *a, const void *b)
{
    const seq_t *x = (const seq_t *)a;
    const seq_t *y = (const seq_t *)b;
    if (*x < *y) return -1;
    if (*x > *y) return 1;
    return 0;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* Static cache of all found numbers */
    static bool initialized = false;
    static seq_t *seq = NULL;
    static size_t seq_len = 0, seq_cap = 0;

    if (!initialized) {
        /* Pre‑compute powers d^e for d=1..9, e=1..9 */
        seq_t pow_tbl[10][10];
        for (int d = 1; d <= 9; ++d) {
            seq_t p = d;
            pow_tbl[d][1] = p;
            for (int e = 2; e <= 9; ++e) {
                p *= (seq_t)d;
                pow_tbl[d][e] = p;
            }
        }

        /* Generate all permutations of length 1..9 */
        uint8_t digits[9];
        for (int k = 1; k <= 9; ++k)
            gen_len(k, 0, 0, digits, 0,
                    pow_tbl,
                    &seq, &seq_len, &seq_cap);

        /* Sort the results ascending */
        qsort(seq, seq_len, sizeof(seq_t), cmp_seq);
        initialized = true;
    }

    if (n < 1 || (size_t)n > seq_len) return 0;   /* out of range */
    return seq[(size_t)(n - 1)];
}
