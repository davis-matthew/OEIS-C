/*
 * A096065  Let p(k) = k-th prime; sequence gives primes q of the form q =
 *          k*p(k) - 1 for some k.
 * https://oeis.org/A096065
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 11, u32 n <= 12, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   get_prime, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: primes q of the form q = k * p(k) – 1, where p(k) is the k‑th ordinary prime.
   The sequence lists, in increasing order of k, each prime q found.  a(0) = 5. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/get_prime.h"
#include "../utils/is_prime_big.h"

/* dynamic storage for the sequence values */
static seq_t *seqvals = NULL;      /* array of sequence elements   */
static idx_t  seqsize = 0;        /* number of elements stored    */
static idx_t  seqcap  = 0;        /* allocated capacity           */
static idx_t  next_k  = 1;        /* next k to be processed      */

/* enlarge the buffer if necessary */
static void grow_seq(idx_t new_cap)
{
    if (new_cap <= seqcap) return;
    seqcap = new_cap > seqcap ? new_cap * 2 : 16U;
    seqvals = (seq_t *)realloc(seqvals, seqcap * sizeof(seq_t));
}

/* generate sequence terms until index n is available */
static void compute_until(idx_t n)
{
    while (seqsize <= n)
    {
        /* compute k * p(k) – 1   (k = next_k) */
        seq_t pk  = (seq_t)get_prime(next_k);       /* ordinary prime p(k) */
        seq_t cand = (seq_t)next_k * pk - 1U;        /* q = k * p(k) – 1   */

        if (is_prime_big(cand))                     /* primality test */
        {
            if (seqsize == seqcap)                 /* ensure storage */
                grow_seq(seqcap ? seqcap * 2 : 16U);
            seqvals[seqsize++] = cand;
        }
        ++next_k;
    }
}

/* required API function ---------------------------------------------- */
seq_t a(idx_t n)
{
    compute_until(n);
    return seqvals[n];
}
