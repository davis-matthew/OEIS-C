/*
 * A180096  a(n) is the smallest number k such that four consecutive prime
 *          numbers prime(n), prime(n+1), prime(n+2) and prime(n+3) are
 *          divisors of k, k+1, k+2 and k+3 respectively.
 * https://oeis.org/A180096
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the smallest k such that k is divisible by prime(n), k+1
 *            is divisible by prime(n+1), k+2 is divisible by prime(n+2), k+3
 *            is divisible by prime(n+3). Computation via the Chinese
 *            Remainder Theorem on four consecutive primes
 * Range:     u8 n <= 1, u16 n <= 8, u32 n <= 58, u64 n <= 6581,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* Helpers for extended Euclidean algorithm – returns inverse of a modulo m
   (m is guaranteed to be prime and thus invertible). */
static uint64_t modinv_uint64(uint64_t a, uint64_t m)
{
    int64_t t0 = 0, t1 = 1;
    int64_t r0 = m, r1 = a;
    while (r1 != 0) {
        uint64_t q = r0 / r1;
        int64_t r_tmp = r0 - (int64_t)q * r1;
        r0 = r1; r1 = r_tmp;
        int64_t t_tmp = t0 - (int64_t)q * t1;
        t0 = t1; t1 = t_tmp;
    }
    if (r0 != 1) return 0;              /* inverse does not exist */
    if (t0 < 0) t0 += m;
    return (uint64_t)t0;
}

/* Static storage for primes.  The sieve runs once on the first call. */
static uint64_t *primes = NULL;
static size_t primes_cnt = 0;

/* Build a sieve up to LIMIT and fill the primes array. */
static void init_primes(void)
{
    /* Sufficient for n ≤ 10000, using the known 10004‑th prime ≈ 104743. */
    const idx_t LIMIT = 120000;   /* a small safety margin */
    uint8_t *is_comp = (uint8_t *)malloc((LIMIT + 1) * sizeof(uint8_t));
    if (!is_comp) return; /* out of memory – will crash downstream */
    is_comp[0] = is_comp[1] = 1;
    for (idx_t i = 2; i * i <= LIMIT; ++i)
        if (!is_comp[i])
            for (idx_t j = i * i; j <= LIMIT; j += i)
                is_comp[j] = 1;

    /* Count primes. */
    primes_cnt = 0;
    for (idx_t i = 2; i <= LIMIT; ++i)
        if (!is_comp[i]) ++primes_cnt;

    primes = (uint64_t *)malloc(primes_cnt * sizeof(uint64_t));
    idx_t pos = 0;
    for (idx_t i = 2; i <= LIMIT; ++i)
        if (!is_comp[i]) primes[pos++] = i;
    free(is_comp);
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (!primes) init_primes();          /* lazy initialization */

    /* Indexing: primes[0] is prime(1). */
    idx_t idx = n - 1;
    uint64_t p0 = primes[idx];
    uint64_t p1 = primes[idx + 1];
    uint64_t p2 = primes[idx + 2];
    uint64_t p3 = primes[idx + 3];

    /* Remainders for the congruences: k ≡ 0 (mod p0), k ≡ p1-1 (mod p1), … */
    uint64_t r1 = p1 - 1;
    uint64_t r2 = p2 - 2;
    uint64_t r3 = p3 - 3;

    seq_t x   = 0;          /* current solution */
    seq_t mprod = p0;       /* product of processed moduli */

    /* Combine with moduli in order p1, p2, p3. */
    struct { uint64_t m; uint64_t r; } step[] = {
        {p1, r1},
        {p2, r2},
        {p3, r3}
    };

    for (int i = 0; i < 3; ++i) {
        uint64_t mi = step[i].m;
        uint64_t ri = step[i].r;

        uint64_t xm = (uint64_t)(x % mi);
        uint64_t diff = (ri + mi - xm) % mi;          /* positive difference */
        uint64_t inv  = modinv_uint64((uint64_t)(mprod % mi), mi);
        uint64_t t    = (diff * inv) % mi;

        x = x + (seq_t)(mprod * t);
        mprod = mprod * mi;
    }

    /* x lies in [0, mprod-1]; it is the required minimum k. */
    return x;
}
