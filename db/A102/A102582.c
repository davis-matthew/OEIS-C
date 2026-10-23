/*
 * A102582  Numbers n such that denominator of Sum_{k=0..4n+1} 1/k! is
 *          (4n+1)!/2.
 * https://oeis.org/A102582
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   the nth term of A102582 is the n-th integer n such that the
 *            denominator of Σ_{k=0}^{4n+1} 1/k! equals (4n+1)!/2. The
 *            function a(idx_t n) returns the n‑th such integer. It generates
 *            terms on demand and memoizes them
 * Range:     u8 n <= 27, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

#ifndef MAX_M
# define MAX_M 500000        /* enough for the first 10000 terms */
#endif

/* Utility to compute a simple sieve of primes up to MAX_M.  */
static void build_primes(int **primes_out, size_t *prime_count_out)
{
    bool *is_prime = (bool*)malloc((MAX_M + 1) * sizeof(bool));
    for (int i = 0; i <= MAX_M; i++) is_prime[i] = true;
    is_prime[0] = is_prime[1] = false;

    for (int p = 2; (long long)p * p <= MAX_M; ++p) {
        if (is_prime[p]) {
            for (int q = p * p; q <= MAX_M; q += p) is_prime[q] = false;
        }
    }

    size_t cap = 1;
    *prime_count_out = 0;
    *primes_out = (int*)malloc(cap * sizeof(int));

    for (int i = 2; i <= MAX_M; ++i) {
        if (is_prime[i]) {
            if (*prime_count_out == cap) {
                cap <<= 1;
                *primes_out = (int*)realloc(*primes_out, cap * sizeof(int));
            }
            (*primes_out)[(*prime_count_out)++] = i;
        }
    }

    free(is_prime);
}

seq_t a(idx_t n)
{
    /* ----------------------------------------------------------------------
       Static data for memoization and for the prime table.  All static
       variables are lazily initialized on the first call.
       ---------------------------------------------------------------------- */
    static bool initialized = false;

    static int *primes = NULL;            /* prime numbers */
    static size_t prime_count = 0;        /* number of primes */
    static seq_t *residues = NULL;       /* residues M_m mod p */

    static idx_t processed_m = 0;         /* last m that was processed */
    static seq_t *seq = NULL;             /* array of sequence terms */
    static size_t seq_capacity = 0;
    static size_t seq_count = 0;          /* number of terms stored */

    if (!initialized) {
        /* build prime table */
        build_primes(&primes, &prime_count);

        /* allocate residues and initialize to 1 (M_0 ≡ 1 mod p) */
        residues = (seq_t*)malloc(prime_count * sizeof(seq_t));
        for (size_t i = 0; i < prime_count; ++i) residues[i] = 1;

        /* allocate sequence array (growable) */
        seq_capacity = 16;
        seq = (seq_t*)malloc(seq_capacity * sizeof(seq_t));

        initialized = true;
    }

    if (n == 0) return 0;   /* undefined, but guard anyway */

    /* ----------------------------------------------------------------------
       Generate terms until we have at least n terms.
       ---------------------------------------------------------------------- */
    while (seq_count < n) {
        /* increase m */
        processed_m++;

        /* Update all residues for primes ≤ m.  The recurrence is
         *   M_m = m * M_{m-1} + 1  (mod p)  */
        for (size_t i = 0; i < prime_count && primes[i] <= processed_m; ++i) {
            int p = primes[i];
            seq_t mult = (seq_t)(processed_m % p);
            residues[i] = ((mult * residues[i]) + 1) % p;
        }

        /* Only m ≡ 1 (mod 4) can produce a valid n (since m = 4n+1).  */
        if (processed_m >= 5 && (processed_m & 3) == 1) {
            bool ok = true;
            for (size_t i = 0; i < prime_count && primes[i] <= processed_m; ++i) {
                int p = primes[i];
                if (p == 2) continue;          /* skip the prime 2 */
                if (residues[i] == 0) {        /* p | N_m → gcd > 2 */
                    ok = false;
                    break;
                }
            }
            if (ok) {
                /* append the corresponding term (n = (m-1)/4) to seq */
                if (seq_count == seq_capacity) {
                    seq_capacity <<= 1;
                    seq = (seq_t*)realloc(seq, seq_capacity * sizeof(seq_t));
                }
                seq[seq_count++] = (seq_t)((processed_m - 1) / 4);
            }
        }
    }

    /* return the (n‑1)-th stored term (0‑based index) */
    return seq[n - 1];
}
