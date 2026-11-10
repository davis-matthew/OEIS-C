/*
 * A078428  Partial sums of A035187.
 * https://oeis.org/A078428
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, changed
 * Formula:   a(n) = Sum_{k=1..n+1} A035187(k), where A035187(k) = Sum_{d|k}
 *            Kronecker(5,d) (multiplicative)
 * Range:     u8 n <= 75, u16 n <= 75, u32 n <= 75, u64 n <= 75, u128 n <= 75,
 *            big any n
 * Validated: partial, pass, 76 terms through n = 75 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static seq_t A035187_a(idx_t n)
{
    if (n == 0)
        return 0;

    seq_t result = 1;
    idx_t x = n;

    for (idx_t p = 2; p * p <= x; ++p) {
        if (x % p == 0) {
            idx_t e = 0;
            while (x % p == 0) {
                x /= p;
                ++e;
            }

            seq_t mul;
            if (p == 5) {
                mul = 1;
            } else {
                uint64_t mod = (uint64_t)p % 5;
                if (mod == 1 || mod == 4) {
                    mul = (seq_t)(e + 1);
                } else {
                    mul = (e % 2 == 0) ? 1 : 0;
                }
            }

            result *= mul;
            if (result == 0)
                return 0;
        }
    }

    if (x > 1) {
        seq_t mul;
        if (x == 5) {
            mul = 1;
        } else {
            uint64_t mod = (uint64_t)x % 5;
            if (mod == 1 || mod == 4) {
                mul = 2;
            } else {
                mul = 0;
            }
        }
        result *= mul;
    }

    return result;
}

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t k = 1; k <= n + 1; ++k) {
        sum += A035187_a(k);
    }
    return sum;
}
