/*
 * A053674  Least number coprime to n, n+1, n+2, n+3, n+4 and n+5.
 * https://oeis.org/A053674
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = smallest prime p>5 such that * gcd(p,n), gcd(p,n+1), … ,
 *            gcd(p,n+5) = 1 * (i.e. p is coprime to the six consecutive
 *            integers starting at n)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Simple deterministic primality test for 64‑bit unsigned integers.
 * Works for the small primes we need (p starts at 7 and grows slowly). */
static bool is_prime_uint64(uint64_t x)
{
    if (x < 2)                 return false;
    if (x % 2 == 0)             return x == 2;
    if (x % 3 == 0)             return x == 3;

    for (uint64_t i = 5; i * i <= x; i += 6)
    {
        if (x % i == 0 || x % (i + 2) == 0)
            return false;
    }
    return true;
}

/*  OEIS sequence A053674: least number coprime to n, n+1, … , n+5. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset is 1; behaviour for n<1 is irrelevant */

    for (uint64_t p = 7; ; ++p)    /* search primes starting at 7 */
    {
        if (!is_prime_uint64(p))
            continue;

        uint64_t r = n % p;        /* n mod p */
        bool ok = true;
        for (uint64_t k = 0; k < 6; ++k)
        {
            if ((r + k) % p == 0)   /* some n+k is a multiple of p */
            {
                ok = false;
                break;
            }
        }
        if (ok)
            return (seq_t)p;       /* cast to the sequence element type */
    }
}
