/*
 * A119936  Least common multiple (LCM) of denominators of the rows of the
 *          triangle of rationals A119935/A119932.
 * https://oeis.org/A119936
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n * lcm(1..n)^2
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 10, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset starts at 1, but guard anyway */

    /*  generate the primes up to n by trial division (fast enough for n≤10000) */
    static uint32_t primes[2000];   /* 1229 primes ≤10000, so 2000 is safe */
    size_t prime_cnt = 0;
    for (idx_t p = 2; p <= n; ++p) {
        bool is_prime = true;
        for (size_t i = 0; i < prime_cnt; ++i) {
            uint32_t q = primes[i];
            if ((size_t)q * (size_t)q > (size_t)p)
                break;
            if (p % q == 0) {
                is_prime = false;
                break;
            }
        }
        if (is_prime) {
            primes[prime_cnt++] = (uint32_t)p;
        }
    }

    /*  compute lcm(1..n) as product of the largest power of each prime ≤ n  */
    seq_t lcm = 1;
    seq_t n_seq = (seq_t)n;
    for (size_t i = 0; i < prime_cnt; ++i) {
        seq_t p = (seq_t)primes[i];
        seq_t power = p;
        while (1) {
            seq_t next = power * p;
            if (next > n_seq || next < power)  /* overflow guard for arbitrary‑precision types */
                break;
            power = next;
        }
        lcm *= power;
    }

    /*  a(n) = n * lcm(1..n)^2  */
    seq_t result = lcm * lcm;
    result *= n_seq;
    return result;
}
