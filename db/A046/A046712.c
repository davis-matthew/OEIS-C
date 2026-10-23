/*
 * A046712  From the Bruck-Ryser theorem: n == 1 or 2 (mod 4) which are not
 *          the sum of 2 squares.
 * https://oeis.org/A046712
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   not stated
 * Range:     u8 n <= 60, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A046712 – Numbers ≡ 1 or 2 (mod 4) that are NOT representable as a sum of two squares.
   The algorithm scans successive integers, keeps only those that satisfy
   (n mod 4 == 1 or 2) and fail the sum‑of‑two‑squares test, and caches
   them for fast indexing.  The test uses the classical property:
   n is a sum of two squares iff every prime ≡ 3 (mod 4) divides n with an even exponent. */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Cache for the sequence values */
static seq_t  *a_cache  = NULL;   /* pointer to stored values          */
static size_t  a_len    = 0;       /* number of values stored           */
static size_t  a_cap    = 0;       /* current allocation capacity       */
static idx_t   a_next   = 1;       /* next integer to examine           */

/* Return true iff n can be expressed as a^2 + b^2. */
static bool
is_sum_of_two_squares(idx_t n)
{
    if (n == 0) return true;

    /* Remove powers of 2 – they are irrelevant for the test */
    while ((n & 1) == 0) n >>= 1;

    /* Trial division by odd primes only */
    for (idx_t p = 3; p <= n / p; p += 2) {
        if (n % p == 0) {
            unsigned int exp = 0;
            while (n % p == 0) {
                n /= p;
                ++exp;
            }
            /* If p ≡ 3 (mod 4) and its exponent is odd, n is NOT a sum of two squares */
            if ((p & 3) == 3 && (exp & 1)) return false;
        }
    }
    /* Remaining factor > sqrt(original) */
    if (n > 1 && (n & 3) == 3) return false;

    return true;
}

/* Ensure the cache contains at least ‘target’ terms. */
static void
generate_until(idx_t target)
{
    while (a_len < (size_t)target) {
        /* Accept only numbers ≡ 1 or 2 (mod 4) */
        if ((a_next & 3) == 1 || (a_next & 3) == 2) {
            if (!is_sum_of_two_squares(a_next)) {
                /* Grow cache if needed */
                if (a_len + 1 > a_cap) {
                    size_t new_cap = a_cap ? 2 * a_cap : 128;
                    seq_t *tmp = (seq_t *)realloc(a_cache, new_cap * sizeof(seq_t));
                    if (!tmp) exit(EXIT_FAILURE);
                    a_cache = tmp;
                    a_cap   = new_cap;
                }
                a_cache[a_len++] = (seq_t)a_next;
            }
        }
        ++a_next;
    }
}

/* Public entry point required by the OEIS harness. */
seq_t
a(idx_t n)
{
    if (n == 0) return 0;                     /* offset is 1 – unused */
    if (a_len < (size_t)n) generate_until(n); /* generate required terms */
    return a_cache[n - 1];
}
