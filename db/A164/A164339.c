/*
 * A164339  Write down the primes dividing n (with repetition) in an exponent
 *          tower (see comment). a(n) = the smallest possible value of such a
 *          tower.
 * https://oeis.org/A164339
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = minimal exponent‑tower value obtained by permuting the
 *            multiset of prime factors of n (each prime repeated as many
 *            times as its exponent)
 * Range:     u8 n <= 15, u16 n <= 15, u32 n <= 26, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   big_pow, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function below implements a depth‑first search over the
   multiset of remaining prime factors, using memoisation to avoid
   recomputing sub‑problems.  It works for any seq_t type, including
   arbitrary‑precision integer classes, and compiles in both C11
   and C++17. */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/factor_u64.h"
#include "../utils/big_pow.h"

/* Recursive helper that returns the minimal tower value for the
   current multiset of remaining prime factors.
   @param state   counts of remaining copies of each prime.
   @param uniq    number of distinct primes in the original factorisation.
   @param primes  array of the distinct primes.
   @param mult    multiplier array for linear indexing of state vectors.
   @param memo    memoisation table indexed by the linear index of `state`. */
static seq_t dfs(int *state, size_t uniq, uint64_t *primes,
                 size_t *mult, seq_t *memo)
{
    size_t idx = 0;
    for (size_t i = 0; i < uniq; ++i)
        idx += (size_t)state[i] * mult[i];

    if (memo[idx] != (seq_t)0)          /* already computed */
        return memo[idx];

    /* Determine whether any primes remain. */
    size_t sum = 0;
    for (size_t i = 0; i < uniq; ++i)
        sum += (size_t)state[i];
    if (sum == 0) {
        memo[idx] = (seq_t)1;
        return memo[idx];
    }

    seq_t best = 0;              /* 0 means “not set yet” */

    for (size_t i = 0; i < uniq; ++i) {
        if (state[i] == 0)
            continue;

        /* Use primes[i] as the top base of the tower. */
        state[i]--;
        seq_t exp = dfs(state, uniq, primes, mult, memo);
        state[i]++;                     /* restore the state */

        seq_t val = big_pow((seq_t)primes[i], exp);

        if (best == 0 || val < best)
            best = val;
    }

    memo[idx] = best;
    return best;
}

seq_t a(idx_t n)
{
    if (n <= 1)               /* a(1) = 1 by definition */
        return (seq_t)1;

    /* Factorise n.  primes[k] and exps[k] hold the k‑th prime factor
       and its exponent; uniq is the number of distinct prime factors. */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t uniq = 0;
    factor_u64((uint64_t)n, primes, exps, &uniq);

    /* Allocate state vector (counts of remaining prime copies). */
    int *state = (int *)malloc(uniq * sizeof(int));
    if (!state)
        return (seq_t)0;                 /* allocation failure */

    for (size_t i = 0; i < uniq; ++i)
        state[i] = (int)exps[i];

    /* Multiplier array for linear indexing of state vectors. */
    size_t *mult = (size_t *)malloc(uniq * sizeof(size_t));
    if (!mult) {
        free(state);
        return (seq_t)0;
    }
    mult[0] = 1;
    for (size_t i = 1; i < uniq; ++i)
        mult[i] = mult[i - 1] * ((size_t)exps[i - 1] + 1);

    /* Total number of possible states. */
    size_t total_states = mult[uniq - 1] * ((size_t)exps[uniq - 1] + 1);

    /* Memoisation table initialized to 0 (uncomputed). */
    seq_t *memo = (seq_t *)calloc(total_states, sizeof(seq_t));
    if (!memo) {
        free(state);
        free(mult);
        return (seq_t)0;
    }

    seq_t result = dfs(state, uniq, primes, mult, memo);

    free(state);
    free(mult);
    free(memo);

    return result;
}
