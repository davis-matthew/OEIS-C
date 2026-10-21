/*
 * A058215  Largest solution of phi(x) = 2^n.
 * https://oeis.org/A058215
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 14, u32 n <= 30, u64 n <= 62, u128 n <= 126,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Largest solution of φ(x) = 2ⁿ.
   For n ≥ 0 the maximal x has the form
      x = 2^m · ∏ pᵢ              (pᵢ are distinct Fermat primes)
   with   m = n – Σkᵢ + 1
   where kᵢ = log₂(pᵢ – 1).  
   Choosing the Fermat primes with the smallest kᵢ greedily yields the
   largest product, because each prime contributes a factor (1 + 2⁻ᵏᵢ). */
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Fermat primes and their corresponding k-values. */
    static const idx_t fermat_primes[] = {3, 5, 17, 257, 65537};
    static const idx_t k_vals[]        = {1, 2, 4, 8, 16};
    const size_t num_fprimes = sizeof fermat_primes / sizeof fermat_primes[0];

    idx_t sum_k = 0;          /* Σkᵢ of chosen primes */
    size_t t    = 0;          /* number of selected primes */

    /* Greedy selection: pick primes with the smallest kᵢ while Σkᵢ ≤ n. */
    while (t < num_fprimes && sum_k + k_vals[t] <= n)
    {
        sum_k += k_vals[t];
        ++t;
    }

    /* Exponent of the power‑of‑two part. (Always ≥ 1 for n ≥ 0.) */
    idx_t pow2 = n - sum_k + 1;

    /* Construct the result. */
    seq_t result = 1;
    for (idx_t i = 0; i < pow2; ++i)
        result *= 2;

    for (size_t i = 0; i < t; ++i)
        result *= fermat_primes[i];

    return result;
}
