/*
 * A059225  Primes p such that x^31 = 2 has no solution mod p.
 * https://oeis.org/A059225
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 216, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A059225: primes p such that x^31 = 2 has no solution modulo p.
 *
 * We find all primes p with p ≡ 1 (mod 31) and test whether
 *
 *      2^((p‑1)/31) ≠ 1 (mod p).
 *
 * The sequence is generated lazily: subsequent calls to a(n) will
 * continue from where the previous call left off, so the function
 * works in any order of n and is efficient for n up to about 10 000.
 *
 * All arithmetic for the modular test is performed with 64‑bit
 * unsigned integers; the result is cast to `seq_t` when stored in
 * the list.  This guarantees exactness for any term that fits into
 * the output type.  The helper functions `next_prime` and the
 * modular exponentiation routine are provided by the utils library.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/next_prime.h"

/* fast modular multiplication using 128‑bit intermediate */
static uint64_t mul_mod_uint64(uint64_t a, uint64_t b, uint64_t mod)
{
    return (uint64_t)(((__uint128_t)a * b) % mod);
}

/* fast modular exponentiation (binary exponentiation) */
static uint64_t pow_mod_uint64(uint64_t base, uint64_t exp, uint64_t mod)
{
    uint64_t result = 1 % mod;
    uint64_t cur = base % mod;
    while (exp) {
        if (exp & 1)
            result = mul_mod_uint64(result, cur, mod);
        cur = mul_mod_uint64(cur, cur, mod);
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    /* static cache of found sequence values */
    static seq_t *seq      = NULL;   /* array of sequence terms */
    static size_t seq_cnt  = 0;      /* number of stored terms   */
    static size_t seq_cap  = 0;      /* capacity of the array    */

    /* static state for generating next prime */
    static uint64_t cur = 2;         /* number to start searching from */

    /* continue generating terms until we have at least n of them */
    while (seq_cnt < n) {
        uint64_t p = next_prime(cur);   /* next prime ≥ cur */
        cur = p + 1;                    /* next search start */

        /* Only primes that are 1 mod 31 can fail the 31‑th power test */
        if ((p - 1) % 31 != 0)
            continue;

        /* Test whether 2^((p‑1)/31) ≠ 1 (mod p) */
        uint64_t exponent = (p - 1) / 31;
        uint64_t pow = pow_mod_uint64(2, exponent, p);
        if (pow != 1) {
            /* grow the sequence array if necessary */
            if (seq_cnt == seq_cap) {
                size_t new_cap = seq_cap ? seq_cap * 2 : 1;
                seq_t *tmp = (seq_t*)realloc(seq, new_cap * sizeof(seq_t));
                seq = tmp;
                seq_cap = new_cap;
            }
            seq[seq_cnt++] = (seq_t)p;
        }
    }

    return seq[n - 1];
}
