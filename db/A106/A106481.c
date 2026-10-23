/*
 * A106481  An Euler phi transform of 1/(1 - x^2).
 * https://oeis.org/A106481
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Sum_{k=0..n} phi(n-k+1)*(k mod 2) Equivalently: a(n) =
 *            sum_{j=1..n, j%2 == n%2} phi(j) (n>0) a(0) = 0
 * Range:     u8 n <= 34, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* Global static cache */
static idx_t max_n = 0;          /* largest n for which data are prepared */
static seq_t *phi_arr = NULL;    /* Euler totient values 1..max_n */
static seq_t *prefix_even = NULL;/* sum of phi(i) for even i <= idx */
static seq_t *prefix_odd  = NULL;/* sum of phi(i) for odd  i <= idx */

/* Recompute data up to new_max (inclusive) */
static void extend_to(idx_t new_max)
{
    if (new_max <= max_n)
        return;

    /* Allocate or grow arrays */
    seq_t *phi_new      = (seq_t *)realloc(phi_arr,
                                            (new_max + 1) * sizeof(seq_t));
    seq_t *even_new     = (seq_t *)realloc(prefix_even,
                                            (new_max + 1) * sizeof(seq_t));
    seq_t *odd_new      = (seq_t *)realloc(prefix_odd,
                                            (new_max + 1) * sizeof(seq_t));

    /* In case realloc fails */
    if (!phi_new || !even_new || !odd_new) {
        /* On failure, leave old data unchanged. */
        return;
    }

    phi_arr      = phi_new;
    prefix_even  = even_new;
    prefix_odd   = odd_new;

    /* Initialise phi[i] = i */
    phi_arr[0] = (seq_t)0;
    for (idx_t i = 1; i <= new_max; ++i)
        phi_arr[i] = (seq_t)i;

    /* Standard sieve to compute Euler's totient */
    for (idx_t p = 2; p <= new_max; ++p) {
        if (phi_arr[p] == (seq_t)p) {   /* p is prime */
            for (idx_t m = p; m <= new_max; m += p) {
                phi_arr[m] -= phi_arr[m] / (seq_t)p;
            }
        }
    }

    /* Build prefix sums for even and odd indices */
    prefix_even[0] = (seq_t)0;
    prefix_odd[0]  = (seq_t)0;
    for (idx_t i = 1; i <= new_max; ++i) {
        if ((i & 1u) == 0) {   /* even */
            prefix_even[i] = prefix_even[i - 1] + phi_arr[i];
            prefix_odd[i]  = prefix_odd[i - 1];
        } else {                /* odd */
            prefix_odd[i] = prefix_odd[i - 1] + phi_arr[i];
            prefix_even[i] = prefix_even[i - 1];
        }
    }

    max_n = new_max;
}

seq_t a(idx_t n)
{
    if (n == 0u)
        return (seq_t)0u;

    extend_to(n);

    return (n & 1u) ? prefix_odd[n] : prefix_even[n];
}
