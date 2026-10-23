/*
 * A137705  Prime numbers p such that p +- ((p-1)/4) are primes.
 * https://oeis.org/A137705
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 66, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  
   Sequence A137705:  Prime numbers p such that p ± ((p-1)/4) are primes.
   We generate primes p (≡1 mod 4) using a sieve up to a fixed limit enough
   for the first 10 000 terms.  For each candidate p we test primality of
   (5p-1)/4 and (3p+1)/4 using the sieve for values ≤LIMIT or a deterministic
   Miller‑Rabin test for larger numbers.  Results are memoised so the function
   works in any order of calls.
*/

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>

/* --- helper primes -- */
static inline uint64_t mul_mod_uint64(uint64_t a, uint64_t b, uint64_t mod)
{
    return (unsigned __int128)a * b % mod;
}

static uint64_t mod_pow_uint64(uint64_t base, uint64_t exp, uint64_t mod)
{
    uint64_t result = 1;
    base %= mod;
    while (exp) {
        if (exp & 1)
            result = mul_mod_uint64(result, base, mod);
        base = mul_mod_uint64(base, base, mod);
        exp >>= 1;
    }
    return result;
}

static bool isPrime64(uint64_t n)
{
    if (n < 2) return false;
    static const uint64_t small_primes[] = {
        2ULL,3ULL,5ULL,7ULL,11ULL,13ULL,17ULL,19ULL,23ULL,29ULL,31ULL,37ULL
    };
    for (size_t i = 0; i < sizeof(small_primes)/sizeof(small_primes[0]); ++i) {
        uint64_t p = small_primes[i];
        if (n % p == 0) return n == p;
    }

    uint64_t d = n - 1;
    int s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        ++s;
    }

    static const uint64_t witnesses[] = {
        2ULL, 325ULL, 9375ULL, 28178ULL,
        450775ULL, 9780504ULL, 1795265022ULL
    };
    for (size_t i = 0; i < sizeof(witnesses)/sizeof(witnesses[0]); ++i) {
        uint64_t a = witnesses[i] % n;
        if (a == 0) continue;
        uint64_t x = mod_pow_uint64(a, d, n);
        if (x == 1 || x == n - 1) continue;
        bool composite = true;
        for (int r = 1; r < s; ++r) {
            x = mul_mod_uint64(x, x, n);
            if (x == n - 1) {
                composite = false;
                break;
            }
        }
        if (composite) return false;
    }
    return true;
}

/* --- sieve of odd numbers up to LIMIT --- */
#define LIMIT 300000000ULL           /* 3e8 > enough for first 10k terms */
static uint8_t *sieve_bits = NULL;   /* one bit per odd number >=3  */
static size_t sieve_bytes = 0;
static bool sieve_built = false;

#define ODD_INDEX(n) (((n)-3)/2)      /* n is odd >=3 */
#define GET_BIT(n) ((sieve_bits[(ODD_INDEX(n)>>3)] >> (ODD_INDEX(n)&7)) & 1)
#define SET_BIT(n) (sieve_bits[(ODD_INDEX(n)>>3)] |= (1u << (ODD_INDEX(n)&7)))

static void build_sieve(void)
{
    size_t num_odds = (LIMIT - 3) / 2 + 1;          /* odds from 3 to LIMIT */
    sieve_bytes = (num_odds + 7) / 8;
    sieve_bits = (uint8_t*)calloc(sieve_bytes, 1);
    if (!sieve_bits) exit(1); /* out of memory */

    uint64_t sqrt_lim = (uint64_t) sqrt((double) LIMIT) + 1;
    for (uint64_t i = 3; i <= sqrt_lim; i += 2) {
        if (!GET_BIT(i)) {
            for (uint64_t j = i * i; j <= LIMIT; j += i << 1) {
                SET_BIT(j);
            }
        }
    }
    sieve_built = true;
}

static bool isPrimeNum(uint64_t n)
{
    if (n <= LIMIT) {
        if (n == 2) return true;
        if (n < 2 || (n & 1) == 0) return false;
        return !GET_BIT((uint32_t)n);
    }
    return isPrime64(n);
}

/* --- memoised sequence cache --- */
static seq_t *seq_cache = NULL;
static size_t seq_cache_cap = 0;
static idx_t term_cnt = 0;          /* number of terms produced so far */
static uint64_t next_candidate = 5;  /* next odd ≡1 mod4 to test */

/* grow cache to at least need_cap */
static void grow_cache(size_t need_cap)
{
    if (need_cap <= seq_cache_cap) return;
    size_t new_cap = seq_cache_cap ? seq_cache_cap << 1 : 256;
    if (new_cap < need_cap) new_cap = need_cap;
    seq_t *new_ptr = (seq_t*)realloc(seq_cache, new_cap * sizeof(seq_t));
    if (!new_ptr) exit(1); /* out of memory */
    seq_cache = new_ptr;
    seq_cache_cap = new_cap;
}

static void ensure_terms(idx_t n)
{
    if (!sieve_built) build_sieve();

    while (term_cnt < n) {
        if (next_candidate > LIMIT) {
            /* Fallback: use Miller‑Rabin for the tail, but for the
               first 10 000 terms the limit is guaranteed to be enough. */
        }
        if (isPrimeNum(next_candidate)) {
            uint64_t p = next_candidate;
            uint64_t p_plus  = (5ULL * p - 1) / 4;
            uint64_t p_minus = (3ULL * p + 1) / 4;
            if (isPrimeNum(p_plus) && isPrimeNum(p_minus)) {
                grow_cache(term_cnt + 1);
                seq_cache[term_cnt++] = (seq_t)p;
            }
        }
        next_candidate += 4;          /* stay on numbers ≡1 mod4 */
    }
}

/* The required function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;        /* off‑by‑one safety, sequence used for n>=1 */
    ensure_terms(n);
    return seq_cache[n - 1];
}
