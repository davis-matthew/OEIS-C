/*
 * A100107  Inverse Moebius transform of Lucas numbers (A000032) 1,3,4,7,11,..
 * https://oeis.org/A100107
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} Lucas(d)
 * Range:     u8 n <= 11, u16 n <= 23, u32 n <= 46, u64 n <= 92, u128 n <= 184,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (Lucas(0)=2, Lucas(1)=1, Lucas(k)=Lucas(k-1)+Lucas(k-2) for k>=2) */

#include <stdlib.h>

/* The function prototype is required. */
seq_t a(idx_t n)
{
    /* Static array to hold Lucas numbers, grows as needed. */
    static seq_t *lucas = NULL;
    static idx_t lucas_len = 0;   /* number of elements allocated   */

    /* Ensure the Lucas array covers all indices up to n. */
    if (lucas_len <= n) {
        /* Allocate or grow the array. */
        idx_t new_len = n + 1;
        seq_t *tmp = (seq_t *)realloc(lucas, new_len * sizeof(seq_t));
        if (!tmp) { /* allocation failed: return 0 as a safe fallback */
            return 0;
        }
        lucas = tmp;

        /* Initialize new elements if needed. */
        if (lucas_len == 0) {
            /* Fill first two entries. */
            lucas[0] = (seq_t)2;
            lucas[1] = (seq_t)1;
            lucas_len = 2;
        }

        /* Compute new Lucas numbers from lucas_len to new_len-1. */
        for (idx_t i = lucas_len; i < new_len; ++i) {
            lucas[i] = lucas[i - 1] + lucas[i - 2];
        }
        lucas_len = new_len;
    }

    /* Compute the sum over all divisors of n. */
    seq_t sum = 0;
    for (idx_t i = 1; i <= n / i; ++i) {
        if (n % i == 0) {
            sum += lucas[i];
            idx_t j = n / i;
            if (j != i) {
                sum += lucas[j];
            }
        }
    }
    return sum;
}
