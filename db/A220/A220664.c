/*
 * A220664  First differences of A030299.
 * https://oeis.org/A220664
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = A030299(n+1) - A030299(n)
 * Range:     u8 n <= 8, u16 n <= 152, u32 n <= 5912, u64 n <= 5912,
 *            u128 n <= 5912, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

/* The build system supplies the types idx_t and seq_t. */
/* No other external includes or dependencies are used. */

static idx_t fact[20] = {0};
static int fact_precomputed = 0;

/* Precompute factorials 0! … 19! (fits in uint64_t).  */
static void precompute_fact(void)
{
    if (fact_precomputed) return;
    fact[0] = 1;
    for (int i = 1; i < 20; ++i)
        fact[i] = fact[i - 1] * (idx_t)i;
    fact_precomputed = 1;
}

/* Return the decimal value of the n-th permutation
   (1‑based index) in the sequence A030299.  */
static seq_t perm_value(idx_t n)
{
    precompute_fact();

    /* Find the block size k such that the n-th term is a
       permutation of [1..k].  */
    idx_t sum = 1;    /* offset for k = 1 */
    idx_t k = 1;
    while (1) {
        if (n < sum + fact[k]) break;
        sum += fact[k];
        ++k;
    }
    idx_t idx_in_k = n - sum;      /* 0‑based index inside the k‑permutations */

    /* Build the permutation digits using the factoradic system.  */
    int k_int = (int)k;                 /* small enough for stack array */
    int list[64];
    for (int i = 0; i < k_int; ++i)
        list[i] = i + 1;

    int remaining = k_int;
    seq_t val = 0;
    idx_t idx = idx_in_k;
    while (remaining > 0) {
        idx_t f = fact[remaining - 1];
        int pos = (int)(idx / f);
        int digit = list[pos];

        /* Append the digit to the decimal value.  */
        val = val * 10 + digit;

        /* Remove the used digit from the list.  */
        for (int j = pos; j < remaining - 1; ++j)
            list[j] = list[j + 1];
        --remaining;
        idx %= f;
    }
    return val;
}

/* External function required by the framework.  */
seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* undefined for index 0 */
    seq_t v1 = perm_value(n);
    seq_t v2 = perm_value(n + 1);
    return v2 - v1;
}
