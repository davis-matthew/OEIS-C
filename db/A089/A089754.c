/*
 * A089754  Smallest prime ending (least significant side) in n if possible
 *          else beginning in n.
 * https://oeis.org/A089754
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = * smallest prime that ends with the decimal
 *            representation of n * (suffix), if such a prime exists;
 *            otherwise the smallest prime * that begins with n (prefix). *
 *            This uses a deterministic Miller–Rabin test for 64‑bit integers
 * Range:     u8 n <= 19, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <limits.h>

/*-----------  Miller Rabin (deterministic for 64‑bit)  -----------*/
static inline uint64_t mulmod_u64(uint64_t a, uint64_t b, uint64_t mod) {
    return (uint64_t)((__uint128_t)a * b % mod);
}
static inline uint64_t powmod_u64(uint64_t a, uint64_t e, uint64_t mod) {
    uint64_t r = 1;
    while (e) {
        if (e & 1) r = mulmod_u64(r, a, mod);
        a = mulmod_u64(a, a, mod);
        e >>= 1;
    }
    return r;
}
static inline int isPrime_u64(uint64_t n) {
    if (n < 2) return 0;
    static const uint64_t small[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    for (int i = 0; i < (int)(sizeof small / sizeof *small); ++i) {
        if (n % small[i] == 0) return n == small[i];
    }
    uint64_t d = n - 1, s = 0;
    while ((d & 1) == 0) { d >>= 1; ++s; }
    static const uint64_t bases[] = {2ULL, 325ULL, 9375ULL, 28178ULL,
                                     450775ULL, 9780504ULL, 1795265022ULL};
    for (int i = 0; i < (int)(sizeof bases / sizeof *bases); ++i) {
        uint64_t a = bases[i] % n;
        if (a == 0) continue;
        uint64_t x = powmod_u64(a, d, n);
        if (x == 1 || x == n - 1) continue;
        int j;
        for (j = 1; j < (int)s; ++j) {
            x = mulmod_u64(x, x, n);
            if (x == n - 1) break;
        }
        if (j == s) return 0;
    }
    return 1;
}

/*-----------  Auxiliary utilities  -----------*/
static const uint64_t POW10[20] = {
    1ULL, 10ULL, 100ULL, 1000ULL, 10000ULL, 100000ULL,
    1000000ULL, 10000000ULL, 100000000ULL, 1000000000ULL,
    10000000000ULL, 100000000000ULL, 1000000000000ULL,
    10000000000000ULL, 100000000000000ULL, 1000000000000000ULL,
    10000000000000000ULL, 100000000000000000ULL,
    1000000000000000000ULL, 10000000000000000000ULL
};

static inline uint64_t pow10_u64(int exp) {
    return POW10[exp];
}

static inline int num_digits_uint64(uint64_t x) {
    int d = 0;
    do { ++d; x /= 10; } while (x);
    return d;
}

/*-----------  Sequence implementation  -----------*/
seq_t a(idx_t n) {
    uint64_t nn = (uint64_t)n;

    /* 1. n itself is prime => suffix prime */
    if (isPrime_u64(nn))
        return (seq_t)nn;

    /* 2. Try suffix prime (only possible if last digit not 0,2,4,5,6,8) */
    uint8_t last = nn % 10;
    if (last != 0 && last != 2 && last != 4 && last != 5 &&
        last != 6 && last != 8) {
        int d = num_digits_uint64(nn);
        uint64_t step = pow10_u64(d);
        uint64_t cand = nn + step;        /* skip composite n itself */
        while (cand > nn) {
            if (isPrime_u64(cand))
                return (seq_t)cand;
            if (cand > ULLONG_MAX - step) break; /* overflow safe stop */
            cand += step;
        }
    }

    /* 3. Search smallest prime that begins with n */
    for (int k = 1; k < 20; ++k) {         /* k = number of extra digits */
        uint64_t powk = pow10_u64(k);
        if (nn > (ULLONG_MAX / powk)) break;  /* overflow guard */

        uint64_t base = nn * powk;           /* n followed by k zeros */
        if (base < nn) break;                /* overflow guard */

        /* iterate over all suffixes of length k */
        for (uint64_t s = 0; s < powk; ++s) {
            uint64_t cand = base + s;
            if (isPrime_u64(cand))
                return (seq_t)cand;
            /* cand grows monotonically; overflow impossible within loop */
        }
    }

    /* Should not happen for n >= 1 by OEIS definition */
    return (seq_t)0;
}
