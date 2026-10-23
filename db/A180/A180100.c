/*
 * A180100  a(n) = smallest number k such that five consecutive prime numbers
 *          prime(n), prime(n+1), prime(n+2), prime(n+3) and prime(n+4) are
 *          divisors of k, k+1, k+2, k+3, and k+4 respectively.
 * https://oeis.org/A180100
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   * a(n) = smallest k such that prime(n) | k, * prime(n+1) | k+1,
 *            * prime(n+2) | k+2, * prime(n+3) | k+3, * prime(n+4) | k+4. *
 *            The value is the unique solution modulo the product of the five
 *            primes. * We compute it by the Chinese‑Remainder Theorem
 * Range:     u8 none, u16 n <= 3, u32 n <= 23, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/get_prime.h"

/* modular inverse of a modulo mod (mod is prime, a < mod) */
static uint64_t modinv_uint64(uint64_t a, uint64_t mod)
{
    int64_t t = 0, newt = 1;
    int64_t r = mod, newr = a;
    while (newr != 0) {
        uint64_t q = r / newr;
        int64_t tmp = newt;
        newt = t - (int64_t)((__int128)q * newt);
        t = tmp;
        tmp = newr;
        newr = r - q * newr;
        r = tmp;
    }
    if (r > 1) return 0;                  /* no inverse – shouldn’t happen */
    if (t < 0) t += mod;
    return (uint64_t)t;
}

seq_t a(idx_t n)
{
    /* fetch five consecutive primes; indices are 1‑based */
    uint64_t p[5];
    p[0] = get_prime(n);
    p[1] = get_prime(n + 1);
    p[2] = get_prime(n + 2);
    p[3] = get_prime(n + 3);
    p[4] = get_prime(n + 4);

    /* residues r[i] = -i (mod p[i]) */
    uint64_t r[5];
    for (int i = 0; i < 5; ++i)
        r[i] = (p[i] - (uint64_t)i) % p[i];

    seq_t cur = 0;        /* current solution modulo processed primes */
    seq_t mod = 1;        /* product of processed primes */

    for (int i = 0; i < 5; ++i) {
        /* product of previous primes modulo p[i] */
        uint64_t prod = 1 % p[i];
        for (int j = 0; j < i; ++j)
            prod = (prod * p[j]) % p[i];

        uint64_t inv = modinv_uint64(prod, p[i]);

        uint64_t cur_mod = (uint64_t)(cur % (seq_t)p[i]);

        uint64_t diff = (r[i] >= cur_mod) ? (r[i] - cur_mod)
                                           : (r[i] + p[i] - cur_mod);

        uint64_t t = (uint64_t)(((unsigned __int128)diff * inv) % p[i]);

        cur = cur + mod * (seq_t)t;
        mod = mod * (seq_t)p[i];
    }

    return cur;   /* minimal non‑negative solution */
}
