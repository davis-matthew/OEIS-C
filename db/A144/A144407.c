/*
 * A144407  a(n) = A058529(n+1)^2.
 * https://oeis.org/A144407
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A058529(n+1)^2. A058529 are the integers whose (prime)
 *            factors are all ≡ 1 or 7 (mod 8), with the convention that 1 is
 *            included as the 1st term
 * Range:     u8 n <= 1, u16 n <= 28, u32 n <= 5583, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static bool is_good(uint64_t x)
{
    if (x == 1) return true;          /* 1 is allowed */
    uint64_t tmp = x;
    /* test prime factors up to sqrt(tmp) */
    for (uint64_t p = 2; p * p <= tmp; ++p) {
        if (tmp % p == 0) {
            /* p is a prime factor; check its residue */
            while (tmp % p == 0) {
                if (p % 8 != 1 && p % 8 != 7) {
                    return false;
                }
                tmp /= p;
            }
        }
    }
    /* any remaining factor > sqrt(tmp) is prime */
    if (tmp > 1) {
        if (tmp % 8 != 1 && tmp % 8 != 7) {
            return false;
        }
    }
    return true;
}

seq_t a(idx_t n)
{
    /* We need the (n+1)-th term of A058529, then square it */
    idx_t target = n + 1;   /* 1‑indexed */
    idx_t count = 0;
    uint64_t candidate = 1;

    while (true) {
        if (is_good(candidate)) {
            ++count;
            if (count == target) {
                seq_t v = (seq_t)candidate;
                return v * v;
            }
        }
        ++candidate;
    }
}
