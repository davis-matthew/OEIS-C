/*
 * A051905  Duplicate of A051426.
 * https://oeis.org/A051905
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  dead
 * Formula:   a(n) = 2 * lcm(1,2,…,n)
 * Range:     i8 n <= 6, i16 n <= 10, i32 n <= 15, i64 n <= 15, i128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* lcm(1,…,n) = ∏_{p ≤ n} p^{⌊log_p n⌋} */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Product of prime powers ≤ n */
    seq_t prod = 1;
    if (n < 2) {
        /* For n = 1, prod stays 1 */
    } else {
        /* Allocate sieve array of size n+1 */
        size_t sz = (size_t)n + 1;
        bool *is_prime = (bool*)malloc(sz * sizeof(bool));
        if (!is_prime) return prod;           /* Out of memory – unlikely in tests */
        for (size_t i = 2; i < sz; ++i) is_prime[i] = true;
        for (size_t i = 2; i * i < sz; ++i) {
            if (is_prime[i]) {
                for (size_t j = i * i; j < sz; j += i) is_prime[j] = false;
            }
        }
        for (size_t p = 2; p < sz; ++p) {
            if (!is_prime[p]) continue;
            /* largest power of prime p not exceeding n */
            idx_t power = (idx_t)p;
            while (power <= n / (idx_t)p) power *= (idx_t)p;
            prod *= (seq_t)power;
        }
        free(is_prime);
    }
    /* Multiply by 2 as per OEIS definition */
    prod *= (seq_t)2;
    return prod;
}
