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
 * Helpers:   int_sqrt, is_prime, push_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime.h"   /* bool is_prime(uint64_t n) */
#include "../utils/int_sqrt.h"   /* idx_t int_sqrt(idx_t x) */
#include "../utils/push_seq.h"   /* void push_seq(seq_t **arr, idx_t *cap, idx_t *size, seq_t val) */

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
        while (!is_prime(cand))
            ++cand;

        /* Convert to seq_t for storage. */
        seq_t p = (seq_t)cand;

        /* Compute bounds to see whether p appears in nextprime(k^2). */
        idx_t lower = int_sqrt(prev_prime);
        if ((idx_t)lower * lower < prev_prime)  /* ceil(sqrt(prev_prime)) */
            ++lower;
        idx_t upper = int_sqrt((idx_t)(cand - 1));  /* floor(sqrt(p-1)) */

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
