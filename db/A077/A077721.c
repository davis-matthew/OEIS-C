/*
 * A077721  Primes which can be expressed as sum of distinct powers of 7.
 * https://oeis.org/A077721
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 158, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   mul_mod64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = the n‑th prime whose base‑7 representation contains
 * only the digits 0 and 1 (OEIS A077721).
 * The code generates base‑7 numbers with digits 0/1, tests them for
 * primality using deterministic Miller–Rabin for 64‑bit integers,
 * stores the primes, and returns the requested element.
 * The routine works for seq_t = 64‑bit integer or arbitrary‑precision
 * integer class because the values are converted from uint64_t.
 */
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mul_mod64.h"

/* ----- Miller–Rabin for 64‑bit integers --------------------------------- */

static uint64_t pow_mod64(uint64_t a, uint64_t exp, uint64_t mod)
{
    uint64_t res = 1;
    while (exp) {
        if (exp & 1) res = mul_mod64(res, a, mod);
        a = mul_mod64(a, a, mod);
        exp >>= 1;
    }
    return res;
}

static bool is_prime_u64(uint64_t n)
{
    if (n < 2) return false;
    static const uint64_t small[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    for (size_t i = 0; i < sizeof small / sizeof *small; ++i) {
        if (n % small[i] == 0) return n == small[i];
    }
    uint64_t d = n - 1;
    int s = 0;
    while ((d & 1) == 0) { d >>= 1; ++s; }
    static const uint64_t bases[] =
        {2ULL, 325ULL, 9375ULL, 28178ULL, 450775ULL, 9780504ULL, 1795265022ULL};
    for (size_t i = 0; i < sizeof bases / sizeof *bases; ++i) {
        uint64_t a = bases[i] % n;
        if (a == 0) continue;
        uint64_t x = pow_mod64(a, d, n);
        if (x == 1 || x == n - 1) continue;
        bool comp = true;
        for (int r = 1; r < s; ++r) {
            x = mul_mod64(x, x, n);
            if (x == n - 1) { comp = false; break; }
        }
        if (comp) return false;
    }
    return true;
}

/* ----- Global state for memoisation ------------------------------------- */
static uint64_t *pow7 = NULL;     /* powers of 7 : pow7[i] = 7^i   */
static size_t   pow_len = 0;     /* allocated length of pow7      */
static seq_t   *prime_list = NULL; /* stored primes */
static size_t   prime_cap  = 0;     /* capacity of prime_list        */
static size_t   prime_cnt  = 0;     /* number of primes stored       */
static idx_t    gen_len    = 0;     /* largest length generated      */

/* Ensure pow7 has space up to index len (inclusive). */
static void ensure_pow7(idx_t len)
{
    if (pow_len > (size_t)len) return;         /* already enough */
    size_t new_cap = pow_len ? pow_len << 1 : 16;
    while (new_cap <= (size_t)len) new_cap <<= 1;
    uint64_t *tmp = (uint64_t*)realloc(pow7, new_cap * sizeof(uint64_t));
    if (!tmp) abort();
    if (pow_len == 0) {
        tmp[0] = 1;
        for (size_t i = 1; i < new_cap; ++i) tmp[i] = tmp[i-1] * 7ULL;
    } else {
        for (size_t i = pow_len; i < new_cap; ++i) tmp[i] = tmp[i-1] * 7ULL;
    }
    pow7    = tmp;
    pow_len = new_cap;
}

/* Ensure prime_list has capacity for at least need elements. */
static void ensure_prime_cap(size_t need)
{
    if (prime_cap >= need) return;
    size_t new_cap = prime_cap ? prime_cap << 1 : 512;
    while (new_cap < need) new_cap <<= 1;
    seq_t *tmp = (seq_t*)realloc(prime_list, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    prime_list = tmp;
    prime_cap  = new_cap;
}

/* Generate base‑7 numbers of length len and test for primality. */
static void generate_len(idx_t len)
{
    if (len <= gen_len) return;          /* already done up to this length */
    ensure_pow7(len);
    if (len == 1) {
        /* only number "1", not prime */
    } else {
        uint64_t base = pow7[len-1];      /* leading digit 1 */
        /* number of choices for remaining digits: 2^(len-1) */
        uint64_t mask_limit = 1ULL << (len-1);
        for (uint64_t mask = 0; mask < mask_limit; ++mask) {
            uint64_t val = base;
            uint64_t m = mask;
            idx_t pos = 0;
            while (m) {
                if (m & 1ULL) val += pow7[pos];
                m >>= 1;
                ++pos;
            }
            if (is_prime_u64(val)) {
                ensure_prime_cap(prime_cnt + 1);
                prime_list[prime_cnt++] = (seq_t)val;
            }
        }
    }
    gen_len = len;
}

/* ----- Public function --------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;          /* undefined for offset < 1 */
    /* ensure enough primes are generated */
    while (prime_cnt < (size_t)n) {
        generate_len(gen_len + 1);
        /* guard against overshooting in case of unforeseen limits */
        if (gen_len > 70) break;          /* 7^70 is astronomically large */
    }
    if (prime_cnt < (size_t)n) return (seq_t)0; /* not enough terms */
    return prime_list[n-1];
}
