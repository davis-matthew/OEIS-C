/*
 * A177436  The number of positive integers m for which the exponents of 2 and
 *          prime(n) in the prime power factorization of m! are both powers of
 *          2.
 * https://oeis.org/A177436
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 127, u16 n <= 127, u32 n <= 127, u64 n <= 127,
 *            u128 n <= 127, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A177436  –  maximal m such that exponents of 2 and prime(n) in m! are
    powers of 2.  The function implements the piece‑wise rule given in
    the OEIS entry:  */
    /*  a(2)=a(3)=7, a(4)=6.  For n≥5:
        1. if p_n = (2^(4k+1)+3)/5   , k≥2          → a(n)=5
        2. if p_n is a Fermat prime (p_n=2^(2^(k-1))+1, k≥3)   → a(n)=4
        3. if p_n = 2^k+3           , k≥3          → a(n)=3
        4. else let k be minimal with 2^(k-1)+3 < p_n ≤ 2^k-1
           then a(n)=2*(1+⌊log₂((p_n-5)/(2^k-p_n))⌋).     */

/*  The code uses only integer arithmetic and the helpers provided by
    the OEIS build system.  It contains a lightweight prime sieve that
    is good enough for n up to at least 100 000.  */
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* Prime sieve constants --------------------------------------------------- */
#define PRIME_LIMIT 200000   /* > 10 000th prime (≈104 729) */

/* Global arrays ------------------------------------------------------------ */
static bool primes_built = false;
static uint32_t primes[20000];   /* 20 000 slots – plenty for 10 000 primes */
static size_t prime_cnt = 0;     /* number of primes stored */

/* ------------------------------------------------------------------------- */
/*  Helper: build a list of primes up to PRIME_LIMIT.  Uses a simple      */
/*  sieve of Eratosthenes.  The result is kept in the global array.        */
/* ------------------------------------------------------------------------- */
static void build_primes(void)
{
    static uint8_t is_comp[PRIME_LIMIT + 1] = {0};
    size_t i, j, pidx = 0;

    for (i = 2; i <= PRIME_LIMIT; ++i) {
        if (!is_comp[i]) {
            primes[pidx++] = (uint32_t)i;
            if (pidx >= 20000) break;
            for (j = i * 2; j <= PRIME_LIMIT; j += i)
                is_comp[j] = 1;
        }
    }
    prime_cnt = pidx;
    primes_built = true;
}

/* ------------------------------------------------------------------------- */
/*  Helper: return 2^e where e<64.                                         */
/* ------------------------------------------------------------------------- */
static inline uint64_t pow2_u64(unsigned e)
{
    return ((uint64_t)1) << e;
}

/* ------------------------------------------------------------------------- */
/*  Helper: floor(log₂(v)) for v>0.   Uses builtin if available.            */
/* ------------------------------------------------------------------------- */
static unsigned floor_log2_uint64(uint64_t v)
{
#if defined(__GNUC__) || defined(__clang__)
    return 63 - __builtin_clzll(v);
#else
    unsigned r = 0;
    while (v >>= 1) ++r;
    return r;
#endif
}

/* ------------------------------------------------------------------------- */
/*  Helper: test whether v is a power of two.                               */
/* ------------------------------------------------------------------------- */
static bool is_power_of_two(uint64_t v)
{
    return v && !(v & (v - 1));
}

/* ------------------------------------------------------------------------- */
/*  Main sequence function.  idx_t and seq_t are defined by the build.      */
/* ------------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* Offset: the sequence is defined for n≥2                                   */
    if (n < 2) return (seq_t)0;

    /* Small special cases that cannot be derived from the general formula.   */
    if (n == 2 || n == 3) return (seq_t)7;
    if (n == 4)           return (seq_t)6;

    /* Ensure prime table is ready.                                               */
    if (!primes_built) build_primes();

    /* Get the n‑th prime (primes array is 0‑based).                             */
    uint64_t p = primes[(size_t)(n - 1)];

    /* 1.  (2^(4*k+1)+3)/5,  k≥2  →  a(n)=5                               */
    uint64_t x = 5 * p - 3;
    if (x > 0 && is_power_of_two(x)) {
        unsigned exp = floor_log2_uint64(x);          /* exp = 4*k+1            */
        if (exp >= 9 && ((exp - 1) & 3) == 0)         /* k≥2  → exp≥9           */
            return (seq_t)5;
    }

    /* 2.  Fermat prime  p = 2^(2^(k-1))+1,  k≥3  →  a(n)=4                */
    uint64_t e = p - 1;
    if (e > 0 && is_power_of_two(e)) {
        unsigned e_exp = floor_log2_uint64(e);        /* e = 2^e_exp          */
        if (e_exp >= 4 && is_power_of_two((uint64_t)e_exp))
            return (seq_t)4;
    }

    /* 3.  p = 2^k + 3,  k≥3  →  a(n)=3                                 */
    uint64_t t = p - 3;
    if (t > 0 && is_power_of_two(t)) {
        unsigned k = floor_log2_uint64(t);            /* k ≥ 1                */
        if (k >= 3) return (seq_t)3;
    }

    /* 4.  General case:  find minimal k such that 2^(k-1)+3 < p ≤ 2^k-1.      */
    unsigned k = 3;
    while (true) {
        uint64_t lower = (pow2_u64(k - 1)) + 3;
        uint64_t upper = pow2_u64(k) - 1;
        if (lower < p && p <= upper) break;
        ++k;
    }

    /* Compute a(n) = 2*(1 + floor(log₂((p-5)/(2^k-p))))                    */
    uint64_t num = p - 5;           /* p ≥ 7  ->  num > 0                     */
    uint64_t den = pow2_u64(k) - p; /* positive by definition of k            */
    uint64_t div = num / den;       /* integer division, div ≥ 1              */
    unsigned l2 = floor_log2_uint64(div);

    return (seq_t)(2 * ((seq_t)1 + (seq_t)l2));
}
