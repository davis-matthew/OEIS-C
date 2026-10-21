/*
 * A205147  The index j<k such that n divides s(k)-s(j), where k is the least
 *          index (A205146) for which such j exists, and
 *          s(k)=prime(k)*prime(k+1).
 * https://oeis.org/A205147
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 60, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence A205147:
   For a given n, let s(k) = prime(k) * prime(k+1).
   Find the minimal k for which there exists j < k with n | s(k) – s(j).
   Return a(n) = j + 1 (indices are 1‑based).  This is equivalent to
   finding the first repeated residue of s(k) modulo n; the later index
   giving the repeat is k and the earlier occurrence index is j.  */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#define PRIME_UPPER 200000      /* enough to contain at least 10002 primes */

/* Global prime list, initialized once */
static uint32_t prime_list[PRIME_UPPER + 1];
static size_t prime_cnt = 0;

/* Build the prime list up to PRIME_UPPER, if not already done */
static void init_primes(void)
{
    if (prime_cnt > 0)
        return;                /* already initialized */

    bool *is_prime = (bool *)calloc(PRIME_UPPER + 1, sizeof(bool));
    if (!is_prime)
        return;                /* allocation failed – leave empty */

    for (size_t i = 2; i <= PRIME_UPPER; ++i)
        is_prime[i] = true;

    for (size_t i = 2; i * i <= PRIME_UPPER; ++i) {
        if (is_prime[i]) {
            for (size_t j = i * i; j <= PRIME_UPPER; j += i)
                is_prime[j] = false;
        }
    }

    for (size_t i = 2; i <= PRIME_UPPER; ++i) {
        if (is_prime[i]) {
            prime_list[prime_cnt++] = (uint32_t)i;
        }
    }
    free(is_prime);
}

/* Compute a(n) */
seq_t a(idx_t n)
{
    uint64_t N = (uint64_t)n;          /* use 64‑bit for modulo operations */

    /* Ensure primes are ready; we need at least N+2 primes */
    init_primes();
    if (prime_cnt < N + 2) {
        /* In the rare case our upper bound was insufficient, enlarge
           the prime list.  (The sequence is only required for n <= ~10000.)
           This fallback recomputes a larger sieve. */
        size_t limit = PRIME_UPPER * 2;
        bool *is_prime = (bool *)calloc(limit + 1, sizeof(bool));
        for (size_t i = 2; i <= limit; ++i)
            is_prime[i] = true;
        for (size_t i = 2; i * i <= limit; ++i) {
            if (is_prime[i]) {
                for (size_t j = i * i; j <= limit; j += i)
                    is_prime[j] = false;
            }
        }
        prime_cnt = 0;
        for (size_t i = 2; i <= limit; ++i) {
            if (is_prime[i]) {
                prime_list[prime_cnt++] = (uint32_t)i;
            }
        }
        free(is_prime);
    }

    /* Allocate array to record the first occurrence of each residue mod N */
    int *first_index = (int *)malloc((size_t)N * sizeof(int));
    if (!first_index)
        return (seq_t)0;                 /* allocation failure – return 0 */

    for (size_t i = 0; i < N; ++i)
        first_index[i] = -1;

    /* Start with k = 0 (s(0) = prime0 * prime1) */
    uint64_t resid = (uint64_t)prime_list[0] * prime_list[1] % N;
    first_index[resid] = 0;             /* index 0 */

    size_t k = 1;
    for (;;) {
        /* Compute s(k) mod N */
        resid = ((uint64_t)prime_list[k] * prime_list[k + 1]) % N;
        int j = first_index[resid];
        if (j >= 0) {
            /* Found first repeat: j is the earlier index, k is the later */
            free(first_index);
            return (seq_t)(j + 1);       /* convert to 1‑based OEIS index */
        }
        first_index[resid] = (int)k;    /* record occurrence of this residue */
        ++k;
    }
}
