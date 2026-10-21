/*
 * A205691  Numbers k for which 6 divides prime(k)-prime(j) for some j<k; each
 *          k occurs once for each such j.
 * https://oeis.org/A205691
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A205691: For each prime index k, output k exactly
   count_k times, where count_k is the number of preceding prime
   indices j<k with prime(j) ≡ prime(k) (mod 6).  Thus a(n) is the
   smallest k for which the cumulative number of outputs reaches n. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Append a prime to a dynamic array of uint64_t primes. */
static void append_prime(uint64_t **p_arr, size_t *cnt, size_t *cap, uint64_t p)
{
    if (*cnt >= *cap) {
        size_t newcap = (*cap == 0) ? 16 : (*cap << 1);
        uint64_t *tmp = (uint64_t *)realloc(*p_arr, newcap * sizeof(uint64_t));
        if (!tmp) abort();                 /* out of memory */
        *p_arr = tmp;
        *cap = newcap;
    }
    (*p_arr)[*cnt] = p;
    (*cnt)++;
}

seq_t a(idx_t n)
{
    if (n == 0)                      /* defined behaviour for offset start */
        return (seq_t)0;

    /* Dynamic array of primes seen so far.  The array is rebuilt on every
       call; for n<=10000 this is inexpensive. */
    uint64_t *primes = NULL;
    size_t prime_cnt = 0, prime_cap = 0;

    idx_t count1 = 0, count5 = 0;    /* counts of primes ≡1 or ≡5 (mod 6) */
    idx_t cum = 0;                   /* cumulative number of emitted terms */
    idx_t k = 1;                     /* current prime index (1‑based) */
    uint64_t p = 0;                  /* current prime value */

    for (;;) {
        /* --- Generate the k‑th prime --------------------------------- */
        if (k == 1) {
            p = 2;
        } else if (k == 2) {
            p = 3;
        } else {
            /* odd candidate > last prime */
            uint64_t cand = primes[prime_cnt - 1] + 2;
            bool isprime = false;
            while (!isprime) {
                if (cand % 2 == 0) { cand++; continue; }
                isprime = true;
                for (size_t i = 0; i < prime_cnt; ++i) {
                    uint64_t q = primes[i];
                    if (q * q > cand) break;
                    if (cand % q == 0) { isprime = false; break; }
                }
                if (!isprime) cand += 2;
            }
            p = cand;
        }

        /* Store the prime for future divisibility tests */
        append_prime(&primes, &prime_cnt, &prime_cap, p);

        /* --- Determine how many times to emit this k ---------------- */
        int rem = (int)(p % 6);
        idx_t m = 0;
        if (rem == 1)        m = count1;
        else if (rem == 5)   m = count5;
        /* residues 0,2,3,4 produce no output (since 2 is the only even prime) */

        cum += m;
        if (cum >= n) {
            free(primes);
            return (seq_t)k;
        }

        /* Update the residue counters used for future primes */
        if (rem == 1) count1++;
        else if (rem == 5) count5++;

        /* Prepare next prime index */
        k++;
    }
}
