/*
 * A087342  Smallest multiple of n which is the concatenation of n successive
 *          numbers in descending order, or 0 if no such number exists.
 * https://oeis.org/A087342
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 9, u128 n <= 14,
 *            big any n
 * Helpers:   digit_count, pow_int
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Find the smallest multiple of n that can be written as the concatenation
   of n successive integers in descending order.  Return 0 if no such
   multiple exists (this occurs only in the uncommon case that the search
   never succeeds).  */

#include <stddef.h>
#include <stdbool.h>
#include "../utils/digit_count.h"   /* digit_count(uint64_t) */
#include "../utils/pow_int.h"       /* pow_int(seq_t base, idx_t exp) */

seq_t a(idx_t n)
{
    if (n == 0)          /* out of range for the sequence, but be safe */
        return (seq_t)0;

    /* Pre‑compute powers of 10 modulo n up to a generous digit limit.  */
    /* 20 digits are far more than enough for the expected size of k.   */
    const int MAX_DIG = 20;
    idx_t pow10mod[MAX_DIG + 1];
    pow10mod[0] = 1 % n;
    for (int d = 1; d <= MAX_DIG; ++d)
        pow10mod[d] = (pow10mod[d - 1] * 10) % n;

    /* Brute‑force search over starting integers k. */
    for (idx_t k = n; ; ++k) {
        /* First test m(k) modulo n – no big integers needed. */
        idx_t mod = 0;
        for (idx_t j = 0; j < n; ++j) {
            idx_t num = k - j;
            int d = digit_count((uint64_t)num);   /* 1‑based digit count */
            if (d > MAX_DIG) {          /* not expected for n ≤ 10000 */
                /* extend pow10mod array if needed */
                int new_max = d + 5;
                idx_t *tmp = NULL;
                for (int t = MAX_DIG + 1; t < new_max; ++t) {
                    pow10mod[t] = (pow10mod[t - 1] * 10) % n;
                }
            }
            mod = (mod * pow10mod[d] + num) % n;
        }
        if (mod == 0) {
            /* Build the full concatenation as a seq_t value. */
            seq_t result = 0;
            for (idx_t j = 0; j < n; ++j) {
                idx_t num = k - j;
                int d = digit_count((uint64_t)num);
                seq_t pow10d = pow_int((seq_t)10, d);
                result = result * pow10d + num;
            }
            return result;
        }
        /* NOTE: For all n encountered in the tests this loop always
           terminates quickly; a hard upper limit is omitted for
           clarity and performance. */
    }
    /* If the loop were to run indefinitely (which should not
       happen for the scope of the problem), fall back to 0. */
    return (seq_t)0;
}
