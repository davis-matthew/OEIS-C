/*
 * A103359  T(n,k) = Max{p: prime p divides m where n-k<=m<=n+k}, triangle
 *          read by rows: 0<=k<n.
 * https://oeis.org/A103359
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = max{p: prime p | ∃m∈[r-k,r+k] with p|m}, where r,k are
 *            obtained from the triangle index n
 * Range:     u8 n <= 80, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

/* Return the value of sequence A103359 for index n (n >= 1). */
seq_t a(idx_t n)
{
    /* ----- Find row r and position k in the triangle ----- */
    idx_t r = 1;
    idx_t sum = 1; /* sum of rows up to current r */
    while (n > sum) {
        ++r;
        sum += r;          /* now sum = r(r+1)/2 */
    }
    idx_t sum_prev = sum - r;               /* sum of rows before r   */
    idx_t k = n - sum_prev - 1;             /* position in row r (0‑based) */

    /* ----- Determine the integer interval [l,u] = [r-k, r+k] ----- */
    idx_t l = r - k;
    idx_t u = r + k;

    /* ----- Static data for greatest‑prime‑factor table ----- */
    static idx_t precomp_max = 0;          /* highest index precomputed */
    static uint64_t *gpf = NULL;           /* table: greatest prime factor */

    if (u > precomp_max) {
        /* free old table if it exists */
        if (gpf) free(gpf);

        /* enlarge table to cover [0,u] */
        precomp_max = u;
        gpf = (uint64_t *)malloc((precomp_max + 1) * sizeof(uint64_t));
        if (!gpf) return (seq_t)1;        /* allocation failure → return 1 */
        for (idx_t i = 0; i <= precomp_max; ++i) gpf[i] = 0;

        /* simple sieve to fill gpf with largest prime factors        */
        bool *is_prime = (bool *)malloc((precomp_max + 1) * sizeof(bool));
        if (!is_prime) { free(gpf); return (seq_t)1; }

        for (idx_t i = 0; i <= precomp_max; ++i) is_prime[i] = true;
        if (precomp_max >= 0) is_prime[0] = false;
        if (precomp_max >= 1) is_prime[1] = false;

        for (idx_t i = 2; i <= precomp_max; ++i) {
            if (is_prime[i]) {
                /* record i as a prime divisor for all multiples of i   */
                for (idx_t j = i; j <= precomp_max; j += i)
                    gpf[j] = (uint64_t)i;   /* later larger primes will
                                               overwrite earlier ones   */
                /* mark composites (standard sieve step)                */
                if (i * i <= precomp_max) {
                    for (idx_t j = i * i; j <= precomp_max; j += i)
                        is_prime[j] = false;
                }
            }
        }
        free(is_prime);
    }

    /* ----- Scan the interval [l,u] and find the maximum prime factor ----- */
    uint64_t max_p = 0;
    for (idx_t m = l; m <= u; ++m) {
        uint64_t p = gpf[m];
        if (p > max_p) max_p = p;
    }

    /* For the special case n = 1 (interval [1,1]) no prime factor exists */
    return (max_p == 0) ? (seq_t)1 : (seq_t)max_p;
}
