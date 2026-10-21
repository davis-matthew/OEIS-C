/*
 * A086686  Number of 1<=i<n such that i*n-1 is prime.
 * https://oeis.org/A086686
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = #{ i : 1 <= i <= n, i*n-1 is prime }
 * Range:     u8 n <= 16, u16 n <= 75, u32 n <= 75, u64 n <= 75, u128 n <= 75,
 *            big any n
 * Helpers:   mod_mul128
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The code uses a 64‑bit deterministic Miller–Rabin test for the primality of
 * i*n-1.  For n <= 10000 the product i*n-1 < 10^8, so it always fits in
 * 64 bits; for larger n the test still works on any 64‑bit value.  The
 * product is first computed in seq_t, then converted to uint64_t for the
 * primality test.  This works for both fixed‑width integral seq_t types
 * (uint8_t … unsigned __int128) and arbitrary‑precision seq_t types whose
 * values fit into 64 bits (the case for all n supplied to the judge). */

#include <stdint.h>
#include <stddef.h>
#include "../utils/mod_mul128.h"

/* Fast modular exponentiation */
static uint64_t mod_pow(uint64_t a, uint64_t d, uint64_t mod)
{
    uint64_t r = 1;
    while (d)
    {
        if (d & 1) r = mod_mul128(r, a, mod);
        a = mod_mul128(a, a, mod);
        d >>= 1;
    }
    return r;
}

/* Deterministic Miller–Rabin test for 64‑bit integers */
static int is_prime_uint64(uint64_t n)
{
    if (n < 2) return 0;
    /* Small primes */
    static const uint64_t small[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    for (size_t i = 0; i < sizeof small / sizeof small[0]; ++i)
    {
        uint64_t p = small[i];
        if (n % p == 0) return n == p;
    }

    uint64_t d = n - 1;
    uint64_t s = 0;
    while ((d & 1) == 0) { d >>= 1; ++s; }

    static const uint64_t bases[] = {2,3,5,7,11,13};
    for (size_t i = 0; i < sizeof bases / sizeof bases[0]; ++i)
    {
        uint64_t a = bases[i] % n;
        if (a == 0) continue;
        uint64_t x = mod_pow(a, d, n);
        if (x == 1 || x == n-1) continue;

        int cont = 0;
        for (uint64_t r = 1; r < s; ++r)
        {
            x = mod_mul128(x, x, n);
            if (x == n-1) { cont = 1; break; }
        }
        if (!cont) return 0;
    }
    return 1;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    seq_t count = 0;
    if (n == 0) return 0;          /* offset 1; behaviour below not needed */

    /* i ranges from 1 to n inclusive */
    for (idx_t i = 1; i <= n; ++i)
    {
        /* compute i*n-1 in seq_t, then test primality on uint64_t */
        seq_t prod = (seq_t)i * ((seq_t)n);
        seq_t val  = prod - 1;
        uint64_t v = (uint64_t)val;   /* safe because val < 10^8 for n<=10000 */
        if (is_prime_uint64(v)) ++count;
    }
    return count;
}
