/*
 * A176656  The positions of single (or isolated or non-twin) primes in
 *          A000040.
 * https://oeis.org/A176656
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the index of the n-th isolated prime
 * Range:     u8 n <= 154, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Dynamic array of primes generated on demand */
static uint64_t *prime_list = NULL;
static size_t prime_len  = 0;
static size_t prime_cap  = 0;

/* Append a prime to the list, resizing as necessary. */
static void append_prime(uint64_t p)
{
    if (prime_len == prime_cap) {
        size_t new_cap = prime_cap ? prime_cap * 2 : 16;
        uint64_t *tmp = (uint64_t *)realloc(prime_list, new_cap * sizeof(uint64_t));
        if (!tmp) exit(EXIT_FAILURE);
        prime_list = tmp;
        prime_cap  = new_cap;
    }
    prime_list[prime_len++] = p;
}

/* Test primality of an unsigned 64‑bit integer using trial division by
   the primes already stored in prime_list.  Since primes are generated
   in increasing order, prime_list always contains all primes ≤ the
   argument. */
static bool is_prime_uint64(uint64_t n)
{
    if (n < 2) return false;
    for (size_t i = 0; i < prime_len; ++i) {
        uint64_t p = prime_list[i];
        if (p * p > n) break;
        if (n % p == 0) return false;
    }
    return true;
}

/* Public sequence function. */
seq_t a(idx_t n)
{
    if (n == (idx_t)0) return (seq_t)0;   /* outside offset, unused */

    /* Reset prime list for each call (lazy recomputation is fine
       for n <= 10 000). */
    prime_len = 0;

    uint64_t isolated_cnt = 0;  /* how many isolated primes encountered */
    uint64_t prime_pos    = 0;  /* position of the current prime in A000040 */
    uint64_t p = 2;            /* current candidate prime */

    while (true) {
        if (is_prime_uint64(p)) {
            append_prime(p);
            prime_pos++;

            bool left  = false;
            bool right = false;

            if (p > 2) left  = is_prime_uint64(p - 2);
            right = is_prime_uint64(p + 2);

            if (!left && !right) {
                isolated_cnt++;
                if (isolated_cnt == n) return (seq_t)prime_pos;
            }
        }

        /* Next odd candidate. */
        if (p == 2) p = 3;
        else        p += 2;
    }
}
