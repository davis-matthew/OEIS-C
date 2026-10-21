/*
 * A089912  a(n) gives sum of number of common unitary divisors of n and m,
 *          where m runs from 1 to n.
 * https://oeis.org/A089912
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 164, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = sum_{m=1..n} 2^{s(m)}
 * where s(m) = number of distinct prime factors p of n such that
 *   (a) p^e divides m
 *   (b) m has no power of p higher than e,
 * i.e. exponent of p in m equals e, the exponent of p in n.
 * The function factors n once per call and brute–forces the sum for
 * 1 ≤ m ≤ n.  For n ≤ 10 000 this is easily fast enough.
 *
 * The result is returned as a seq_t; all multiplications are
 * performed using the same type to keep intermediate size minimal.
 */

#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* out‑of‑range index */

    /* factor n into primes and exponents */
    uint64_t temp = (uint64_t)n;     /* n fits in uint64_t for all tests */
    uint64_t prime[16];
    uint64_t expo[16];
    size_t pcnt = 0;

    for (uint64_t p = 2; p * p <= temp; ++p) {
        if (temp % p == 0) {
            uint64_t e = 0;
            while (temp % p == 0) {
                temp /= p;
                ++e;
            }
            prime[pcnt] = p;
            expo[pcnt] = e;
            ++pcnt;
        }
    }
    if (temp > 1) {                   /* remaining prime factor */
        prime[pcnt] = temp;
        expo[pcnt] = 1;
        ++pcnt;
    }

    seq_t sum = 0;

    /* brute force over all m from 1 to n */
    for (idx_t m = 1; m <= n; ++m) {
        idx_t mcopy = m;
        int s = 0;                    /* number of primes whose e matches */

        for (size_t i = 0; i < pcnt; ++i) {
            uint64_t p = prime[i];
            uint64_t e = expo[i];
            int cnt = 0;

            while (mcopy % p == 0) {  /* count exponent of p in m */
                mcopy /= p;
                ++cnt;
            }
            if (cnt == (int)e) {
                ++s;
            }
        }

        seq_t contrib = (seq_t)1 << s; /* 2^s, cast before shift */
        sum += contrib;
    }

    return sum;
}
