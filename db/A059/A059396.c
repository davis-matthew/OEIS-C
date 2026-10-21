/*
 * A059396  Number of primes less than square root of n-th prime; i.e., number
 *          of trial divisions by smaller primes to show that n-th prime is
 *          indeed prime.
 * https://oeis.org/A059396
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of primes ≤ ⌊√pₙ⌋, where pₙ is the n‑th prime
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_u64, isqrt_u64, push_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime_u64.h"   /* bool is_prime_u64(uint64_t); */
#include "../utils/push_uint64.h"   /* void push_uint64(uint64_t **, size_t *, size_t *, uint64_t); */
#include "../utils/isqrt_u64.h"     /* uint64_t isqrt_u64(uint64_t); */

/* Static storage for primes generated on demand */
static uint64_t *prime_buf = NULL; /* array of primes */
static size_t   prime_cap = 0;     /* capacity of prime_buf   */
static size_t   prime_len = 0;     /* current number of primes */

/* Return the n‑th prime (1‑indexed).  Generates primes when needed. */
static uint64_t nth_prime(idx_t n)
{
    while (prime_len < n) {
        uint64_t candidate = (prime_len == 0) ? 2 : prime_buf[prime_len - 1] + 1;

        /* Skip even numbers after 2 */
        if (candidate > 2 && (candidate & 1ULL) == 0)
            ++candidate;

        for (;;) {
            if (is_prime_u64(candidate)) {
                push_uint64(&prime_buf, &prime_len, &prime_cap, candidate);
                break;
            }
            candidate += 2ULL;           /* next odd candidate */
        }
    }
    return prime_buf[n - 1];
}

/* OEIS A059396: number of primes ≤ ⌊√pₙ⌋. */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;                      /* offset is at 1 */

    uint64_t p_n   = nth_prime(n);
    uint64_t sq    = isqrt_u64(p_n);   /* floor(sqrt(p_n)) */

    /* Count how many stored primes are ≤ sq */
    size_t cnt = 0;
    while (cnt < prime_len && prime_buf[cnt] <= sq)
        ++cnt;

    return (seq_t)cnt;
}
