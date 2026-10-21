/*
 * A069260  a(n) = core(1)*core(2)*...*core(n) where core(n) is the squarefree
 *          part of n (A007913).
 * https://oeis.org/A069260
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Π_{p≤n} p^{ Σ_{k≥1} (-1)^{k+1} ⌊n/p^k⌋ }
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 16, u64 n <= 26, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* product of the squarefree parts of 1…n */
seq_t a(idx_t n)
{
    if (n < 1) return 1;                     /* 1 is the first displayed term   */
    if (n == 1) return 1;                    /* core(1)=1                        */

    /* sieve primes up to n */
    idx_t size = n + 1;
    uint8_t *composite = (uint8_t*)calloc(size, 1);
    for (idx_t i = 2; (idx_t)(i * i) <= n; ++i)
        if (!composite[i])
            for (idx_t j = i * i; j <= n; j += i)
                composite[j] = 1;

    seq_t result = 1;

    for (idx_t p = 2; p <= n; ++p) {
        if (composite[p]) continue;

        /* compute exponent of p in a(n) */
        idx_t val = p, exp = 0;
        bool sign = true;                    /* true → +, false → – */
        while (val <= n) {
            idx_t add = n / val;
            exp += sign ? add : -add;
            if (val > n / p) break;          /* prevent overflow of val*p   */
            val *= p;
            sign = !sign;
        }

        /* multiply result by p^exp using exponentiation by squaring */
        seq_t power = 1, base = (seq_t)p;
        idx_t e = exp;
        while (e > 0) {
            if (e & 1) power *= base;
            base *= base;
            e >>= 1;
        }
        result *= power;
    }

    free(composite);
    return result;
}
