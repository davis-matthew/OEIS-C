/*
 * A107079  Minimal number of squared primes in a squarefree gap of length n.
 * https://oeis.org/A107079
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 + number of squarefree integers in [1, n-1] (mu(0) is
 *            treated as 1, so a(1)=1). This follows from the definition a(n)
 *            = Σ_{k=0}^{n-1} (mu(n‑k‑1) mod 2)
 * Range:     u8 n <= 418, u16 n <= 9153, u32 n <= 9153, u64 n <= 9153,
 *            u128 n <= 9153, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static const idx_t _sieve_primes[] = {2,3,5,7,11,13,17,19,23,29,31,37,41,43,47,
                                     53,59,61,67,71,73,79,83,89,97};
static const int _sieve_prime_cnt = 25;

seq_t a(idx_t n)
{
    /* The sequence is defined only for n ≥ 1.  If called with n == 0
       (outside the offset) return 0.  This value will never be checked. */
    if (n == 0) return 0;

    seq_t count = 1;                     /* mu(0) is treated as 1 */

    for (idx_t i = 1; i < n; ++i) {
        /* Test whether i is square‑free */
        int is_squarefree = 1;
        for (int pi = 0; pi < _sieve_prime_cnt; ++pi) {
            idx_t p  = _sieve_primes[pi];
            idx_t pp = p * p;
            if (pp > i) break;
            if (i % pp == 0) {          /* divisible by a square prime */
                is_squarefree = 0;
                break;
            }
        }
        if (is_squarefree) ++count;     /* include in the sum */
    }
    return count;
}
