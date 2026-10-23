/*
 * A174405  Partial sums of Sum_{k=1..n} n/gcd(n,k), or partial sums of
 *          Sum_{d|n} d*phi(d) (see A057660).
 * https://oeis.org/A174405
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∑_{k=1..n} k * φ(k) * ⌊ n / k ⌋ where φ is Euler's
 *            totient function. The function computes the nth term of sequence
 *            A174405
 * Range:     u8 n <= 9, u16 n <= 64, u32 n <= 2602, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* malloc, realloc */

/* static storage for Euler's totient values */
static idx_t *phi = NULL;   /* phi[1..max_phi] */
static idx_t max_phi = 0;   /* largest index of φ that has been computed */

/* Ensure φ[1..n] is available.  We recompute from scratch each time
   this routine is invoked with a larger n.  For n ≤ max_phi nothing
   is done.  The comments explain why this is safe and inexpensive
   for the required problem sizes. */
static void ensure_phi(idx_t n)
{
    if (n <= max_phi)
        return;   /* already available */

    /* Allocate storage for new indices */
    phi = (idx_t *)realloc(phi, (size_t)(n + 1) * sizeof(idx_t));
    for (size_t i = 0; i <= (size_t)n; ++i)
        phi[i] = (idx_t)i;          /* initial value i */

    /* Compute φ by a simple sieve */
    for (size_t p = 2; p <= (size_t)n; ++p)
    {
        if (phi[p] == p)            /* p is prime */
        {
            for (size_t m = p; m <= (size_t)n; m += p)
                phi[m] -= phi[m] / p;
        }
    }
    max_phi = n;
}

/* Sequence A174405: partial sums of ∑_{k=1..i} i/gcd(i,k) */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;   /* a(0) is undefined – return 0 for safety */

    ensure_phi(n);

    seq_t sum = 0;
    for (idx_t k = 1; k <= n; ++k)
    {
        /* term = k * φ(k) * floor(n / k) */
        seq_t term = (seq_t)k * (seq_t)phi[k] * (seq_t)(n / k);
        sum += term;
    }
    return sum;
}
