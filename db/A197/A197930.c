/*
 * A197930  Numbers n such that the number of distinct residues in x^(n-1)
 *          (mod n), x=0..n-1, equals n.
 * https://oeis.org/A197930
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 45, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   factor_u64, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A197930: Numbers n such that the number of distinct
   residues in x^(n-1) (mod n), x = 0..n-1, equals n. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/is_prime.h"
#include "../utils/factor_u64.h"

static inline uint64_t _gcd_u64(uint64_t a, uint64_t b)
{
    while (b) {
        uint64_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* Test whether n = 2*m satisfies the property.
   m must be odd. For m = 1 it is true.
   For prime m it is true.
   Otherwise m must be square‑free and
   gcd(2*m-1, λ(m)) = 1, where λ(m) = lcm(p-1) over the
   distinct prime factors p of m. */
static bool _candidate_ok(idx_t m)
{
    if (m == 1)                     /* n = 2 => 1 passes */
        return true;
    if (is_prime(m))
        return true;

    /* m is composite odd – factor it */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt;
    factor_u64((uint64_t)m, primes, exps, &cnt);

    bool squarefree = true;
    uint64_t lambda = 1;
    for (size_t i = 0; i < cnt; ++i) {
        if (exps[i] > 1) {          /* not square‑free */
            squarefree = false;
            break;
        }
        uint64_t val = primes[i] - 1;
        uint64_t g = _gcd_u64(lambda, val);
        lambda = lambda / g * val;
    }

    if (!squarefree)
        return false;

    uint64_t e = 2 * (uint64_t)m - 1;
    return (_gcd_u64(e, lambda) == 1);
}

seq_t a(idx_t n)
{
    /* static state for memoization */
    static seq_t *seq      = NULL;
    static size_t seq_len  = 0;
    static size_t seq_cap  = 0;
    static idx_t  next_m   = 1;          /* m for generating 2*m */

    /* first term is 1 */
    if (seq_len == 0) {
        seq_cap = 16;
        seq_len = 1;
        seq = (seq_t *)malloc(seq_cap * sizeof(seq_t));
        if (!seq) exit(1);              /* out of memory */
        seq[0] = (seq_t)1;
    }

    /* generate terms until we reach index n */
    while (seq_len < n) {
        idx_t m = next_m;
        ++next_m;

        /* skip even m > 1 (which give n divisible by 4) */
        if (m > 1 && ((m & 1) == 0))
            continue;

        if (_candidate_ok(m)) {
            /* ensure capacity */
            if (seq_len >= seq_cap) {
                seq_cap *= 2;
                seq = (seq_t *)realloc(seq, seq_cap * sizeof(seq_t));
                if (!seq) exit(1);
            }
            seq[seq_len] = (seq_t)2 * (seq_t)m;
            ++seq_len;
        }
    }

    return seq[n - 1];
}
