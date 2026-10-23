/*
 * A127513  Partial sums of A127511.
 * https://oeis.org/A127513
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = a(n-1) + b(n), b(n) = 0 if n is not squarefree; b(n) =
 *            (-1)^ω(n) * 2^(n-1) for squarefree n. Here ω(n) is the number of
 *            distinct prime factors. a(0)=0
 * Range:     i8 n <= 9, i16 n <= 16, i32 n <= 32, i64 n <= 64, i128 n <= 128,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n) {
    /* state kept across calls */
    static idx_t max_n = 0;                /* largest n already computed */
    static seq_t *a_arr = NULL;            /* cumulative sums, 1‑indexed */
    static uint64_t *spf = NULL;           /* smallest prime factor table */
    static idx_t spf_limit = 0;            /* size of spf table */
    static seq_t pow_prev = 1;             /* 2^(i-1) for the current i */

    if (n > max_n) {
        /* enlarge the smallest‑prime‑factor table if needed */
        if (n > spf_limit) {
            uint64_t *new_spf = (uint64_t *)calloc(n + 1, sizeof(uint64_t));
            if (!new_spf) exit(1);
            for (idx_t i = 2; i <= n; ++i) new_spf[i] = i;
            for (idx_t i = 2; i * i <= n; ++i) {
                if (new_spf[i] == i) {
                    for (idx_t j = i * i; j <= n; j += i) {
                        if (new_spf[j] == j) new_spf[j] = i;
                    }
                }
            }
            free(spf);
            spf = new_spf;
            spf_limit = n;
        }

        /* enlarge the cumulative array */
        if (a_arr == NULL) {
            a_arr = (seq_t *)calloc(n + 1, sizeof(seq_t)); /* a_arr[0] = 0 */
            if (!a_arr) exit(1);
        } else {
            seq_t *tmp = (seq_t *)realloc(a_arr, (n + 1) * sizeof(seq_t));
            if (!tmp) exit(1);
            a_arr = tmp;
        }

        /* compute consecutive terms up to n */
        for (idx_t i = max_n + 1; i <= n; ++i) {
            /* factor i to test square‑free and count distinct primes */
            idx_t temp = i;
            bool squarefree = true;
            int omega = 0;
            while (temp > 1) {
                uint64_t p = spf[temp];
                int cnt = 0;
                while (temp % p == 0) {
                    temp /= p;
                    ++cnt;
                    if (cnt > 1) {  /* a squared prime factor found */
                        squarefree = false;
                        break;
                    }
                }
                if (!squarefree) break;
                ++omega;
            }

            seq_t term = 0;
            if (squarefree) {
                /* magnitude: 2^(i-1); sign depends on omega parity */
                if (omega & 1) term = -pow_prev;
                else term = pow_prev;
            }
            a_arr[i] = a_arr[i - 1] + term;

            /* prepare power for the next index */
            pow_prev = pow_prev * (seq_t)2;
        }

        max_n = n;
    }

    return a_arr[n];
}
