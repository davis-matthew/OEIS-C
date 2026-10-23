/*
 * A181662  a(n) is the smallest positive integral multiple of 2^n not in the
 *          range of the Euler phi function.
 * https://oeis.org/A181662
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A058887(n) * 2^n where A058887(n) is the smallest odd
 *            prime p such that 2^e * p + 1 is composite for all 0 ≤ e ≤ n
 * Range:     u8 n <= 3, u16 n <= 10, u32 n <= 26, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   build_primes_upto, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation precomputes all primes up to 272000 (≥271129),
   then for each requested n scans primes until it finds a prime
   satisfying the condition.  The result is computed as 2^n * p.  */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#include "../utils/build_primes_upto.h"
#include "../utils/is_prime_big.h"

/*--- Prime table -----------------------------------------------------*/
static uint64_t *primes = NULL;
static size_t    prime_cnt = 0;
static bool      primes_ready = false;

static void ensure_primes(void)
{
    if (!primes_ready) {
        /* 272000 > 271129 guarantees that every needed prime is included. */
        build_primes_upto(272000, &primes, &prime_cnt);
        primes_ready = true;
    }
}

/*--- Small power of two ------------------------------------------------*/
static inline seq_t pow2_local(idx_t exp)
{
    seq_t res = (seq_t)1;
    while (exp--) {
        res <<= 1;          /* shift left; works for all seq_t sizes */
    }
    return res;
}

/*--- Sequence function ------------------------------------------------*/
seq_t a(idx_t n)
{
    ensure_primes();

    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = primes[i];
        bool ok = true;

        /* Test the composite condition for all e = 0 .. n */
        for (idx_t e = 0; e <= n; ++e) {
            seq_t val = pow2_local(e);
            val *= (seq_t)p;
            val += (seq_t)1;

            if (is_prime_big(val)) {   /* composite required, so prime is bad */
                ok = false;
                break;
            }
        }

        if (ok) {
            seq_t res = pow2_local(n);
            res *= (seq_t)p;
            return res;
        }
    }

    /* In theory this point is never reached for valid n.  */
    return (seq_t)0;
}
