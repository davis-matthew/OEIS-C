/*
 * A053728  For n=1,2,3,..., compute sum of aliquot divisors of n; if result
 *          is prime append this prime to sequence.
 * https://oeis.org/A053728
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, look
 * Formula:   a(n) = the n-th prime that equals the sum of the proper divisors
 *            of some integer m (m >= 2). For each m >= 2 compute σ(m)-m; if
 *            this number is prime it is appended to the sequence
 * Range:     u8 n <= 45, u16 n <= 5992, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  The function is written in plain C (C11) and is also valid C++17.  */

/*  A small sieve of primes is used for factorisation.  The code
    never calls realloc on an array of seq_t, so it works both when
    seq_t is a plain integer type and when it is an arbitrary–precision
    integer class.  */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/* ---------- prime sieve (up to 2 000 000) --------------------------- */
#define PRIME_SIEVE_LIMIT 2000000

static uint32_t *prime_list   = NULL;
static size_t   prime_cnt     = 0;
static size_t   prime_cap     = 0;
static unsigned char *prime_sieve = NULL;

static void init_primes(void)
{
    if (prime_sieve != NULL) return;   /* already initialised */

    prime_sieve = (unsigned char*)calloc(PRIME_SIEVE_LIMIT + 1, 1);
    prime_cap   = 1024;
    prime_list  = (uint32_t*)malloc(prime_cap * sizeof(uint32_t));

    for (uint32_t i = 2; i <= PRIME_SIEVE_LIMIT; ++i)
    {
        if (!prime_sieve[i])                /* i is prime */
        {
            if (prime_cnt == prime_cap)      /* grow array */
            {
                prime_cap <<= 1;
                prime_list = (uint32_t*)realloc(prime_list, prime_cap * sizeof(uint32_t));
            }
            prime_list[prime_cnt++] = i;
            if ((uint64_t)i * i <= PRIME_SIEVE_LIMIT)
            {
                for (uint64_t j = (uint64_t)i * i; j <= PRIME_SIEVE_LIMIT; j += i)
                    prime_sieve[(size_t)j] = 1;
            }
        }
    }
}

/* ----------------- deterministic Miller–Rabin for 64‑bit ------------- */
static inline uint64_t modmul64(uint64_t a, uint64_t b, uint64_t m)
{
    return (unsigned __int128)a * b % m;
}

static inline uint64_t modpow64(uint64_t a, uint64_t e, uint64_t m)
{
    uint64_t r = 1 % m;
    while (e)
    {
        if (e & 1) r = modmul64(r, a, m);
        a = modmul64(a, a, m);
        e >>= 1;
    }
    return r;
}

static int isPrime64(uint64_t n)
{
    if (n < 2) return 0;
    static const uint64_t small[] =
        {2ULL,3ULL,5ULL,7ULL,11ULL,13ULL,17ULL,19ULL,23ULL,29ULL,31ULL,37ULL};
    for (size_t i = 0; i < sizeof(small)/sizeof(small[0]); ++i)
    {
        if (n % small[i] == 0) return n == small[i];
    }

    uint64_t d = n - 1;
    unsigned int s = 0;
    while ((d & 1) == 0) { d >>= 1; ++s; }

    static const uint64_t bases[] =
        {2ULL,325ULL,9375ULL,28178ULL,450775ULL,9780504ULL,1795265022ULL};
    for (size_t i = 0; i < sizeof(bases)/sizeof(bases[0]); ++i)
    {
        uint64_t a = bases[i] % n;
        if (a == 0) continue;
        uint64_t x = modpow64(a, d, n);
        if (x == 1 || x == n - 1) continue;
        int composite = 1;
        for (unsigned j = 1; j < s; ++j)
        {
            x = modmul64(x, x, n);
            if (x == n - 1) { composite = 0; break; }
        }
        if (composite) return 0;
    }
    return 1;
}

/* ---------- sum of proper divisors (σ(m) - m) ----------------------- */
static uint64_t sum_proper_divisors(idx_t n)
{
    if (n <= 1) return 0;
    uint64_t sum = 1;
    uint64_t tmp = n;

    for (size_t i = 0; i < prime_cnt && (uint64_t)prime_list[i] * prime_list[i] <= tmp; ++i)
    {
        uint64_t p = prime_list[i];
        if (tmp % p == 0)
        {
            uint64_t term = 1;
            uint64_t mult = 1;
            while (tmp % p == 0)
            {
                tmp   /= p;
                mult  *= p;
                term  += mult;
            }
            sum *= term;
        }
    }
    if (tmp > 1) sum *= (tmp + 1);   /* remaining prime factor */

    return sum - n;   /* proper divisors exclude n itself */
}

/* ---------- sequence cache ---------------------------------------- */
#define SEQ_CACHE_MAX 200000       /* large enough for the test range */

static seq_t seq_cache[SEQ_CACHE_MAX];
static size_t seq_len      = 0;
static idx_t  processed_m  = 1;   /* last m value processed */

/* fill the cache until it contains at least target terms */
static void ensure_sequence(idx_t target)
{
    if (seq_len >= (size_t)target) return;

    if (prime_sieve == NULL) init_primes();

    while (seq_len < (size_t)target)
    {
        ++processed_m;                     /* next m to inspect */
        uint64_t s = sum_proper_divisors(processed_m);
        if (isPrime64(s))
        {
            seq_cache[seq_len++] = (seq_t)s;   /* cast is safe for 64‑bit results */
        }
    }
}

/* ---------- public routine ----------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;         /* sequence is 1‑indexed */
    ensure_sequence(n);
    return seq_cache[(size_t)n - 1];
}
