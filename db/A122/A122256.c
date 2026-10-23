/*
 * A122256  Number of numbers <= n with 3-smooth Euler's totient (A000010).
 * https://oeis.org/A122256
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 72, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A122256: Number of integers m ≤ n with φ(m) a 3‑smooth number
   (i.e. φ(m) = 2^a ⋅ 3^b for some a,b ≥ 0).  The function
   computes this by sieving Euler's totient function up to n and then
   counting the smooth values. */
#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;

    /* Allocate array for φ(0..n).  φ[0] unused. */
    seq_t *phi = (seq_t *)malloc((size_t)(n + 1) * sizeof(seq_t));
    if (!phi) return 0;  /* fallback on allocation failure */

    /* Initialise φ[i] = i. */
    for (idx_t i = 0; i <= n; ++i)
        phi[i] = (seq_t)i;

    /* Sieve totients in O(n log log n). */
    for (idx_t p = 2; p <= n; ++p) {
        if (phi[p] == (seq_t)p) {          /* p is prime */
            for (idx_t k = p; k <= n; k += p)
                phi[k] -= phi[k] / (seq_t)p;
        }
    }

    /* Count how many φ(i) are 3‑smooth. */
    seq_t count = 0;
    for (idx_t i = 1; i <= n; ++i) {
        seq_t x = phi[i];
        while ((x & 1) == 0)      /* divide by 2 while even */
            x >>= 1;
        while (x % 3 == 0)
            x /= 3;
        if (x == 1)
            ++count;
    }

    free(phi);
    return count;
}
