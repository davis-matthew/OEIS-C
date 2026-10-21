/*
 * A098839  Smith square numbers.
 * https://oeis.org/A098839
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 10, u32 n <= 1208, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Sequence A098839: Smith square numbers – the squares that are Smith numbers.
 * Algorithm: sequentially test integers m = 2,3,…; each time compute
 *   sq   = m²                      (as seq_t)
 *   sum_sq   = digit_sum(sq)
 *   sum_fac  = 2 * (sum over prime factors p^e of m of e * digit_sum(p))
 * If sum_sq == sum_fac, store sq as the next term.
 *
 * A static cache keeps all terms that have been found so that arbitrary
 * order calls of a(n) are O(1) after the terms up to n have been generated.
 *
 * The code uses only integer arithmetic and no external library functions.
 * For arbitrary‑precision seq_t types any provided implementation of
 * multiplication and mod is used.  The algorithm works for 8, 16, 32, 64
 * as well as arbitrary‑precision builds.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* ---- digit sums ----------------------------------------------------- */

/* digit sum of a 32‑bit integer */
static unsigned int digit_sum_uint32(uint32_t x)
{
    unsigned int s = 0;
    while (x > 0U) {
        s += (unsigned int)(x % 10U);
        x /= 10U;
    }
    return s;
}

/* digit sum of any seq_t value */
static unsigned int digit_sum_seq_t(seq_t x)
{
    unsigned int s = 0;
    while (x > 0) {
        s += (unsigned int)(x % 10);
        x /= 10;
    }
    return s;
}

/* ---- integer square root for uint64_t -------------------------------- */

static uint32_t isqrt_u64(uint64_t v)
{
    /* binary search on [0, 2^32-1] using 128‑bit multiplication to avoid overflow */
    uint64_t lo = 0, hi = (uint64_t)0xFFFFFFFFU;
    while (lo <= hi) {
        uint64_t mid = (lo + hi) >> 1;
        unsigned __int128 sq = (unsigned __int128)mid * (unsigned __int128)mid;
        if (sq == v) return (uint32_t)mid;
        if (sq < v) lo = mid + 1;
        else        hi = mid - 1;
    }
    return (uint32_t)hi;
}

/* ---- prime list generation ------------------------------------------ */

static uint32_t *prime_list   = NULL;
static size_t   prime_count   = 0;
static size_t   prime_cap     = 0;
static uint32_t prime_limit   = 2;   /* primes up to this value are stored */

/* ensure that primes up to `limit` (inclusive) are generated */
static void ensure_primes(uint32_t limit)
{
    if (limit <= prime_limit) return;

    /* start generating from the next integer after current limit */
    for (uint32_t n = prime_limit + 1; n <= limit; ++n) {
        if (n < 2) continue;
        if (n > 2 && (n & 1) == 0) continue;   /* skip even numbers */

        bool isprime = true;
        for (size_t i = 0; i < prime_count; ++i) {
            uint32_t p = prime_list[i];
            if ((uint64_t)p * (uint64_t)p > n) break;
            if (n % p == 0) { isprime = false; break; }
        }
        if (isprime) {
            if (prime_count == prime_cap) {
                prime_cap = prime_cap ? prime_cap * 2 : 8;
                prime_list = (uint32_t *)realloc(prime_list, prime_cap * sizeof(uint32_t));
            }
            prime_list[prime_count++] = n;
        }
    }
    prime_limit = limit;
}

/* ---- factor digit sum ------------------------------------------------ */

static unsigned int factor_digit_sum(uint64_t m)
{
    unsigned int sum = 0;
    uint64_t temp = m;

    /* ensure primes up to sqrt(m) are available */
    uint32_t limit = isqrt_u64(temp);
    ensure_primes(limit + 1);

    for (size_t i = 0; i < prime_count; ++i) {
        uint32_t p = prime_list[i];
        if ((uint64_t)p * (uint64_t)p > temp) break;
        if (temp % p == 0) {
            unsigned int cnt = 0;
            while (temp % p == 0) {
                temp /= p;
                ++cnt;
            }
            sum += digit_sum_uint32(p) * cnt;
        }
    }
    if (temp > 1) {          /* remaining prime factor */
        sum += digit_sum_uint32((uint32_t)temp);
    }
    return sum;
}

/* ---- cache for computed terms ---------------------------------------- */

static struct {
    seq_t  *terms;          /* 1‑based array of terms */
    size_t terms_len;       /* next free index (first free is 1) */
    size_t terms_cap;       /* allocated capacity */
    uint64_t next_m;        /* next integer m to test */
} gw = { NULL, 1, 0, 2 };   /* terms_len starts at 1 (index 0 unused) */

/* ensure the cache can hold `needed` entries (1‑based) */
static void ensure_terms_capacity(size_t needed)
{
    if (gw.terms_cap >= needed) return;
    size_t new_cap = gw.terms_cap ? gw.terms_cap * 2 : 8;
    while (new_cap < needed) new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(gw.terms, new_cap * sizeof(seq_t));
    gw.terms     = tmp;
    gw.terms_cap = new_cap;
}

/* compute the next Smith square (if any) and store it if found */
static void compute_next(void)
{
    uint64_t m = gw.next_m++;

    /* square value in seq_t */
    seq_t sq = (seq_t)m * (seq_t)m;

    unsigned int sum_sq    = digit_sum_seq_t(sq);
    unsigned int sum_facs  = factor_digit_sum(m) * 2U;   /* doubled for m² */

    if (sum_sq == sum_facs) {
        if (gw.terms_len + 1 > gw.terms_cap) {
            ensure_terms_capacity(gw.terms_len + 2);
        }
        gw.terms[gw.terms_len++] = sq;
    }
}

/* ---- main sequence function ------------------------------------------ */

seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* offset is 1; undefined for 0 */

    /* prefill prime list with 2 */
    if (prime_list == NULL) {
        prime_cap  = 8;
        prime_list = (uint32_t *)malloc(prime_cap * sizeof(uint32_t));
        prime_list[0] = 2;
        prime_count   = 1;
        prime_limit   = 2;
    }

    /* ensure cache large enough for n */
    ensure_terms_capacity(n + 1);

    /* generate terms until we have at least n terms stored */
    while (gw.terms_len <= n) {
        compute_next();
    }

    return gw.terms[n];
}
