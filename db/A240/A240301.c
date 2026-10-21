/*
 * A240301  a(n) is the number of sets of three positive integers p_1 < p_2 <
 *          p_3 such that 2*p_2 = p_1 + p_3, where p_i (i=1,2,3) is either 1
 *          or a prime number and p_3 = prime(n).
 * https://oeis.org/A240301
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3668, u16 n <= 9547, u32 n <= 9547, u64 n <= 9547,
 *            u128 n <= 9547, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A240301:
   a(n) = number of triples (p1,p2,p3) with p1<p2<p3,
   2*p2 = p1 + p3, each p_i is 1 or a prime, and p3 = prime(n)
   (offset 2: a(2) is the first term). */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* ---------- prime storage and generation -------------------------------- */
static uint64_t *prime_list = NULL;   /* sorted primes */
static size_t   prime_count = 0;      /* how many primes we have */
static size_t   prime_cap   = 0;      /* allocated capacity */

/* Grow the array of primes to hold at least new_cap elements. */
static void
prime_grow(size_t new_cap)
{
    uint64_t *tmp = (uint64_t *)realloc(prime_list, new_cap * sizeof(uint64_t));
    if (!tmp) exit(1);          /* abort on OOM */
    prime_list = tmp;
    prime_cap  = new_cap;
}

/* Ensure that we have at least n primes in prime_list (1‑based). */
static void
ensure_prime_count(size_t n)
{
    if (prime_count >= n) return;

    if (prime_cap == 0) {
        prime_cap = 128;
        prime_list = (uint64_t *)malloc(prime_cap * sizeof(uint64_t));
        if (!prime_list) exit(1);
    }

    uint64_t candidate = (prime_count == 0) ? 2 : prime_list[prime_count - 1] + 1;

    while (prime_count < n) {
        bool is_prime = true;

        /* trial division using already found primes */
        for (size_t i = 0; i < prime_count &&
             (unsigned __int128)prime_list[i] * prime_list[i] <= candidate; ++i) {
            if (candidate % prime_list[i] == 0) {
                is_prime = false;
                break;
            }
        }

        if (is_prime) {
            if (prime_count == prime_cap) prime_grow(prime_cap * 2);
            prime_list[prime_count++] = candidate;
        }
        ++candidate;
    }
}

/* Search for x in prime_list[0 .. upto-1] */
static bool
is_prime_in_array(uint64_t x, size_t upto)
{
    size_t lo = 0, hi = upto;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        uint64_t v = prime_list[mid];
        if (v == x) return true;
        if (v < x) lo = mid + 1;
        else hi = mid;
    }
    return false;
}

/* ---------- sequence function ------------------------------------------- */
seq_t a(idx_t n)
{
    if (n < 2) return 0;              /* offset 2 */

    /* prime(n) is the (n)‑th prime, 1‑based */
    ensure_prime_count((size_t)n);
    uint64_t p3 = prime_list[n - 1];   /* nth prime */

    seq_t cnt = 0;
    /* iterate over all primes p2 < p3 */
    for (size_t i = 0; i < prime_count; ++i) {
        uint64_t p2 = prime_list[i];
        if (p2 >= p3) break;

        uint64_t p1 = 2 * p2 - p3;
        if (p1 < 1) continue;          /* p1 must be positive */
        if (p1 >= p2) continue;        /* need p1 < p2 */

        if (p1 == 1 || is_prime_in_array(p1, prime_count))
            ++cnt;
    }
    return cnt;
}
