/*
 * A126660  Array read by antidiagonals: A(k,n) = smallest odd prime q such
 *          that pq+2k is prime, where p is the n-th odd prime, or 0 if no
 *          such prime exists.
 * https://oeis.org/A126660
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) = array read by antidiagonals of * A(k,n) = smallest odd
 *            prime q such that p*q+2k is prime, * where p is the n-th odd
 *            prime. If no such prime exists, * a(n) = 0. * The sequence is
 *            defined for n >= 1 with offset 1
 * Range:     u8 n <= 16, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   get_prime, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/get_prime.h"
#include "../utils/is_prime_big.h"

/* Upper bound for q to avoid infinite search. */
#define Q_LIMIT 200000

static seq_t
solve_entry(idx_t k, idx_t col)
{
    /* p = nth odd prime at column 'col' (1‑based) */
    seq_t p = (seq_t) get_prime(col + 1);  /* col=1 returns the 2nd prime, 3 */

    /* Search for smallest odd prime q such that p*q + 2k is prime. */
    for (idx_t q_index = 2; /* start at 3 */; ++q_index) {
        seq_t q = (seq_t) get_prime(q_index);   /* q increases over odd primes */
        if (q > (seq_t) Q_LIMIT)   /* give up if q exceeds bound */
            break;

        seq_t val = p * q + ((seq_t)2 * (seq_t)k);
        if (is_prime_big(val))
            return q;
    }

    return 0;
}

seq_t a(idx_t n)
{
    /* Find diagonal s such that the cumulative count up to that
     * diagonal just exceeds n.  The cumulative count up to
     * diagonal s (sum of indices = s) is T_{s-1} = (s-1)*s/2. */
    idx_t m = 0;          /* will end up as the smallest integer with
                            T_m >= n */
    idx_t cumulative = 0;
    while (cumulative < n) {
        ++m;
        cumulative += m;
    }

    idx_t s = m + 1;                         /* diagonal sum k + n = s   */
    idx_t T_prev = ((m - 1) * m) / 2;        /* T_{s-2} = cumulative of previous diag */
    idx_t offset = n - T_prev - 1;

    idx_t k = m - offset;                    /* row index   (1‑based)   */
    idx_t col = offset + 1;                  /* column index (1‑based)   */

    return solve_entry(k, col);
}
