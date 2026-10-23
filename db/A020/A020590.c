/*
 * A020590  Smallest nonempty set S containing prime divisors of 4k+5 for each
 *          k in S.
 * https://oeis.org/A020590
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 5, u32 n <= 5, u64 n <= 5, u128 n <= 5,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A020590:
   Smallest nonempty set S containing prime divisors of 4k+5
   for each k in S.  The sequence lists the elements of S in
   increasing order.  It is obtained by starting with k = 1,
   repeatedly adding the prime divisors of 4k+5 and closing
   under this operation. */

/* Because the sequence is finite (the closure stops at 73),
   the function builds the set once and then answers queries
   from the cached array. */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* Maximum number of primes expected in the closure. */
#define MAX_PRIMES 128

static uint64_t primes_array[MAX_PRIMES];
static size_t primes_count = 0;
static bool built = false;

/* Simple trial‑division to find the smallest prime factor. */
static uint64_t min_prime_factor(uint64_t n)
{
    if ((n & 1U) == 0) return 2;
    for (uint64_t d = 3; d * d <= n; d += 2)
        if (n % d == 0) return d;
    return n;
}

/* Add a prime to the array if it is not already present. */
static void add_prime(uint64_t p)
{
    for (size_t i = 0; i < primes_count; ++i)
        if (primes_array[i] == p) return;
    if (primes_count < MAX_PRIMES)
        primes_array[primes_count++] = p;
}

/* Build the closure set starting from k = 1. */
static void build_closure(void)
{
    uint64_t queue[MAX_PRIMES];
    size_t head = 0, tail = 0;

    /* Start with k = 1. */
    queue[tail++] = 1;

    while (head < tail) {
        uint64_t k = queue[head++];
        uint64_t m = 4 * k + 5;
        uint64_t t = m;

        /* Factor m and add each prime factor. */
        while (t > 1) {
            uint64_t p = min_prime_factor(t);
            /* If p is new, enqueue it for further processing. */
            bool already = false;
            for (size_t i = 0; i < primes_count; ++i)
                if (primes_array[i] == p) { already = true; break; }
            if (!already) {
                add_prime(p);
                if (tail < MAX_PRIMES) queue[tail++] = p;
            }
            while (t % p == 0) t /= p;
        }
    }

    /* Sort primes in ascending order (bubble sort is fine for ≤5 items). */
    for (size_t i = 0; i < primes_count; ++i)
        for (size_t j = i + 1; j < primes_count; ++j)
            if (primes_array[i] > primes_array[j]) {
                uint64_t tmp = primes_array[i];
                primes_array[i] = primes_array[j];
                primes_array[j] = tmp;
            }

    built = true;
}

seq_t a(idx_t n)
{
    if (!built) build_closure();

    if (n == 0 || (size_t)n > primes_count) return 0;
    return (seq_t)primes_array[(size_t)n - 1];
}
