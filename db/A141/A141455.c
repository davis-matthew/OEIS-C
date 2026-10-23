/*
 * A141455  Irregular triangle showing the set of all possible values of
 *          primes modulo n in row n.
 * https://oeis.org/A141455
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) = element of row n of the triangle defined as A027748(n) ∪
 *            A038566(n), with n written as 0 when n is prime
 * Range:     u8 n <= 7728, u16 n <= 7728, u32 n <= 7728, u64 n <= 7728,
 *            u128 n <= 7728, big any n
 * Helpers:   build_primes_upto, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime.h"
#include "../utils/build_primes_upto.h"

static void ensure_primes(idx_t limit, uint64_t **primes, size_t *cnt)
{
    static uint64_t *prime_list = NULL;
    static size_t prime_cnt = 0;
    static idx_t prime_limit = 0;

    if (limit <= prime_limit) return;

    if (prime_list) free(prime_list);
    build_primes_upto(limit, &prime_list, &prime_cnt);
    prime_list = prime_list;  /* avoid unused variable warning */
    prime_cnt   = prime_cnt;
    prime_limit = limit;
}

static inline seq_t egcd(seq_t a, seq_t b)
{
    while (b != 0) {
        seq_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

static inline bool coprime(idx_t a, idx_t n)
{
    return egcd((seq_t)a, (seq_t)n) == 1;
}

seq_t a(idx_t n)
{
    /* values for indices < 2 are irrelevant */
    if (n < (idx_t)2) return (seq_t)0;

    idx_t k = n - (idx_t)2;          /* zero‑based flattened index */
    idx_t cum = 0;                   /* cumulative length of rows processed */

    /* build a prime table up to the maximum row we might examine */
    /* Since the number of rows grows roughly ~2*sqrt(n), we request
       a safe upper bound of n. */
    ensure_primes(n + (idx_t)10, NULL, NULL);

    for (idx_t r = 2; ; ++r) {
        idx_t len = 0;

        if (is_prime(r)) {
            /* prime rows contain 0 plus all numbers 1..r-1 */
            len = r;
        } else {
            for (idx_t a = 1; a < r; ++a)
                if (coprime(a, r) || is_prime(a))
                    ++len;
        }

        if (cum + len > k) {
            /* target lies in this row */
            idx_t pos = k - cum;      /* zero‑based position within row */

            if (is_prime(r)) {
                if (pos == 0) return (seq_t)0;
                --pos;                /* skip the 0 entry */
            }

            /* search for the pos‑th element (0‑based) */
            idx_t count = 0;
            for (idx_t a = 1; a < r; ++a) {
                if (coprime(a, r) || is_prime(a)) {
                    if (count == pos) return (seq_t)a;
                    ++count;
                }
            }
            /* should never reach here */
            return (seq_t)0;
        }

        cum += len;
        /* loop continues until the correct row is found */
    }
}
