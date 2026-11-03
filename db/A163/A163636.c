/*
 * A163636  The sum of all odd numbers from 2n-1 up to the n-th odd nonprime.
 * https://oeis.org/A163636
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (A014076(n)+2*n-1)*(A014076(n)-2*n+3)/4
 * Range:     u8 n <= 7, u16 n <= 211, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 62 terms through n = 969 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n): sum of all odd numbers from 2n-1 up to the n‑th odd nonprime. */

#include <stddef.h>
#include <stdbool.h>

/* Find the n‑th odd nonprime (1‑indexed).  The sequence includes 1
 * and all odd composite numbers. */
static seq_t nth_odd_nonprime(idx_t n)
{
    idx_t   count      = 0;          /* how many nonprimes found so far */
    seq_t   candidate  = (seq_t)1;   /* current odd number to test */

    while (1) {
        bool isComposite = false;

        if (candidate == (seq_t)1) {
            /* 1 is considered a nonprime by definition. */
            isComposite = true;
        } else {
            /* test whether candidate is prime. */
            bool isPrime = true;
            for (seq_t d = (seq_t)3; d <= candidate / d; d += (seq_t)2) {
                if (candidate % d == (seq_t)0) {
                    isPrime = false;
                    break;
                }
            }
            if (!isPrime)
                isComposite = true;
        }

        if (isComposite) {
            ++count;
            if (count == n)
                return candidate;
        }
        candidate += (seq_t)2;   /* move to the next odd number */
    }
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0)                 /* undefined for n = 0; return 0 safely */
        return (seq_t)0;

    seq_t odd_n = nth_odd_nonprime(n);

    seq_t term = ((odd_n + ((seq_t)2 * (seq_t)n - (seq_t)1)) *
                  (odd_n - ((seq_t)2 * (seq_t)n) + (seq_t)3)) / (seq_t)4;
    return term;
}
