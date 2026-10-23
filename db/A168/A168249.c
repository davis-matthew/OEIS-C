/*
 * A168249  n-th single or isolated number minus n-th squarefree number.
 * https://oeis.org/A168249
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A167706(n) - A005117(n) (single or isolated minus
 *            square‑free). The function computes the n‑th term for n ≥ 1
 * Range:     u8 n <= 48, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* For the given problem n starts at 1.  Return 0 for n == 0. */
    if (n == 0) return 0;

    /* Initial upper bound for the sieve.  The factor 20 has been chosen
       empirically to be ample for n up to 10 000 (200 000).  The loop
       below doubles the limit if it turns out to be too small. */
    size_t limit = (size_t)(20 * n + 200);

    for (;;) {
        /* Allocate space for primality test and smallest prime factor. */
        bool *is_prime = (bool *)calloc(limit + 1, sizeof(bool));
        uint32_t *spf = (uint32_t *)calloc(limit + 1, sizeof(uint32_t));
        if (!is_prime || !spf) {
            free(is_prime);
            free(spf);
            return 0;  /* Allocation error */
        }

        /* Plant the initial primality flags. */
        for (size_t i = 2; i <= limit; ++i) is_prime[i] = true;

        /* Sieve of Eratosthenes, filling spf as we go. */
        for (size_t i = 2; i <= limit; ++i) {
            if (!is_prime[i]) continue;      /* composite */
            spf[i] = (uint32_t)i;             /* smallest prime factor of a prime is itself */
            if (i * i <= limit) {
                for (size_t j = i * i; j <= limit; j += i) {
                    if (is_prime[j]) is_prime[j] = false;
                    if (!spf[j]) spf[j] = (uint32_t)i;
                }
            }
        }

        /* Counters and stored n‑th terms. */
        idx_t  sq_cnt  = 0;
        idx_t  sn_cnt  = 0;
        seq_t  sq_n    = 0;
        seq_t  sn_n    = 0;

        /* The sequence A005117(1) is 1 */
        if (sq_cnt < n) {
            sq_n   = (seq_t)1;
            sq_cnt = 1;
        }

        /* Scan integers, stopping when we have collected n terms of each. */
        for (size_t i = 2; i <= limit && (sq_cnt < n || sn_cnt < n); ++i) {
            /* --- Check square‑free status of i --- */
            bool sq = true;
            uint32_t x = (uint32_t)i;
            uint32_t prev_p = 0;
            while (x > 1) {
                uint32_t p = spf[x];
                if (p == prev_p) {        /* exponent ≥ 2 */
                    sq = false;
                    break;
                }
                prev_p = p;
                x /= p;
            }
            if (sq && sq_cnt < n) {
                sq_n   = (seq_t)i;
                sq_cnt = (idx_t)sq_cnt + 1;
            }

            /* --- Check single/isolated status of i --- */
            if (sn_cnt < n) {
                bool single = false;
                if (is_prime[i]) {   /* i is prime */
                    bool left  = (i > 2) && is_prime[i - 2];
                    bool right = (i + 2 <= limit) && is_prime[i + 2];
                    single = !left && !right;
                } else {            /* i is composite */
                    bool left  = (i > 1) && is_prime[i - 1];
                    bool right = (i + 1 <= limit) && is_prime[i + 1];
                    single = left && right;
                }
                if (single) {
                    sn_n   = (seq_t)i;
                    sn_cnt = (idx_t)sn_cnt + 1;
                }
            }
        }

        /* Clean up the temporary arrays. */
        free(is_prime);
        free(spf);

        /* Did we gather enough terms? */
        if (sq_cnt >= n && sn_cnt >= n) {
            /* Result is the difference of the two n‑th terms. */
            return sn_n - sq_n;
        }

        /* Not enough terms – increase the sieve size and repeat. */
        limit *= 2;
    }
}
