/*
 * A145376  If T(n)>=n is the nearest triangular number to n and p_n is the
 *          n-th prime, then a(n) is the least prime such that
 *          T(a(n)*p_n)-a(n)p_n is a triangular number.
 * https://oeis.org/A145376
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least prime q such that T(q·p_n) – q·p_n is a triangular
 *            number, where T(m) is the smallest triangular number ≥ m and p_n
 *            is the n‑th prime (p_1 = 2)
 * Range:     u8 n <= 54, u16 n <= 6555, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* ------------------------------------------------------------
 *  Auxiliary integer functions
 * ------------------------------------------------------------ */
static inline uint64_t isqrt64(uint64_t x)
{
    uint64_t r = 0, bit = 1ULL << 62;
    while (bit > x) bit >>= 2;
    while (bit) {
        if (x >= r + bit) {
            x -= r + bit;
            r = (r >> 1) + bit;
        } else {
            r >>= 1;
        }
        bit >>= 2;
    }
    return r;
}

static inline bool is_triangular_uint64(uint64_t v)
{
    uint64_t t = isqrt64(8ULL * v + 1ULL);
    return (t * t == 8ULL * v + 1ULL) && ((t & 1U) == 1U);
}

/* ------------------------------------------------------------
 *  Prime sieve (once per program)
 * ------------------------------------------------------------ */
static uint32_t *primes = NULL; /* pointer to array of primes (uint32_t) */
static size_t    primes_cnt = 0; /* number of primes stored */

/* Simple sieve up to a numeric limit (must be >= the largest prime we need). */
static void init_primes(void)
{
    if (primes_cnt > 0) return;          /* already done */

    const uint32_t limit = 3000000U;      /* covers several hundred thousand primes */
    bool *sift = (bool *)calloc((size_t)limit + 1, sizeof(bool));
    if (!sift) { return; }              /* allocation failed: leave primes empty */

    size_t cap = (size_t)limit / 10;     /* rough upper bound for number of primes */
    primes = (uint32_t *)malloc(cap * sizeof(uint32_t));
    if (!primes) { free(sift); return; }

    for (uint32_t i = 2; i <= limit; ++i) {
        if (!sift[i]) {                 /* i is prime */
            if (primes_cnt == cap) {    /* grow array */
                cap <<= 1;
                uint32_t *tmp = (uint32_t *)realloc(primes, cap * sizeof(uint32_t));
                if (!tmp) break;
                primes = tmp;
            }
            primes[primes_cnt++] = i;
            if ((uint64_t)i * i <= limit) {
                for (uint64_t j = (uint64_t)i * i; j <= limit; j += i) {
                    sift[(size_t)j] = true;
                }
            }
        }
    }
    free(sift);
}

/* ------------------------------------------------------------
 *  Main sequence function
 * ------------------------------------------------------------ */
seq_t a(idx_t n)
{
    /* Ensure the prime list is ready. */
    init_primes();

    if (n == 0 || primes_cnt == 0) {
        return (seq_t)0; /* invalid request; sequence is 1‑based */
    }

    seq_t p_n = (seq_t)primes[(size_t)n - 1U]; /* n‑th prime, 1‑based */

    /* Iterate over all primes as candidate q. */
    for (size_t idx = 0; idx < primes_cnt; ++idx) {
        seq_t q = (seq_t)primes[idx];

        /* Compute x = q * p_n using 64‑bit to avoid overflow. */
        uint64_t x = (uint64_t)q * (uint64_t)p_n;

        /* Find the nearest triangular number >= x. */
        uint64_t sq = isqrt64(8ULL * x + 1ULL);
        uint64_t k  = (sq - 1ULL) / 2ULL;
        uint64_t Tk = k * (k + 1ULL) / 2ULL;
        if (Tk < x) {
            ++k;
            Tk = k * (k + 1ULL) / 2ULL;
        }
        uint64_t diff = Tk - x;

        /* Check whether the difference is triangular. */
        if (is_triangular_uint64(diff)) {
            return q; /* first such prime is the desired value */
        }
    }

    /* If we get here, the sieve limit was insufficient.  This should not
       happen for the intended range; return 0 as a sentinel. */
    return (seq_t)0;
}
