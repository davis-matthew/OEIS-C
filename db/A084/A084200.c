/*
 * A084200  LeastCommonMultiple{q+1: q prime, q < prime(n), q+1 divides
 *          prime(n)+1}.
 * https://oeis.org/A084200
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = lcm{ q+1 : q prime, q < prime(n), q+1 | prime(n)+1 }
 * Range:     u8 n <= 54, u16 n <= 6542, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  The implementation builds a small sieve once, then for each
    requested n factors prime(n)+1, scans all its divisors d>1,
    checks whether d-1 is a prime less than prime(n) and, if so,
    includes d in the running least‑common multiple.   */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>   /* for size_t */

#define MAX_PRIME_LIMIT 200000                /* covers prime(10000) ≈ 104729 */
#define MAX_PRIMES      20000                  /* enough for the limit above */

static bool        sieve_inited   = false;
static bool        prime_flag[MAX_PRIME_LIMIT + 1];
static uint32_t    primes[MAX_PRIMES];
static size_t      prime_count     = 0;

/* ---------- sieve construction ---------- */
static void init_sieve(void)
{
    if (sieve_inited)
        return;
    sieve_inited = true;

    for (int i = 0; i <= MAX_PRIME_LIMIT; ++i)
        prime_flag[i] = true;
    prime_flag[0] = prime_flag[1] = false;
    for (int i = 2; i * i <= MAX_PRIME_LIMIT; ++i)
        if (prime_flag[i])
            for (int j = i * i; j <= MAX_PRIME_LIMIT; j += i)
                prime_flag[j] = false;

    for (int i = 2; i <= MAX_PRIME_LIMIT && prime_count < MAX_PRIMES; ++i)
        if (prime_flag[i])
            primes[prime_count++] = (uint32_t)i;
}

/* ---------- least common multiple for seq_t ---------- */
static inline seq_t gcd_seq(seq_t a, seq_t b)
{
    while (b) {
        seq_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

static inline seq_t lcm_seq(seq_t a, seq_t b)
{
    if (a == 0 || b == 0)
        return 0;
    return (a / gcd_seq(a, b)) * b;
}

/* ---------- main sequence routine ---------- */
seq_t a(idx_t n)
{
    if (n == 0)          /* the sequence is defined for n ≥ 1 */
        return 0;

    init_sieve();

    /* obtain prime(n) */
    if (n > prime_count)
        return 0;        /* safety guard – should not happen with current limits */

    uint32_t pn = primes[n - 1];      /* prime(n) as an unsigned 32‑bit value */
    seq_t   pn1 = (seq_t)pn + 1;      /* prime(n)+1 */

    /* factorisation of pn1 */
    seq_t tmp = pn1;
    struct {
        seq_t factor;
        int   power;
    } facts[10];
    size_t fcnt = 0;

    for (size_t i = 0; i < prime_count && (seq_t)primes[i] * primes[i] <= tmp; ++i) {
        uint32_t pr = primes[i];
        if (tmp % (seq_t)pr == 0) {
            seq_t fp = (seq_t)pr;
            int   e  = 0;
            while (tmp % fp == 0) {
                tmp /= fp;
                ++e;
            }
            facts[fcnt].factor = fp;
            facts[fcnt].power  = e;
            ++fcnt;
        }
    }
    if (tmp > 1) {                /* remaining prime factor */
        facts[fcnt].factor = tmp;
        facts[fcnt].power  = 1;
        ++fcnt;
    }

    /* generate all divisors of pn1 (maximum < 200000 has < 128 divisors) */
    seq_t divisors[256];
    size_t dcnt = 1;
    divisors[0] = 1;
    for (size_t i = 0; i < fcnt; ++i) {
        seq_t base = facts[i].factor;
        int   exp  = facts[i].power;
        size_t old = dcnt;
        seq_t pow = 1;
        for (int k = 1; k <= exp; ++k) {
            pow *= base;                    /* base^k */
            for (size_t j = 0; j < old; ++j)
                divisors[dcnt++] = divisors[j] * pow;
        }
    }

    /* compute LCM of applicable divisors */
    seq_t lcm_val = 1;
    for (size_t i = 0; i < dcnt; ++i) {
        seq_t d = divisors[i];
        if (d <= 1)
            continue;
        seq_t q = d - 1;
        if (q < pn &&
            q <= MAX_PRIME_LIMIT &&
            prime_flag[(int)q])
            lcm_val = lcm_seq(lcm_val, d);
    }
    return lcm_val;
}
