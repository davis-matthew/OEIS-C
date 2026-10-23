/*
 * A195558  Hypotenuses of primitive Pythagorean triples in A195556 and
 *          A195557.
 * https://oeis.org/A195558
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 19, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A195558: Hypotenuses of primitive Pythagorean triples
   (see OEIS for definition).  
   a(1) = 1.  
   Differences d_k are defined by
        d_0 = 12,  d_1 = 456,  d_{k+1} = 38*d_k - d_{k-1}   (k ≥ 1)
   Each d_k is used three times in succession.  Thus
        a(n) = 1 + Σ_{k=0}^{⌊(n-2)/3⌋} d_k · min(3, n-1-3k).
   The function below computes a(n) for any n ≥ 1 using only integer
   operations on type seq_t. */

#include <stdint.h>

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;     /* out‑of‑range, not used */
    if (n == 1) return (seq_t)1;

    idx_t steps = n - 1;             /* total number of differences to use */
    seq_t sum = (seq_t)0;

    /* Initial differences */
    const seq_t d0 = (seq_t)12;
    const seq_t d1 = (seq_t)456;

    /* Variables for the recurrence d_{k+1} = 38*d_k - d_{k-1} */
    seq_t prev = d0;                 /* d_{k-1} when computing next */
    seq_t curr = d1;                 /* d_k          */
    seq_t dk;                        /* current difference to use */
    idx_t j = 0;                     /* index of current d_k */

    while (steps > 0) {
        if (j == 0) {
            dk = d0;
        } else if (j == 1) {
            dk = d1;
        } else {
            /* compute next difference using the recurrence */
            dk = curr * (seq_t)38 - prev;
            prev = curr;
            curr = dk;
        }

        /* Determine how many times this difference is applied (≤ 3) */
        idx_t use = (steps < (idx_t)3) ? steps : (idx_t)3;
        sum += dk * (seq_t)use;
        steps -= use;
        ++j;
    }

    return (seq_t)1 + sum;
}
