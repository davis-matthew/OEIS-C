/*
 * A070319  a(n) = Max_{k=1..n} tau(k) where tau(x)=A000005(x) is the number
 *          of divisors of x.
 * https://oeis.org/A070319
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = max_{k=1..n} τ(k), where τ(k) is the number of divisors
 *            of k
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation uses a divisor sieve that recomputes the whole
   array up to the requested n each time a larger value is asked for.  */

#include <stdlib.h>
#include <stdint.h>

static idx_t  cached_n = 0;               /* largest n for which we have computed */
static uint32_t *tau    = NULL;           /* τ values for 1 .. cached_n   */
static seq_t   *cummax = NULL;           /* cumulative maximums         */

seq_t a(idx_t n)
{
    /* The OEIS offset is 1 – return 0 for n == 0 regardless of the
       definition of the sequence. */
    if (n == 0)
        return 0;

    /* If we already have a table covering this n, simply return the
       stored result. */
    if (n <= cached_n)
        return cummax[(size_t)n];

    /* Extend the tables to size n+1 (indices 0..n). */
    size_t newsize = (size_t)n + 1;     /* 0 is unused but convenient */

    /* Reallocate the τ array.  The cast is required for C++ compliance. */
    uint32_t *new_tau  = (uint32_t*)realloc(tau, newsize * sizeof(uint32_t));
    seq_t   *new_cummax = (seq_t*)realloc(cummax, newsize * sizeof(seq_t));
    if (!new_tau || !new_cummax) {      /* allocation failed */
        return 0;
    }
    tau    = new_tau;
    cummax = new_cummax;

    /* Initialise τ entries to 0. */
    for (size_t i = 0; i < newsize; ++i)
        tau[i] = 0;

    /* Divisor sieve: for each i, increment τ(j) for all multiples j of i. */
    for (size_t i = 1; i < newsize; ++i) {
        for (size_t j = i; j < newsize; j += i)
            ++tau[j];
    }

    /* Build cumulative maximum array cummax[n] = max_{1≤k≤n} τ(k). */
    seq_t curmax = 0;
    cummax[0] = 0;                       /* unused but handy */
    for (size_t i = 1; i < newsize; ++i) {
        if ((seq_t)tau[i] > curmax)
            curmax = (seq_t)tau[i];
        cummax[i] = curmax;
    }

    cached_n = n;
    return cummax[(size_t)n];
}
