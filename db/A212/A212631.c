/*
 * A212631  Number of dominating subsets of the rooted tree with Matula-Goebel
 *          number n.
 * https://oeis.org/A212631
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 67, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   build_primes_upto, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = number of dominating subsets of the rooted tree with Matula‐Goebel number n.
 * Recurrence (used in a DFS over the Mathula number n):
 *   For a node that has children of types child_i repeated e_i times, let
 *
 *     S  = Π_i (A_i + B_i + C_i)^{e_i}
 *     T  = Π_i (A_i + B_i)^{e_i}
 *     U  = Π_i (B_i)^{e_i}
 *
 *   where (A_i,B_i,C_i) are respectively
 *       the numbers of selections in which the child is selected,
 *       dominated by its child, or dominated by its parent.
 *
 *   Then for that node we set
 *
 *     A = S                 (node is selected)
 *     B = T - U             (node is not selected but is dominated by a child)
 *     C = U                 (node is not selected and dominated by its parent)
 *
 *   The answer for n is A + B, because the root has no parent.
 *
 *   The Matula number n is translated into a rooted tree by:
 *     - n = 1 => leaf
 *     - n > 1 factorised as n = p_1^{e_1} ... p_k^{e_k}
 *       where each p_i is a prime.
 *       Let t_i be the index of prime p_i in the ordered list of primes
 *       (2 is the first prime, 3 the second, etc.).
 *       Then the root has e_i copies of a child subtree whose Matula number
 *       is t_i.  Recursion is applied to each child.
 *
 *   Complexity:  O(n log n) for the first call and O(1) for repeated calls
 *   because of memoisation.  Works for n up to at least 10 000 in well
 *   under two seconds on a typical machine.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#include "../utils/factor_u64.h"          /* factor_u64(uint64_t n, uint64_t *pr, uint64_t *ex, size_t *cnt); */
#include "../utils/build_primes_upto.h"   /* build_primes_upto(idx_t limit, uint64_t **primes_out, size_t *cnt_out); */

static seq_t *A_tbl = NULL;
static seq_t *B_tbl = NULL;
static seq_t *C_tbl = NULL;
static unsigned char *computed = NULL;   /* 0 = not computed, 1 = computed */
static size_t memo_cap = 0;

/* Prime table: primes[i] is the (i+1)-th prime (starting from 0). */
static uint64_t *prime_tbl = NULL;
static size_t prime_cnt = 0;
static idx_t prime_limit = 0;

/* Ensure there is room for indices up to n (inclusive). */
static void ensure_memo(idx_t n)
{
    if (n < memo_cap)
        return;
    size_t new_cap = (size_t)n + 1;
    new_cap <<= 1;   /* grow by factor of 2 */
    seq_t *na = (seq_t *)realloc(A_tbl, new_cap * sizeof(seq_t));
    seq_t *nb = (seq_t *)realloc(B_tbl, new_cap * sizeof(seq_t));
    seq_t *nc = (seq_t *)realloc(C_tbl, new_cap * sizeof(seq_t));
    unsigned char *ncum = (unsigned char *)realloc(computed, new_cap * sizeof(unsigned char));
    if (!na || !nb || !nc || !ncum) {
        abort();   /* memory error */
    }
    A_tbl = na;
    B_tbl = nb;
    C_tbl = nc;
    computed = ncum;
    for (size_t i = memo_cap; i < new_cap; ++i) {
        A_tbl[i] = 0;
        B_tbl[i] = 0;
        C_tbl[i] = 0;
        computed[i] = 0;
    }
    memo_cap = new_cap;
}

/* Make sure primes up to 'limit' are available. */
static void ensure_primes(idx_t limit)
{
    if (limit <= prime_limit)
        return;
    if (prime_tbl)
        free(prime_tbl);
    build_primes_upto(limit, &prime_tbl, &prime_cnt);
    prime_limit = limit;
}

/* Return 1‑based index of prime p in the prime table; 0 on failure. */
static idx_t prime_index(uint64_t p)
{
    if (prime_tbl == NULL)
        return 0;
    size_t lo = 0, hi = prime_cnt;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        if (prime_tbl[mid] == p)
            return (idx_t)(mid + 1);
        if (prime_tbl[mid] < p)
            lo = mid + 1;
        else
            hi = mid;
    }
    return 0;   /* should not happen for valid primes */
}

/* Recursive DP over Matula number n; fills A_tbl[n], B_tbl[n], C_tbl[n]. */
static void compute_dp(idx_t n)
{
    if (n == 0)
        return;
    ensure_memo(n);
    if (computed[n])
        return;

    if (n == 1) {
        A_tbl[1] = 1;
        B_tbl[1] = 0;
        C_tbl[1] = 1;
        computed[1] = 1;
        return;
    }

    uint64_t pr[64], ex[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, pr, ex, &cnt);

    /* Ensure primes for each factor */
    for (size_t i = 0; i < cnt; ++i)
        ensure_primes((idx_t)pr[i]);

    seq_t prod_abc = 1;
    seq_t prod_ab  = 1;
    seq_t prod_b   = 1;

    for (size_t i = 0; i < cnt; ++i) {
        uint64_t prime_factor = pr[i];
        unsigned int e = (unsigned int)ex[i];
        idx_t child_t = prime_index(prime_factor);

        compute_dp(child_t);

        seq_t sum_abc = A_tbl[child_t] + B_tbl[child_t] + C_tbl[child_t];
        seq_t sum_ab  = A_tbl[child_t] + B_tbl[child_t];
        seq_t sum_b   = B_tbl[child_t];

        seq_t pow_abc = 1;
        seq_t pow_ab  = 1;
        seq_t pow_b   = 1;
        for (unsigned int k = 0; k < e; ++k) {
            pow_abc *= sum_abc;
            pow_ab  *= sum_ab;
            pow_b   *= sum_b;
        }

        prod_abc *= pow_abc;
        prod_ab  *= pow_ab;
        prod_b   *= pow_b;
    }

    A_tbl[n] = prod_abc;
    B_tbl[n] = prod_ab - prod_b;
    C_tbl[n] = prod_b;
    computed[n] = 1;
}

/* Public interface required by the system. */
seq_t a(idx_t n)
{
    compute_dp(n);
    return A_tbl[n] + B_tbl[n];
}
