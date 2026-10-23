/*
 * A023201  Primes p such that p + 6 is also prime. (Lesser of a pair of sexy
 *          primes.)
 * https://oeis.org/A023201
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, changed
 * Formula:   a(n) = the n‑th prime p such that p+6 is also prime (A023201)
 * Range:     u8 n <= 30, u16 n <= 1745, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* Trial‑division primality test for 64‑bit inputs. */
static bool is_prime_uint64(uint64_t n)
{
    if (n < 2) return false;
    if ((n & 1ULL) == 0) return n == 2ULL;
    for (uint64_t d = 3; d * d <= n; d += 2)
        if (n % d == 0) return false;
    return true;
}

seq_t a(idx_t n)
{
    static seq_t *arr = NULL;
    static size_t used = 0;
    static size_t cap = 0;
    static uint64_t candidate = 3; /* prime before the first eligible prime (5) */

    while (used < (size_t)n) {
        /* Advance to the next prime. */
        candidate++;
        while (!is_prime_uint64(candidate))
            ++candidate;

        /* If candidate + 6 is also prime, store it. */
        if (is_prime_uint64(candidate + 6)) {
            if (used == cap) {
                size_t newcap = cap ? cap * 2 : 4;
                seq_t *tmp = (seq_t *)realloc(arr, newcap * sizeof(seq_t));
                if (!tmp) abort();
                arr = tmp;
                cap = newcap;
            }
            arr[used++] = (seq_t)candidate;
        }
    }
    return arr[(size_t)n - 1];
}
