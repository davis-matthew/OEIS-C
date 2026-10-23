/*
 * A106477  Diagonal sums of Euler phi function sequence array.
 * https://oeis.org/A106477
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = sum_{k=0..floor(n/2)} phi(n-2k+1)
 * Range:     u8 n <= 33, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* External helper: φ(n) for n ≤ value of the local array.
   The array is filled by our own sieve and indexed as phi[n]. */
static seq_t *phi = NULL;
static idx_t phi_max = 0;

/* Allocate and sieve φ[0..size] (inclusive). */
static void recompute_phi(idx_t size)
{
    /* Allocate fresh storage.  Previous data, if any, is discarded. */
    free(phi);
    phi = (seq_t *)malloc((size + 1) * sizeof(seq_t));
    if (!phi)
        return;   /* allocation failure – never expected for small sizes */

    /* Initialise φ(i) with i; φ[0] is set to 0. */
    for (idx_t i = 0; i <= size; ++i)
        phi[i] = (seq_t)i;

    /* Classic Euler sieve. */
    for (idx_t i = 2; i <= size; ++i) {
        if (phi[i] == (seq_t)i) {                 /* i is prime          */
            const seq_t i_minus_1 = (seq_t)(i - 1);
            for (idx_t j = i; j <= size; j += i) {
                phi[j] = (phi[j] / (seq_t)i) * i_minus_1;
            }
        }
    }

    phi_max = size;
}

/* Ensure φ[0..size] is available.  Expands the table if necessary. */
static void ensure_phi(idx_t size)
{
    if (size <= phi_max)
        return;
    recompute_phi(size);
}

/* Sequence A106477: diagonal sums of Euler φ-function array. */
seq_t a(idx_t n)
{
    /* We need φ values up to n+1 at least. */
    ensure_phi(n + 1);

    seq_t sum = 0;
    const idx_t half = n / 2U;           /* floor(n/2)   */

    for (idx_t k = 0; k <= half; ++k) {
        const idx_t m = n - 2U * k + 1U;  /* m runs from n+1 down to 1 */
        sum += phi[m];
    }
    return sum;
}
