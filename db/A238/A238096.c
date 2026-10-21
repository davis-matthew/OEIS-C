/*
 * A238096  a(n) = Sum_{k=2..n} floor(n/k)*floor((tau(k)+1)/2), where tau =
 *          A000005.
 * https://oeis.org/A238096
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=2..n} floor(n/k) * floor((tau(k)+1)/2)
 * Range:     u8 n <= 47, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where tau(k) is the number of positive divisors of k. */

#include <stdlib.h>
#include <string.h>

/* Global cache for divisor counts [0..max_precalc] */
static idx_t   max_precalc = 0;
static unsigned short *tau_arr = NULL;

/* Ensure tau_arr contains divisor counts for all k <= n */
static void ensure_tau(idx_t n)
{
    if (n <= max_precalc)
        return;        /* already prepared */

    /* Allocate array of size n+1 (indices 0..n) */
    unsigned short *new_arr = (unsigned short *)realloc(tau_arr,
                                (size_t)(n + 1) * sizeof(*new_arr));
    if (!new_arr)
        exit(EXIT_FAILURE);   /* allocation failed */
    tau_arr = new_arr;

    /* Initialize all entries to zero */
    memset(tau_arr, 0, (size_t)(n + 1) * sizeof(*tau_arr));

    /* Compute divisor counts using a divisor sieve */
    for (idx_t i = 1; i <= n; ++i) {
        for (idx_t j = i; j <= n; j += i)
            tau_arr[j]++;
    }

    max_precalc = n;
}

/* Sequence function */
seq_t a(idx_t n)
{
    if (n < 2)           /* a(1) = 0 */
        return (seq_t)0;

    ensure_tau(n);       /* make sure tau_arr[2..n] is ready */

    seq_t sum = 0;
    for (idx_t k = 2; k <= n; ++k) {
        unsigned short tau_k = tau_arr[k];
        /* ceil(tau_k/2) == (tau_k + 1) >> 1 */
        unsigned int weight = (tau_k + 1u) >> 1;
        seq_t term = (seq_t)(n / k) * (seq_t)weight;
        sum += term;
    }

    return sum;
}
