/*
 * A082934  A082928(1) + A082928(2) + ... + A082928(n).
 * https://oeis.org/A082934
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Sum_{k=1..n} ( is_prime(k)?k+1: (k%2==0?k/2:k) )
 * Range:     u8 n <= 24, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   is_prime, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"
#include "../utils/seq_realloc.h"

seq_t a(idx_t n)
{
    static seq_t *sum = NULL;     /* cumulative sums, sum[0] = 0 */
    static size_t sum_cap = 0;    /* number of elements allocated (>0) */
    static idx_t cur_max = 0;     /* highest n for which sum is computed */

    if (n > cur_max) {
        size_t need = (size_t)(n + 1);  /* need space for indices 0..n */

        if (sum_cap < need) {
            seq_t *tmp = seq_realloc(sum, sum_cap, need);
            if (!tmp) return 0;          /* out‑of‑memory fallback */
            sum = tmp;
            sum_cap = need;
        }

        /* initialise the first element if this is the first allocation */
        if (cur_max == 0) {
            sum[0] = (seq_t)0;
        }

        for (idx_t i = cur_max + 1; i <= n; ++i) {
            seq_t val;
            if (is_prime(i)) {
                val = (seq_t)i + (seq_t)1;
            } else {
                if ((i & 1) == 0) {
                    val = ((seq_t)i) / (seq_t)2;
                } else {
                    val = (seq_t)i;
                }
            }
            sum[(size_t)i] = sum[(size_t)(i - 1)] + val;
        }
        cur_max = n;
    }
    return sum[(size_t)n];
}
