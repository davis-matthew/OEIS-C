/*
 * A163638  Two copies of the sequence A141468, one at prime the other at
 *          nonprime indices.
 * https://oeis.org/A163638
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, less
 * Formula:   a(n) is the nth element of A163638 – a sequence obtained by
 *            copying the list of non‑prime numbers twice, once at each prime
 *            index and once at each non‑prime index
 * Range:     u8 n <= 75, u16 n <= 75, u32 n <= 75, u64 n <= 75, u128 n <= 75,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime.h"

/* external linkage */
seq_t a(idx_t n)
{
    /* Two independent counters that walk through the natural numbers.
       Each counter keeps its own position and skips prime values.  */
    static idx_t nonprime_counter = 0;  /* for non‑prime indices   */
    static idx_t prime_counter   = 0;   /* for prime   indices   */

    if (is_prime(n)) {
        /* prime index: fetch next non-prime number for the prime copy   */
        while (is_prime(prime_counter))
            ++prime_counter;
        seq_t val = (seq_t)prime_counter;
        ++prime_counter;
        return val;
    } else {
        /* non‑prime index: fetch next non‑prime number for the non‑prime copy */
        while (is_prime(nonprime_counter))
            ++nonprime_counter;
        seq_t val = (seq_t)nonprime_counter;
        ++nonprime_counter;
        return val;
    }
}
