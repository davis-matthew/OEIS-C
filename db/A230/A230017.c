/*
 * A230017  Prime terms in the tribonacci-like sequence, A214825.
 * https://oeis.org/A230017
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th prime term of A214825, where b(k) = b(k-1) + b(k-2)
 *            + b(k-3), b(0) = 1, b(1) = b(2) = 3; computed by generating b(k)
 *            and testing each term for primality (a(1) = a(2) = 3 because
 *            b(1) and b(2) are both 3)
 * Range:     u8 n <= 7, u16 n <= 10, u32 n <= 12, u64 n <= 12, u128 n <= 16,
 *            big any n
 * Validated: partial, pass, 16 terms through n = 16 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>

/* (a + b) mod m for a, b < m, without overflow */
static seq_t A230017_addmod(seq_t a, seq_t b, seq_t m)
{
    seq_t t = m - b;
    if (a >= t) return a - t;
    return a + b;
}

/* (a * b) mod m, overflow-safe in fixed-width builds */
static seq_t A230017_mulmod(seq_t a, seq_t b, seq_t m)
{
    a %= m;
    b %= m;
    if (a == 0 || b == 0) return (seq_t)0;
    {
        seq_t p = a * b;
        if (p / a == b) return p % m;
    }
    {
        seq_t r = 0;
        while (b > 0) {
            if ((b & 1) != 0) r = A230017_addmod(r, a, m);
            a = A230017_addmod(a, a, m);
            b >>= 1;
        }
        return r;
    }
}

static seq_t A230017_powmod(seq_t base, seq_t e, seq_t m)
{
    seq_t r = 1;
    base %= m;
    while (e > 0) {
        if ((e & 1) != 0) r = A230017_mulmod(r, base, m);
        e >>= 1;
        if (e > 0) base = A230017_mulmod(base, base, m);
    }
    return r % m;
}

static const unsigned A230017_small_primes[] = {
    2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47,
    53, 59, 61, 67, 71, 73, 79, 83, 89, 97
};

static bool A230017_is_prime(seq_t m)
{
    size_t np = sizeof(A230017_small_primes) / sizeof(A230017_small_primes[0]);
    size_t i;
    seq_t d, m1;
    unsigned s = 0;
    if (m < 2) return false;
    for (i = 0; i < np; i++) {
        unsigned p = A230017_small_primes[i];
        if (m == p) return true;
        if (m % p == 0) return false;
    }
    if (m < 9409) return true;   /* 97^2 */
    m1 = m - 1;
    d = m1;
    while ((d & 1) == 0) { d >>= 1; s++; }
    for (i = 0; i < 20 && i < np; i++) {
        seq_t x = A230017_powmod((seq_t)A230017_small_primes[i], d, m);
        unsigned r;
        bool ok = false;
        if (x == 1 || x == m1) continue;
        for (r = 1; r < s; r++) {
            x = A230017_mulmod(x, x, m);
            if (x == m1) { ok = true; break; }
        }
        if (!ok) return false;
    }
    return true;
}

seq_t a(idx_t n)
{
    seq_t x = 1, y = 3, z = 3;   /* b(k-3), b(k-2), b(k-1) */
    idx_t count;
    if (n == 1 || n == 2) return (seq_t)3;
    count = 2;
    for (;;) {
        seq_t s = x + y + z;
        if (s < z) return (seq_t)0;   /* overflow in fixed-width build */
        x = y; y = z; z = s;
        if (A230017_is_prime(s)) {
            count++;
            if (count == n) return s;
        }
    }
}
