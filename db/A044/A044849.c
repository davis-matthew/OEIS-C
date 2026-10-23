/*
 * A044849  Positive integers having no fewer base-8 runs of even length than
 *          odd.
 * https://oeis.org/A044849
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 49, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>

/* A044849: Positive integers having no fewer base‑8 runs of even length than odd. */
seq_t a(idx_t n)
{
    static seq_t *cache   = NULL;
    static size_t cap     = 0;
    static size_t cached  = 0;
    static seq_t  last_val = 0;

    if (n == 0) return 0;  /* undefined for n < 1, but guard against misuse */

    /* Allocate or grow cache if needed */
    if (n > cached) {
        if (n > cap) {
            size_t new_cap = cap ? cap : 8;
            while (new_cap < n) new_cap <<= 1;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) return 0;               /* allocation failure */
            cache = tmp;
            cap   = new_cap;
        }
        seq_t cur = last_val;
        while (cached < n) {
            cur += 1;          /* next integer to test */

            /* Count runs in base 8 */
            seq_t temp = cur;
            int prev_digit = -1, run_len = 0;
            int even = 0, odd = 0;
            while (temp > 0) {
                int d = (int)(temp % 8);
                temp /= 8;
                if (d == prev_digit) {
                    run_len++;
                } else {
                    if (prev_digit != -1) {
                        if ((run_len & 1) == 0) even++;
                        else                  odd++;
                    }
                    run_len    = 1;
                    prev_digit = d;
                }
            }
            if (prev_digit != -1) {                 /* last run */
                if ((run_len & 1) == 0) even++;
                else                  odd++;
            }

            if (even >= odd) {
                cache[cached++] = cur;
            }
        }
        last_val = cur;           /* remember where we stopped */
    }
    return cache[n-1];
}
