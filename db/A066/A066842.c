/*
 * A066842  a(n) = Product_{k|n} k^k; product is over the positive divisors,
 *          k, of n.
 * https://oeis.org/A066842
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, changed
 * Formula:   a(n) = Product_{k|n} k^k
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 7, u64 n <= 11, u128 n <= 19,
 *            big any n
 * Validated: standard, pass, 31 terms through n = 100 (2026-11-01)
 * Helpers:   ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/ipow.h"

/* Helper to factor n into primes (pr) and exponents (ex).  */
static void
_factor(uint64_t n, uint64_t *pr, int *ex, size_t *cnt)
{
    size_t c = 0;
    for (uint64_t p = 2; p * p <= n; ++p) {
        if (n % p == 0) {
            int e = 0;
            while (n % p == 0) {
                n /= p;
                ++e;
            }
            pr[c] = p;
            ex[c] = e;
            ++c;
        }
    }
    if (n > 1) {
        pr[c] = n;
        ex[c] = 1;
        ++c;
    }
    *cnt = c;
}

/* Recursively generate all divisors of n.  For each divisor d,
   multiply the running product by d^d. */
static void
_gen_divs(size_t idx,
          uint64_t cur,
          seq_t *res,
          const uint64_t *pr,
          const int *ex,
          size_t cnt)
{
    if (idx == cnt) {
        seq_t val   = (seq_t)cur;      /* divisor d as seq_t   */
        seq_t pow   = ipow(val, val); /* d^d                 */
        *res        = (*res) * pow;
        return;
    }
    uint64_t mult = cur;
    for (int e = 0; e <= ex[idx]; ++e) {
        _gen_divs(idx + 1, mult, res, pr, ex, cnt);
        mult *= pr[idx];                /* multiply by prime for next exponent */
    }
}

/* External linkage: compute a(n).  n >= 1.  */
seq_t
a(idx_t n)
{
    /* n is at least 1; cast to uint64_t for internal use.  */
    uint64_t u = (uint64_t)n;

    /* Small fixed buffers: n <= 10000 has at most 5 distinct primes.  */
    uint64_t primes[8];
    int      exps[8];
    size_t   cnt;

    _factor(u, primes, exps, &cnt);

    seq_t result = (seq_t)1;
    _gen_divs(0, (uint64_t)1, &result, primes, exps, cnt);

    return result;
}
