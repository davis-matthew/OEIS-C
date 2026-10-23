/*
 * A134834  Let {b_n(m)} be a sequence defined by b_n(0)=1, b_n(m) is the
 *          largest prime dividing (b_n(m-1) + n). Then a(n) is the smallest
 *          positive integer such that b_n(m + a(n)) = b_n(m), for all
 *          integers m that are greater than some positive integer M.
 * https://oeis.org/A134834
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 83, u32 n <= 83, u64 n <= 83, u128 n <= 83,
 *            big any n
 * Helpers:   build_primes_upto, floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function computes a(n), the minimal period of the sequence
 * b_n defined by
 *
 *   b_n(0) = 1,
 *   b_n(m) = largest prime divisor of (b_n(m-1) + n)
 *
 *  The smallest positive integer a(n) satisfies
 *   b_n(m + a(n)) = b_n(m)   for all sufficiently large m.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/floor_sqrt.h"
#include "../utils/build_primes_upto.h"

/* Prime table used for factorisation of the numbers appearing in the
 * recurrence.  The primes are stored as 64‑bit integers (they are
 * sufficient for the expected range of numbers in this sequence). */
static uint64_t *primes   = NULL;   /* array of primes */
static size_t   primes_len   = 0;   /* number of primes stored */
static idx_t    primes_limit = 0;   /* current maximum prime value present */

/* Ensure that primes array contains all primes ≤ limit. */
static void ensure_primes_upto(idx_t limit)
{
    if (limit <= primes_limit)
        return;
    uint64_t *new_primes = NULL;
    size_t   new_len     = 0;
    build_primes_upto(limit, &new_primes, &new_len);
    free(primes);
    primes   = new_primes;
    primes_len = new_len;
    primes_limit = limit;
}

/* Largest prime divisor of a seq_t number (num > 1).  The algorithm
 * performs a trial division using the primes stored in the static
 * table.  For numbers larger than (UINT64_MAX)^2 we limit the search
 * to the largest available prime, which is sufficient for the
 * intended range of inputs. */
static seq_t gpf_large(seq_t num)
{
    seq_t x      = num;
    seq_t max_f  = 1;
    seq_t sqrt_x = floor_sqrt(x);

    /* Convert sqrt to a bounded idx_t. */
    idx_t limit = (sqrt_x > (seq_t)UINT64_MAX) ? UINT64_MAX : (idx_t)sqrt_x;
    ensure_primes_upto(limit);

    for (size_t i = 0; i < primes_len; ++i) {
        seq_t p = (seq_t)primes[i];
        if (x < p)
            break;
        while (x % p == 0) {
            max_f = p;
            x    /= p;
        }
        if (x == 1)
            break;
    }

    /* If a prime factor > sqrt(x) remains, it is the largest prime factor. */
    if (x > 1 && x > max_f)
        max_f = x;
    return max_f;
}

/* Compute a(n).  The function is cache‑friendly and works
 * independently of the build (uint64_t or arbitrary precision). */
seq_t a(idx_t n)
{
    /* Buffer holding the sequence values encountered so far.
     * We grow it as needed. */
    seq_t *visited = NULL;
    size_t visited_len = 0;
    size_t visited_cap = 16;   /* initial capacity */
    visited = (seq_t *)malloc(visited_cap * sizeof(seq_t));
    if (!visited)
        return 0;              /* memory allocation failure – unlikely */

    /* start with b(0) = 1 */
    visited[0] = (seq_t)1;
    visited_len = 1;

    seq_t current = (seq_t)1;
    seq_t N = (seq_t)n;

    for (;;) {
        /* next = largest prime divisor of current + N */
        seq_t sum     = current + N;
        seq_t next_val = gpf_large(sum);

        /* Detect cycle: if next_val has appeared before, the period
         * is the distance to its first occurrence. */
        for (size_t i = 0; i < visited_len; ++i) {
            if (visited[i] == next_val) {
                seq_t period = (seq_t)(visited_len - i);
                free(visited);
                return period;
            }
        }

        /* Store the new value, growing the buffer if necessary. */
        if (visited_len == visited_cap) {
            visited_cap *= 2;
            visited = (seq_t *)realloc(visited, visited_cap * sizeof(seq_t));
            if (!visited)
                return 0; /* memory allocation failure */
        }
        visited[visited_len++] = next_val;
        current = next_val;
    }
}
