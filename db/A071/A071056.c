/*
 * A071056  Smallest number m such that m = p(i) mod p(i)^2 for 1<=i<=n.
 * https://oeis.org/A071056
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least positive integer m such that m ≡ p(i) (mod p(i)^2)
 *            for 1 ≤ i ≤ n, where p(i) is the i‑th prime
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 6, u64 n <= 10, u128 n <= 13,
 *            big any n
 * Helpers:   egcd64, mod_inverse
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Use the Chinese Remainder Theorem iteratively.  For each new prime
    we solve                m' = m + M·k              (mod M·p^2)
    with                       M · k ≡ p – m  (mod p^2)
    where M is the current modulus (product of the previous p^2).
    k is obtained by multiplying the difference by the modular inverse
    of M modulo p^2.  All arithmetic is performed with type seq_t.  */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/egcd64.h"
#include "../utils/mod_inverse.h"

/* ---------- small sieve of Eratosthenes ---------- */
#define PRIME_CAP 200000          /* enough for the 10 000th prime (≈104 729) */
static uint32_t primes[PRIME_CAP];
static size_t prime_cnt = 0;

static void init_primes(void)
{
    if (prime_cnt != 0) return;               /* already initialised */
    size_t limit = PRIME_CAP;
    bool *is_composite = (bool*)malloc(limit + 1);
    if (!is_composite) return;                /* worst case: no primes generated */
    for (size_t i = 0; i <= limit; ++i) is_composite[i] = false;
    size_t cnt = 0;
    for (size_t i = 2; i <= limit && cnt < PRIME_CAP; ++i)
        if (!is_composite[i]) {
            primes[cnt++] = (uint32_t)i;
            if (i * i <= limit)
                for (size_t j = i * i; j <= limit; j += i)
                    is_composite[j] = true;
        }
    prime_cnt = cnt;
    free(is_composite);
}

/* ---------- main sequence function ---------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* out of range; not used by tests */

    init_primes();

    if (n > prime_cnt) return 0;     /* safety – should not happen */

    seq_t m = 0;        /* current solution         */
    seq_t M = 1;        /* current modulus (product) */

    for (size_t i = 0; i < (size_t)n; ++i)
    {
        seq_t p  = (seq_t)primes[i];
        seq_t p2 = p * p;                 /* p^2  (fits in 64 bits) */

        if (M == 1)                /* first prime, trivial */
        {
            m = p % p2;
        }
        else
        {
            seq_t Mmod = M % p2;               /* M mod p^2   */
            seq_t inv   = mod_inverse(Mmod, p2);   /* inverse of M mod p^2 */
            seq_t mmod  = m % p2;                     /* m mod p^2   */

            seq_t diff  = (p + p2 - mmod) % p2;       /* (p − m) mod p^2 */

            seq_t k = diff * inv % p2;                /* solve for k   */

            m = m + M * k;                            /* new solution   */
        }
        M = M * p2;                                 /* update modulus */
    }
    return m;
}
