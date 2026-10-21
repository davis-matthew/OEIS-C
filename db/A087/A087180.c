/*
 * A087180  Number partition numbers <= P(n) of the form 3*k (P = A000041).
 * https://oeis.org/A087180
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 765, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A087180:  a(n) = #{ m ≤ n : partition number p(m) ≡ 0 (mod 3) } */
/* The algorithm computes the partition numbers modulo 3 by Euler’s
   recurrence and counts how many of them are divisible by 3.  It is
   O(n √n) and works for any n ≥ 0.  No external tables are needed. */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static arrays holding p(m) mod 3 and the cumulative counts.  They
       are lazily expanded as the function is called with larger n. */
    static unsigned char *p_mod3 = NULL;   /* p(m) % 3   */
    static idx_t       *cum    = NULL;     /* cumulative zeros count */
    static idx_t        size   = 0;         /* number of computed elements */

    /* Expand storage if needed.  (Realloc requires an explicit cast in C++.) */
    if (n >= size) {
        size_t new_cap = (size_t)(n + 1);
        unsigned char *tmp_p = (unsigned char *)realloc(p_mod3, new_cap * sizeof *p_mod3);
        idx_t *tmp_c = (idx_t *)realloc(cum, new_cap * sizeof *cum);
        if (!tmp_p || !tmp_c) exit(1);   /* allocation failed */
        p_mod3 = tmp_p;
        cum    = tmp_c;

        /* Compute new values from the old size up to n (inclusive). */
        for (idx_t i = size; i <= n; ++i) {
            int res;          /* p(i) mod 3 */
            if (i == 0) {
                res = 1 % 3;   /* p(0) = 1 */
            } else {
                int sign = 1;
                res = 0;
                for (int k = 1;; ++k) {
                    int pent1 = k * (3 * k - 1) / 2;
                    if (pent1 > i) break;
                    int val = p_mod3[(size_t)(i - pent1)];
                    res = (res + (sign == 1 ? val : 3 - val)) % 3;

                    int pent2 = k * (3 * k + 1) / 2;
                    if (pent2 <= i) {
                        int val2 = p_mod3[(size_t)(i - pent2)];
                        res = (res + (sign == 1 ? val2 : 3 - val2)) % 3;
                    }
                    sign = -sign;
                }
            }
            p_mod3[(size_t)i] = (unsigned char)res;

            /* Update cumulative count. */
            if (i == 0)
                cum[(size_t)i] = (res == 0) ? 1 : 0;
            else
                cum[(size_t)i] = cum[(size_t)(i - 1)] + ((res == 0) ? 1 : 0);
        }
        size = n + 1;
    }
    return (seq_t)cum[(size_t)n];
}
