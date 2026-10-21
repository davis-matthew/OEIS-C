/*
 * A051547  a(n) = lcm{ phi(1), ..., phi(n) }, where phi is Euler's totient
 *          function A000010.
 * https://oeis.org/A051547
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = lcm{phi(1), phi(2), …, phi(n)} (offset 1)
 * Range:     u8 n <= 18, u16 n <= 46, u32 n <= 82, u64 n <= 172,
 *            u128 n <= 316, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    static idx_t max_n   = 0;
    static idx_t *phi    = NULL;     /* phi[i] holds Euler totient of i */
    static seq_t *lcm_arr = NULL;    /* lcm_arr[i] = a(i) */

    if (n < 1)                       /* undefined for n = 0; safeguard */
        return 0;

    /* Extend precomputed tables up to n if necessary */
    if (n > max_n) {
        /* 1) Compute phi[1..n] (sieve of Euler). */
        idx_t *new_phi = (idx_t*)malloc((n + 1) * sizeof(idx_t));
        if (!new_phi) return 0;      /* allocation failure, unlikely */
        new_phi[0] = 0;
        for (idx_t i = 1; i <= n; ++i)
            new_phi[i] = i;
        for (idx_t p = 2; p <= n; ++p) {
            if (new_phi[p] == p) {   /* p is prime */
                for (idx_t j = p; j <= n; j += p)
                    new_phi[j] -= new_phi[j] / p;
            }
        }
        free(phi);
        phi = new_phi;

        /* 2) Resize lcm_arr to hold values up to n. */
        seq_t *new_lcm = (seq_t*)realloc(lcm_arr, (n + 1) * sizeof(seq_t));
        if (!new_lcm) return 0;      /* allocation failure */
        lcm_arr = new_lcm;

        /* 3) Compute a(k) for k = max_n+1 .. n. */
        seq_t curr_lcm = (max_n == 0) ? (seq_t)1 : lcm_arr[max_n];
        for (idx_t k = max_n + 1; k <= n; ++k) {
            seq_t phi_k = (seq_t)phi[k];
            seq_t g     = gcd(curr_lcm, phi_k);
            curr_lcm = (curr_lcm / g) * phi_k;
            lcm_arr[k] = curr_lcm;
        }
        max_n = n;
    }

    return lcm_arr[n];
}
