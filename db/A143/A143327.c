/*
 * A143327  Table T(n,k) by antidiagonals. T(n,k) is the number of primitive
 *          (=aperiodic) k-ary words (n,k >= 1) with length less than or equal
 *          to n which are earlier in lexicographic order than any other word
 *          derived by cyclic shifts of the alphabet.
 * https://oeis.org/A143327
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(n_row, n_col) where T is computed by T(n,k) =
 *            Σ_{d=1..n} k^(d‑1) * M(n/d), with M(m) = Σ_{i=1..m} μ(i) and μ
 *            the Möbius function
 * Range:     u8 n <= 32, u16 n <= 73, u32 n <= 182, u64 n <= 453,
 *            u128 n <= 1160, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The index n is the antidiagonal flattening of the 2‑D table
   T(n,k).  For the first number on the second diagonal (1,2) the
   index is 2, the third diagonal begins with index 4, etc. */

/* Standard headers          */
#include <stdlib.h>
#include <stdint.h>

/* Computes the n‑th term of OEIS A143327. */
seq_t a(idx_t n)
{
    /* --------------------------------------------------------------------
       1. Map the linear index n to a pair (row, col) = (n_row, n_col).
       -------------------------------------------------------------------- */
    if (n == 0) return 0;                 /* undefined but harmless */

    /* Find the smallest s such that s*(s‑1)/2 >= n   (s≥2).         */
    idx_t s = 2;
    while ((s - 1) * s / 2 < n) ++s;

    idx_t prev_cum = (s - 2) * (s - 1) / 2;      /* terms before this diagonal */
    idx_t offset   = n - prev_cum - 1;            /* 0‑based offset in this diagonal */

    idx_t n_row = offset + 1;       /* 1‑based row index      */
    idx_t n_col = s - n_row;        /* 1‑based column index   */

    /* --------------------------------------------------------------------
       2. Ensure we have the Möbius function μ up to n_row.
          (We need its prefix sum M(m) for m up to floor(n_row/d), which
           never exceeds n_row itself.)
       -------------------------------------------------------------------- */
    static size_t maxN = 0;
    static int32_t *mu   = NULL;
    static int32_t *pref = NULL;

    if (n_row > maxN) {
        size_t newN = (size_t)n_row;
        int32_t *new_mu   = (int32_t*)realloc(mu,   (newN + 1) * sizeof(int32_t));
        int32_t *new_pref = (int32_t*)realloc(pref, (newN + 1) * sizeof(int32_t));
        if (!new_mu || !new_pref) { abort(); }
        mu   = new_mu;
        pref = new_pref;

        /* Linear sieve for μ up to newN */
        size_t *primes  = (size_t*)malloc((newN + 1) * sizeof(size_t));
        unsigned char *is_comp = (unsigned char*)calloc(newN + 1, 1);
        size_t pc = 0;

        mu[0] = 0;                     /* unused */
        mu[1] = 1;
        if (newN >= 2) {
            for (size_t i = 2; i <= newN; ++i) {
                if (!is_comp[i]) {
                    primes[pc++] = i;
                    mu[i] = -1;
                }
                for (size_t j = 0; j < pc; ++j) {
                    size_t v = i * primes[j];
                    if (v > newN) break;
                    is_comp[v] = 1;
                    if (i % primes[j] == 0) {
                        mu[v] = 0;
                        break;
                    } else {
                        mu[v] = -mu[i];
                    }
                }
            }
        }

        /* Prefix sums of μ */
        pref[0] = 0;
        for (size_t i = 1; i <= newN; ++i) {
            pref[i] = pref[i - 1] + mu[i];
        }

        free(primes);
        free(is_comp);
        maxN = newN;
    }

    /* --------------------------------------------------------------------
       3. Compute T(n_row, n_col) = Σ_{d=1..n_row} n_col^(d‑1) * M(n_row/d)
       -------------------------------------------------------------------- */
    seq_t result = 0;
    seq_t power  = 1;               /* n_col^(d-1) for current d = 1 */

    for (idx_t d = 1; d <= n_row; ++d) {
        idx_t m = n_row / d;                    /* floor(n_row/d) */
        int32_t mu_sum = pref[m];                /* M(m) */
        if (mu_sum != 0) {
            result += power * (seq_t)mu_sum;
        }
        power *= n_col;                         /* next power n_col^d */
    }
    return result;
}
