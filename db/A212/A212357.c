/*
 * A212357  Coefficients for the cycle index polynomial for the cyclic group
 *          C_n multiplied by n, n>=1, read as partition polynomial.
 * https://oeis.org/A212357
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   ensure_capacity, is_uniform, phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = coefficient of the n‑th partition (ordered in Abramowitz–Stegun
 * order) of some m in the flat list of coefficients of m·Z(C_m).
 * The coefficient is φ(k) if the partition is k repeated m/k times, 0 otherwise.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/phi.h"              /* seq_t phi(idx_t m) */
#include "../utils/ensure_capacity.h" /* void ensure_capacity(size_t, size_t*, void**, size_t) */
#include "../utils/is_uniform.h"

/* Storage for computed coefficients, built incrementally. */
static seq_t  *coeffs = NULL;
static size_t  coeff_cap = 0;
static size_t  coeff_len = 0;

/* Highest n for which rows have been generated. */
static idx_t  last_row = 0;

/* Append a coefficient, growing the table if necessary. */
static void append_coeff(seq_t value)
{
    size_t needed = coeff_len + 1;
    if (needed > coeff_cap)
        ensure_capacity(needed, &coeff_cap, (void **)&coeffs, sizeof(seq_t));
    coeffs[coeff_len++] = value;
}

/* Process a partition: compute its coefficient and store it. */
static void process_partition(const int *parts, int len)
{
    seq_t coeff = 0;
    if (is_uniform(parts, len))
        coeff = phi((idx_t)parts[0]);   /* φ(k) for the repeated part k */
    append_coeff(coeff);
}

/* Generate all partitions of rem into parts_left parts, each ≤ max_part,
 * with the remaining parts non‑increasing. Arguments are 0‑based for parts[]. 
 * The partitions are produced in lexicographically non‑increasing order
 * within the fixed length constraint. */
static void gen_fixed_len_parts(int rem, int parts_left, int max_part,
                                int *parts, int pos)
{
    if (parts_left == 0) {
        if (rem == 0)
            process_partition(parts, pos);
        return;
    }

    /* If the remaining sum is insufficient or excessive for the remaining parts,
       prune the search. */
    if (rem < parts_left) return;                    /* each part ≥ 1 */
    if (rem > parts_left * max_part) return;         /* cannot keep <= max_part */

    int upper = rem < max_part ? rem : max_part;
    for (int v = upper; v >= 1; --v) {
        /* After picking v, the remaining sum must be enough for the remaining
           parts (each at least 1). If not, skip to the next smaller v. */
        if (rem - v < parts_left - 1) continue;
        parts[pos] = v;
        gen_fixed_len_parts(rem - v, parts_left - 1, v, parts, pos + 1);
    }
}

/* Generate all partitions of n (in Abramowitz–Stegun order) and compute
 * their coefficients. */
static void generate_row(idx_t n)
{
    int parts[64];   /* maximum length needed for n up to about 200 */
    for (int len = 1; len <= (int)n; ++len)
        gen_fixed_len_parts((int)n, len, (int)n, parts, 0);
}

/* Public function returning the n-th term of the sequence (1‑based). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;         /* undefined for n < 1 */

    /* Generate rows until we have at least n terms. */
    while (coeff_len < (size_t)n) {
        ++last_row;
        generate_row(last_row);
    }
    return coeffs[n - 1];
}
