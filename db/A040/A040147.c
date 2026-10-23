/*
 * A040147  Primes p such that x^4 = 20 has a solution mod p.
 * https://oeis.org/A040147
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 20, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   grow_seq, is_prime, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Tests whether 20 is a fourth power residue modulo p;
   returns the nth prime p such that ∃x with x^4 ≡ 20 (mod p). */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime.h"
#include "../utils/mod_pow.h"
#include "../utils/grow_seq.h"

seq_t a(idx_t n) {
    static seq_t *primes = NULL;   /* cached primes that satisfy the condition */
    static size_t len   = 0;        /* number of stored primes */
    static size_t cap   = 0;        /* allocated capacity */
    static uint64_t last = 2;       /* last integer checked for primality */

    while (len < (size_t)n) {          /* generate new terms until we have enough */
        /* find next prime ≥ last */
        uint64_t p = last;
        while (!is_prime((idx_t)p)) ++p;

        bool good = false;
        if (p == 2 || p == 5) {
            good = true;              /* 20 is 0 mod p → trivial solution */
        } else {
            /* compute gcd(4, p-1). For odd primes, it's 4 if p≡1 (mod 4), else 2 */
            uint64_t g = (p % 4 == 1) ? 4 : 2;
            uint64_t exp = (p - 1) / g;
            /* a = 20 mod p (which is non‑zero here) */
            seq_t a = (seq_t)(20 % p);
            seq_t r = mod_pow(a, exp, (seq_t)p);
            good = (r == (seq_t)1);
        }

        if (good) {
            if (len == cap) {
                grow_seq(&primes, &cap, len + 1);
                /* grow_seq might reallocate; ensure primes isn't NULL now */
                if (!primes) return 0;
            }
            primes[len++] = (seq_t)p;
        }

        last = p + 1;          /* start searching from the next number */
    }

    return primes[n - 1];
}
