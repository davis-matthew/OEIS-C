/*
 * A107304  Numbers k such that 5k - 7 is prime.
 * https://oeis.org/A107304
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 53, u16 n <= 7070, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime, push_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   a(n) = smallest k such that 5*k - 7 is prime,
   returning the nth such k (OEIS seq A107304). 
   The function dynamically extends an internal array of found k.
*/
#include "../utils/is_prime.h"
#include "../utils/push_seq.h"

seq_t a(idx_t n)
{
    static seq_t *seq = NULL;   /* array of k’s */
    static idx_t seq_cap = 0;   /* capacity of seq */
    static idx_t seq_size = 0;  /* number of entries in seq */
    static uint64_t next_k = 2; /* next k to test (first valid k) */

    if (n == 0) return 0; /* undefined for offset 1, but harmless */

    /* Grow the array until we have at least n entries */
    while (seq_size < n)
    {
        uint64_t val = 5 * next_k - 7;
        if (is_prime(val))
        {
            push_seq(&seq, &seq_cap, &seq_size, (seq_t)next_k);
        }
        ++next_k;
    }

    return seq[n - 1];
}
