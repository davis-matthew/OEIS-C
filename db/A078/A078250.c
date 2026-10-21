/*
 * A078250  a(1) = 1, a(n) = smallest multiple of n using prime digits if n is
 *          composite else smallest multiple of n using composite digits, with
 *          a(n) = 0 if there are none.
 * https://oeis.org/A078250
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = smallest multiple of n whose decimal digits are prime
 *            (2,3,5,7) if n is composite, composite (0,4,6,8,9) if n is
 *            prime, leading digit forbidden from 0, a(1) = 1. If no such
 *            multiple exists, return 0
 * Range:     u8 n <= 12, u16 n <= 181, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   bfs_min_multiple
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation uses a breadth‑first search over the
   remainders modulo n to find the minimal digit‑restricted multiple.
   The search is guaranteed to terminate because there are only n
   possible remainders; if none lead to 0 we return 0.  */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/bfs_min_multiple.h"

typedef seq_t seq_t;
typedef idx_t idx_t;

/* Simple deterministic primality test for n ≤ 10^8  */
static bool is_prime_u64(idx_t n)
{
    if (n < 2) return false;
    if (n == 2 || n == 3) return true;
    if (n % 2 == 0 || n % 3 == 0) return false;
    for (idx_t i = 5; i * i <= n; i += 6)
        if (n % i == 0 || n % (i + 2) == 0)
            return false;
    return true;
}

seq_t a(idx_t n)
{
    if (n == 1) return 1;

    /* digit sets */
    static const int prime_digits[]      = {2,3,5,7};
    static const int composite_digits[]   = {0,4,6,8,9};
    static const int composite_start[]    = {4,6,8,9};

    if (is_prime_u64(n)) {
        /* prime: use composite digits */
        return bfs_min_multiple(n,
                                composite_digits,
                                sizeof(composite_digits)/sizeof(int),
                                composite_start,
                                sizeof(composite_start)/sizeof(int));
    } else {
        /* composite: use prime digits */
        return bfs_min_multiple(n,
                                prime_digits,
                                sizeof(prime_digits)/sizeof(int),
                                prime_digits,
                                sizeof(prime_digits)/sizeof(int));
    }
}
