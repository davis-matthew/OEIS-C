/*
 * A065966  Numbers k such that phi(k) / 2 is prime.
 * https://oeis.org/A065966
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 23, u16 n <= 759, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   calc_phi, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes numbers k such that phi(k)/2 is prime: OEIS A065966.
   The nth term a(n) is the nth integer k for which phi(k)/2 is a prime.
   The implementation evaluates phi(k) on demand using the provided
   calc_phi() routine and tests primality of phi(k)/2 with is_prime_big().
   It memoises the sequence values so that arbitrary call order is
   supported.  All operations are performed using seq_t so that the code
   works for the arbitrary‑precision build as well as for fixed width
   integer builds. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/calc_phi.h"
#include "../utils/is_prime_big.h"

/* Static state for the memoised sequence */
static seq_t *seq = NULL;     /* array of found k's */
static size_t seq_len = 0;    /* number of stored elements */
static size_t seq_cap = 0;    /* current allocation size */
static idx_t  next_k  = 1;    /* next integer to test */

/* Ensure the seq array can hold at least 'need' elements */
static void ensure_seq_cap(size_t need)
{
    if (seq_cap >= need) return;
    size_t new_cap = seq_cap ? seq_cap : 16;
    while (new_cap < need) new_cap *= 2;
    seq = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
    if (!seq) exit(1);
    seq_cap = new_cap;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined for n=0, but return 0 */

    /* Generate terms until we have at least n of them */
    while (seq_len < (size_t)n)
    {
        /* Compute phi(next_k) */
        seq_t phi_val = calc_phi((seq_t)next_k);

        /* Check if phi is even and phi/2 is prime */
        if ((phi_val & 1) == 0)
        {
            seq_t half = phi_val >> 1;      /* phi/2 */
            if (is_prime_big(half))
            {
                ensure_seq_cap(seq_len + 1);
                seq[seq_len++] = next_k;
            }
        }
        ++next_k;
    }
    return seq[n - 1];
}
