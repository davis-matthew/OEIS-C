/*
 * A144581  Triangular numbers k*(k+1)/2 such that (number of prime factors of
 *          k) + (number of prime factors of k+1) is composite.
 * https://oeis.org/A144581
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th triangular number k*(k+1)/2 for which
 *            Ω(k)+Ω(k+1) is composite (Ω = number of prime factors, counted
 *            with multiplicity)
 * Range:     u8 n <= 10, u16 n <= 197, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   count_prime_factors, is_prime_big, seq_alloc_new_array,
 *            seq_free_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/count_prime_factors.h"   /* seq_t count_prime_factors(seq_t m); */
#include "../utils/is_prime_big.h"          /* bool is_prime_big(seq_t n); */
#include "../utils/seq_alloc_new_array.h"   /* seq_t *seq_alloc_new_array(size_t n); */
#include "../utils/seq_free_array.h"        /* void seq_free_array(seq_t *p); */

seq_t a(idx_t n)
{
    /* Static state for memoisation of the sequence terms. */
    static idx_t last_k = 0;      /* last tested k (i.e. the last k that was processed) */
    static idx_t found  = 0;      /* number of terms already stored in the cache */
    static seq_t *buf   = NULL;   /* array of accepted triangular numbers */
    static size_t buf_cap = 0;    /* current capacity of |buf| */

    /* Extend the cache until it contains at least n terms. */
    while (found < n) {
        /* Candidate k. */
        idx_t k = ++last_k;

        /* Ω(k) and Ω(k+1). */
        seq_t omega_k  = count_prime_factors((seq_t)k);
        seq_t omega_k1 = count_prime_factors((seq_t)(k + 1));

        /* Sum of the Ω values.  We need it as an idx_t so later we can test
           primality and composite status using the provided helpers. */
        idx_t sum = (idx_t)omega_k + (idx_t)omega_k1;

        /* The sum must be >1 (i.e. not 0 or 1) and not prime.  If it is
           prime or <=1 we skip this k. */
        if (sum <= 1 || is_prime_big((seq_t)sum))
            continue;  /* not valid; try next k */

        /* Compute the triangular number using seq_t arithmetic. */
        seq_t tri = ((seq_t)k) * ((seq_t)k + 1) / 2;

        /* Grow the buffer if necessary (doubling strategy). */
        if (found == buf_cap) {
            size_t new_cap = buf_cap == 0 ? 64 : buf_cap << 1;
            seq_t *new_buf = seq_alloc_new_array(new_cap);
            for (size_t i = 0; i < found; ++i)
                new_buf[i] = buf[i];
            if (buf != NULL)
                seq_free_array(buf);
            buf      = new_buf;
            buf_cap  = new_cap;
        }

        /* Store the new triangular number and move to the next slot. */
        buf[found++] = tri;
    }

    /* a(1) corresponds to buf[0] because indices are 1‑based in the OEIS
       definition. */
    return buf[n - 1];
}
