/*
 * A100714  Number of runs in binary expansion of A000040(n) (the n-th prime
 *          number) for n > 0.
 * https://oeis.org/A100714
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = number of runs in the binary representation of the n‑th
 *            prime number (A005811(A000040(n)))
 * Range:     u8 n <= 54, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   count_runs
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* NOTE: this implementation uses a simple incremental prime sieve that
   memorises all primes found so far.  It is suitable for the required
   limits (n up to about 10000).  It works for any seq_t that supports the
   usual arithmetic and modulus operators, and for any idx_t (here
   unsigned 64‑bit). */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/count_runs.h"

/* Between passes we keep a dynamic array of primes.  The array is
   grown lazily and reused across different calls to a().  The
   pointer and size counters are static, so the state is shared among
   all users of the routine. */

static seq_t *prime_list   = NULL;   /* array of found primes */
static idx_t  prime_count  = 0;      /* how many primes are stored */
static idx_t  prime_cap    = 0;      /* array capacity */
static seq_t  next_candidate = 2;    /* next odd integer to test */

static seq_t get_kth_prime(idx_t k)
{
    /* Grow the prime list until it contains at least k primes. */
    while (prime_count < k) {
        seq_t cand = next_candidate;
        bool   is_prime = true;

        /* Trial division by the primes we already know. */
        for (idx_t i = 0; i < prime_count; ++i) {
            seq_t p = prime_list[i];
            if (p * p > cand) break;          /* do not need to test larger p */
            if (cand % p == 0) { is_prime = false; break; }
        }

        if (is_prime) {
            /* Store the new prime. */
            if (prime_count == prime_cap) {
                idx_t new_cap = prime_cap ? prime_cap * 2 : 16;
                seq_t *tmp = (seq_t *)realloc(prime_list, new_cap * sizeof(seq_t));
                if (!tmp) abort(); /* allocation failed */
                prime_list = tmp;
                prime_cap  = new_cap;
            }
            prime_list[prime_count++] = cand;
        }

        /* Advance to the next candidate (skip even numbers after 2). */
        if (cand == 2)
            next_candidate = 3;
        else
            next_candidate += 2;
    }

    /* Return the k‑th prime (1‑based). */
    return prime_list[k - 1];
}

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* defined behaviour for offset < 1, not used */
    seq_t prime = get_kth_prime(n);
    return count_runs(prime);
}
