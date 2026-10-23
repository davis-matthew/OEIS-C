/*
 * A022831  a(n) = c(1)p(1) + ... + c(n)p(n), where c(i) = 1 if a(i-1) <= p(i)
 *          and c(i) = -1 if a(i-1) > p(i), for i = 1,...,n (p(i) = primes).
 * https://oeis.org/A022831
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{i=1..n} c(i)·p(i), c(i)=+1 if a(i-1)≤p(i), else -1;
 *            Initial: a(1)=2 (since a(0)=0 ≤ p(1)=2)
 * Range:     u8 n <= 47, u16 n <= 5008, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/get_prime.h"

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;          /* offset is 1; behaviour for 0 is arbitrary */

    seq_t a_val = 0;                      /* a(0) */
    for (idx_t i = 1; i <= n; ++i) {
        uint32_t p_uint = get_prime(i);   /* a builtin implementation of 1‑based primes */
        seq_t p = (seq_t)p_uint;          /* promote to seq_t */
        if (a_val <= p)
            a_val += p;
        else
            a_val -= p;
    }
    return a_val;
}
