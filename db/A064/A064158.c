/*
 * A064158  Integers m such that (x1*x2*..xk)^(x1+x2+..xk) =
 *          (x1+x2+..xk)^(x1*x2*..xk) where x1x2..xk are the digits of m in
 *          base 10.
 * https://oeis.org/A064158
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 17, u16 n <= 71, u32 n <= 179, u64 n <= 179,
 *            u128 n <= 179, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A064158 – Numbers m such that (product of digits)^(sum of digits) =
   (sum of digits)^(product of digits). */

/* Explanation: A term m has digits d1…dk (each 1–9) with
 * product P = Π di and sum S = Σ di.
 * It satisfies the condition iff either P==S or (P==2 && S==4), which
 * captures the special three‑digit numbers 112, 121, 211.
 * The algorithm constructs numbers in increasing length (1,2,3,…) and
 *, for each length, performs a depth‑first search over all digit
 * combinations in lexicographic order, stopping when the global list
 * contains at least n terms.  Pruning cuts branches when the current
 * product already exceeds the maximal possible sum for the remaining
 * positions.
 */

#include <stdint.h>
#include <stdlib.h>

static seq_t *seq = NULL;          /* dynamic array of sequence values */
static size_t seq_cap = 0;         /* current capacity of seq            */
static size_t seq_len = 0;         /* number of values already generated  */

static idx_t cur_len = 0;          /* length of numbers currently generated */
#define MAX_LEN 20                /* safe upper bound for the required n */

/* Append value to the global array, resizing if necessary */
static void push_value(seq_t val)
{
    if (seq_len == seq_cap) {
        if (seq_cap == 0) seq_cap = 16;
        else seq_cap <<= 1;
        seq = (seq_t *)realloc(seq, seq_cap * sizeof(seq_t));
    }
    seq[seq_len++] = val;
}

/* Depth‑first search over numbers of fixed length `len`.
 * pos   – current position (0 … len)
 * val   – numeric value built so far
 * sum   – lexical sum of digits so far
 * prod  – product of digits so far
 */
static void dfs_len(idx_t len, idx_t pos, seq_t val, idx_t sum, seq_t prod)
{
    /* If we have built a complete number */
    if (pos == len) {
        if (prod == sum || (prod == (seq_t)2 && sum == (idx_t)4))
            push_value(val);
        return;
    }

    /* Prune: product already too large for any possible sum */
    if (prod > (seq_t)9 * len) return;
    /* Prune: even if we choose maximum digits the sum can't catch up with product */
    if (prod > sum + (idx_t)9 * (len - pos)) return;
    /* Prune: sum already exceeds the maximum reachable sum */
    if (sum > (idx_t)9 * len) return;

    /* Try each possible next digit (1‑9) in increasing order */
    for (int d = 1; d <= 9; ++d) {
        seq_t new_prod = prod * (seq_t)d;
        if (new_prod > (seq_t)9 * len) continue; /* skip this branch */
        idx_t new_sum = sum + (idx_t)d;
        seq_t new_val = val * (seq_t)10 + (seq_t)d;
        dfs_len(len, pos + 1, new_val, new_sum, new_prod);
    }
}

/* Generate all numbers of a given length */
static void generate_len(idx_t len)
{
    dfs_len(len, 0, 0, 0, 1);
}

/* Ensure that the sequence contains at least n elements. */
static void ensure_seq(idx_t n)
{
    if (seq_len >= n) return;
    while (seq_len < n && cur_len < MAX_LEN) {
        ++cur_len;
        generate_len(cur_len);
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for offset 0; safety */
    ensure_seq(n);
    return seq[n - 1];             /* a(1) is the first element of seq */
}
