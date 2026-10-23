/*
 * A039654  a(n) = prime reached by iterating f(x) = sigma(x)-1 starting at n,
 *          or -1 if no prime is ever reached.
 * https://oeis.org/A039654
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 63, u16 n <= 7499, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_big, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Compute A039654: start at n, repeatedly compute f(x)=σ(x)-1
   until a prime is reached.  The sequence is assumed to always
   reach a prime for n ≥ 2; if not, return -1.
*/
#include <stdbool.h>
#include "../utils/is_prime_big.h"
#include "../utils/sigma_of.h"

/* External linkage as requested */
seq_t a(idx_t n)
{
    if (n < 2) {
        /* undefined offset, return “no prime” */
        return (seq_t)-1;
    }

    seq_t cur = (seq_t)n;
    /* Practical safety limit to avoid infinite loops in the unlikely event
       that a prime is never reached.  The limit is far beyond expected
       iterations for n ≤ 10 000. */
    const unsigned long long LIMIT_ITER = 1000ULL;

    for (unsigned long long iter = 0; iter < LIMIT_ITER; ++iter) {
        if (is_prime_big(cur)) {
            return cur;
        }
        /* σ(x) is defined for 0 < x, and cur ≥ 2 always.  */
        idx_t idx = (idx_t)cur;           /* convert to idx_t for sigma_of */
        seq_t next = sigma_of(idx) - 1;   /* f(cur) */
        if (next == cur) {               /* reached a fixed point that is not prime */
            break;
        }
        cur = next;
    }

    /* If the loop exceeded the safety limit, return “no prime”. */
    return (seq_t)-1;
}
