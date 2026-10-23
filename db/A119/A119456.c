/*
 * A119456  Numbers m such that the Bernoulli number B_{10*m} has denominator
 *          66.
 * https://oeis.org/A119456
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 26, u16 n <= 6825, u32 n <= 8894, u64 n <= 8894,
 *            u128 n <= 8894, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the OEIS sequence A119456.
   A(n) is the nth integer m such that the Bernoulli number B_{10m}
   has denominator 66.  The terms satisfy
   a(n) = A051230(n)/10 = A051229(n)/5.
   The implementation generates terms in order by testing each integer
   m for the required property and memoises the results.  The test
   works by checking that no divisor d of (10·m) has d+1 a prime ≥13.
   Only trial division using a small prime table (up to 5 000 000)
   is needed, which is fast enough for the expected range of n.

   The code is valid C11 and C++17.  No external utilities are used.
   All arithmetic stays within 64 bit unsigned integers, so it works
   for all seq_t types that can hold the produced values.  */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static const uint32_t PRIME_LIMIT = 5000000;  /* primes up to this limit are used */

/* ---------- small prime sieve ---------- */
static uint64_t *global_primes = NULL;
static size_t global_prime_cnt  = 0;

static void init_prime_list(void)
{
    if (global_primes != NULL) return;
    size_t n = PRIME_LIMIT + 1;
    bool *is_composite = (bool *)malloc(n);
    if (!is_composite) return;       /* should not happen */
    for (size_t i = 0; i < n; ++i) is_composite[i] = false;

    for (uint32_t i = 2; (uint64_t)i * i <= PRIME_LIMIT; ++i)
        if (!is_composite[i])
            for (uint32_t j = i * i; j <= PRIME_LIMIT; j += i)
                is_composite[j] = true;

    /* first count primes */
    size_t cnt = 0;
    for (uint32_t i = 2; i <= PRIME_LIMIT; ++i)
        if (!is_composite[i]) ++cnt;

    global_primes = (uint64_t *)malloc(cnt * sizeof(uint64_t));
    if (!global_primes) { free(is_composite); return; }

    size_t idx = 0;
    for (uint32_t i = 2; i <= PRIME_LIMIT; ++i)
        if (!is_composite[i]) global_primes[idx++] = i;

    global_prime_cnt = cnt;
    free(is_composite);
}

/* ---------- primality test (trial division) ---------- */
static bool is_prime_u64(uint64_t n)
{
    if (n < 2) return false;
    if (n <= 3) return true;
    if ((n & 1ULL) == 0) return false;
    init_prime_list();
    for (size_t i = 0; i < global_prime_cnt; ++i) {
        uint64_t p = global_primes[i];
        if (p * p > n) break;
        if (n % p == 0) return false;
    }
    return true;
}

/* ---------- factorisation of a positive integer n ----------
   Factors are stored in arrays `fac` and `exp`.  The caller must ensure
   the arrays are large enough (at least 20 elements).  The function
   returns the number of distinct prime factors via `cnt`.  */
static void factorise(uint64_t n, uint64_t fac[20], uint8_t exp[20], size_t *cnt)
{
    *cnt = 0;
    if (n <= 1) return;
    init_prime_list();
    for (size_t i = 0; i < global_prime_cnt && (uint64_t)global_primes[i] * global_primes[i] <= n; ++i) {
        uint64_t p = global_primes[i];
        if (n % p == 0) {
            uint8_t e = 0;
            while (n % p == 0) { n /= p; ++e; }
            fac[*cnt] = p;
            exp[*cnt] = e;
            ++(*cnt);
        }
    }
    if (n > 1) {
        fac[*cnt] = n;
        exp[*cnt] = 1;
        ++(*cnt);
    }
}

/* ---------- validate a candidate m ----------
   Returns true if no divisor d of (10⋅m) satisfies d+1 is a prime ≥13.  */
static bool valid_m(uint64_t m)
{
    if ((m & 1ULL) == 0) {/* m must be odd (because 2 already in 10) */
        /* If m is even, 2 appears twice in 10⋅m, leading to divisor 2¹⁰… which would
           allow p=3; the denominator then contains 3 already, so an even m is allowed.
           But the OEIS definition uses odd m (see comments).  We preserve that. */
        return false;
    }

    /* Factor m */
    uint64_t fac[20];
    uint8_t exp[20];
    size_t cnt;
    factorise(m, fac, exp, &cnt);

    /* Add factors for 2 and 5 coming from 10.  Increase their exponents if already present. */
    bool has2 = false, has5 = false;
    for (size_t i = 0; i < cnt; ++i) {
        if (fac[i] == 2) { exp[i]++; has2 = true; }
        if (fac[i] == 5) { exp[i]++; has5 = true; }
    }
    if (!has2) {
        fac[cnt] = 2; exp[cnt] = 1; ++cnt;
    }
    if (!has5) {
        fac[cnt] = 5; exp[cnt] = 1; ++cnt;
    }

    /* Recursive enumeration of all divisors of 10⋅m.  If any divisor d>1 satisfies
       d+1 is prime ≥13, m is invalid.  */
    bool invalid = false;
    /* stack-based recursion to avoid function call overhead */
    struct StackNode { size_t idx; uint64_t cur; };
    struct StackNode stack[256];
    size_t sp = 0;
    stack[sp++] = (struct StackNode){0, 1};

    while (sp > 0 && !invalid) {
        struct StackNode node = stack[--sp];
        size_t idx = node.idx;
        uint64_t cur = node.cur;
        if (idx == cnt) {
            if (cur > 1) {
                uint64_t pd = cur + 1;
                if (pd > 11 && is_prime_u64(pd)) { invalid = true; break; }
            }
            continue;
        }
        /* iterate exponent of fac[idx] from 0 to exp[idx] */
        uint64_t mul = 1;
        for (uint8_t e = 0; e <= exp[idx]; ++e) {
            /* push next state */
            stack[sp++] = (struct StackNode){idx + 1, cur * mul};
            mul *= fac[idx];
        }
    }
    return !invalid;
}

/* ---------- sequence storage ---------- */
static seq_t *cache  = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;
static uint64_t last_checked = 0;

/* ---------- public function ---------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* safety, offset starts at 1 */

    init_prime_list();

    if (cache == NULL) {
        cache_cap = 64;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return 0;  /* allocation failure */
        cache_len = 0;
        last_checked = 0;
    }

    /* Generate terms until we have enough. */
    while (cache_len < (size_t)n) {
        ++last_checked;
        if (valid_m(last_checked)) {
            if (cache_len == cache_cap) {
                cache_cap <<= 1;
                cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
                if (!cache) return 0;
            }
            cache[cache_len++] = (seq_t)last_checked;
        }
    }

    return cache[n - 1];
}
