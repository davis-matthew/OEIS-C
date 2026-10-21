/*
 * A077766  Number of primes of form 4k+1 between n^2 and (n+1)^2.
 * https://oeis.org/A077766
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of primes of form 4k+1 between n² and (n+1)²
 * Range:     u8 n <= 3893, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Deterministic Miller–Rabin primality test for 64‑bit values */
static unsigned long long mod_mul(unsigned long long a, unsigned long long b,
                                  unsigned long long mod) {
    return (unsigned __int128)a * b % mod;
}
static unsigned long long mod_pow(unsigned long long a, unsigned long long d,
                                  unsigned long long mod) {
    unsigned long long r = 1;
    while (d) {
        if (d & 1) r = mod_mul(r, a, mod);
        a = mod_mul(a, a, mod);
        d >>= 1;
    }
    return r;
}
static int is_prime_uint64(unsigned long long n) {
    if (n < 2) return 0;
    /* small primes */
    static const unsigned long long small[] =
        {2ULL,3ULL,5ULL,7ULL,11ULL,13ULL,17ULL,19ULL,23ULL,29ULL,31ULL,37ULL};
    for (unsigned i = 0; i < sizeof(small)/sizeof(small[0]); ++i) {
        unsigned long long p = small[i];
        if (n % p == 0) return n == p;
    }
    /* write n-1 = d * 2ⁿ */
    unsigned long long d = n - 1, s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        ++s;
    }
    /* deterministic bases for 64‑bit integers */
    static const unsigned long long bases[] =
        {2ULL, 325ULL, 9375ULL, 28178ULL, 450775ULL, 9780504ULL, 1795265022ULL};
    for (unsigned i = 0; i < sizeof(bases)/sizeof(bases[0]); ++i) {
        unsigned long long a = bases[i];
        if (a % n == 0) continue;
        unsigned long long x = mod_pow(a, d, n);
        if (x == 1 || x == n - 1) continue;
        int composite = 1;
        for (unsigned r = 1; r < s; ++r) {
            x = mod_mul(x, x, n);
            if (x == n - 1) {
                composite = 0;
                break;
            }
        }
        if (composite) return 0;
    }
    return 1;
}

/* Main sequence function */
seq_t a(idx_t n) {
    if (n < 1) return 0;
    unsigned long long nsq = (unsigned long long)n * n;
    unsigned long long nextsq = (unsigned long long)(n + 1) * (n + 1);
    unsigned long long start = nsq + 1;
    unsigned long long end   = nextsq - 1;
    seq_t cnt = 0;
    for (unsigned long long x = start; x <= end; ++x) {
        if ((x & 3ULL) == 1ULL && is_prime_uint64(x))
            ++cnt;
    }
    return cnt;
}
