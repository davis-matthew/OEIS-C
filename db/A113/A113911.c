/*
 * A113911  Prime numbers not appearing in the nextprime(x^2) sequence
 *          A007491.
 * https://oeis.org/A113911
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n-th prime p such that there is no integer k * with
 *            nextprime(k^2)=p. This is tested by checking whether *
 *            ceil(sqrt(prev_prime)) ≤ floor(sqrt(p-1)). If the inequality
 *            fails * the prime p does not appear in the nextprime(x^2)
 *            sequence and is * added to a(n). The sequence is generated o...
 * Range:     u8 n <= 39, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Validated: partial, pass, 56 terms through n = 56 (2026-11-03)
 * Helpers:   is_prime_u64, isqrt, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/is_prime_u64.h"
#include "../utils/isqrt.h"
#include "../utils/mem.h"

/* Dynamic array helpers. */
static void push_seq(seq_t **arr, idx_t *cap, idx_t *size, seq_t val)
{
    if (*size >= *cap) {
        size_t new_cap = (*cap == 0) ? 4 : (*cap << 1);
        *arr = mem_resize(seq_t, *arr, new_cap);
        *cap = new_cap;
    }
    (*arr)[*size] = val;
    (*size)++;
}

seq_t a(idx_t n)
{
    /* Return 0 for indices below the offset – behaviour below offset is
     * unspecified and allowed to be anything. */
    if (n < 1) return 0;

    /* Static state that survives between calls. */
    static seq_t *seq = NULL;      /* array of computed terms */
    static idx_t seq_len = 0;      /* number of terms stored */
    static idx_t seq_cap = 0;      /* allocated capacity of seq */

    static uint64_t cand = 3;      /* next integer to test for primality */
    static uint64_t prev_prime = 2;/* previous prime seen */

    /* Generate terms until we have at least n of them. */
    while (seq_len < n)
    {
        /* Find next prime among successive integers. */
        while (!is_prime_u64(cand))
            ++cand;

        /* Convert to seq_t for storage. */
        seq_t p = (seq_t)cand;

        /* Compute bounds to see whether p appears in nextprime(k^2). */
        idx_t lower = isqrt(prev_prime);
        if ((idx_t)lower * lower < prev_prime)  /* ceil(sqrt(prev_prime)) */
            ++lower;
        idx_t upper = isqrt((idx_t)(cand - 1));  /* floor(sqrt(p-1)) */

        /* If no integer k satisfies ceil(sqrt(prev_prime)) ≤ k ≤ floor(sqrt(p-1)),
         * then p does not appear in the nextprime(x^2) sequence. */
        if (lower > upper)
            push_seq(&seq, &seq_cap, &seq_len, p);

        /* Update state for the next prime. */
        prev_prime = cand;
        ++cand;
    }

    /* Return the requested term. */
    return seq[n - 1];
}
