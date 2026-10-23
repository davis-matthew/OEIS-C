/*
 * A135368  a(n) = (nextprime(12*n) - previousprime(12*n))/2.
 * https://oeis.org/A135368
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (next_prime(12*n) - previous_prime(12*n))/2
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Prime testing based on simple trial division.  Works for n up to 12*10000=120000. */
#include <stdint.h>
#include <stdbool.h>

/* Test whether x is a prime number (x >= 2). */
static bool is_prime_idx(idx_t x)
{
    if (x < 2) return false;
    if (x == 2) return true;
    if ((x & 1) == 0) return false;
    for (idx_t i = 3; i * i <= x; i += 2)
        if (x % i == 0) return false;
    return true;
}

/* Find the prime immediately less than start (start > 2). */
static idx_t previous_prime(idx_t start)
{
    idx_t p = start - 1;
    while (p >= 2 && !is_prime_idx(p))
        --p;
    return p;
}

/* Find the prime immediately greater than start. */
static idx_t next_prime(idx_t start)
{
    idx_t p = start + 1;
    while (!is_prime_idx(p))
        ++p;
    return p;
}

seq_t a(idx_t n)
{
    idx_t target = n * 12ULL;          /* 12 * n */
    idx_t prev = previous_prime(target);
    idx_t next = next_prime(target);
    seq_t diff = (seq_t)next - (seq_t)prev;
    return diff / 2;
}
