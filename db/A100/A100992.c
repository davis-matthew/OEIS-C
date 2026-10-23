/*
 * A100992  Indices k of Fibonacci numbers F(k) (A000045) that are divisible
 *          by k+1.
 * https://oeis.org/A100992
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th integer k such that F(k) mod (k+1) == 0, where
 *            F(k) is the k-th Fibonacci number (F(0)=0, F(1)=1)
 * Range:     u8 n <= 25, u16 n <= 3266, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   fib_mod_uint64, fib_pair, mul_mod_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/fib_mod_uint64.h"
#include "../utils/fib_pair.h"
#include "../utils/mul_mod_u64.h"

/* Return the n-th element of OEIS A100992. */
seq_t a(idx_t n)
{
    static seq_t *table = NULL; /* stores found k values */
    static idx_t table_len = 0;
    static idx_t next_k = 1;    /* next k to test */

    while (table_len < n) {
        uint64_t mod = (uint64_t)(next_k + 1);
        if (fib_mod_uint64(next_k, mod) == 0) {
            seq_t *tmp = (seq_t *)realloc(table, (table_len + 1) * sizeof(seq_t));
            if (!tmp) { /* allocation failure; abort */
                exit(EXIT_FAILURE);
            }
            table = tmp;
            table[table_len] = (seq_t)next_k;
            ++table_len;
        }
        ++next_k;
    }
    return table[n - 1];
}
