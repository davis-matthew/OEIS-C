/*
 * A107189  Primes of the form 3x^2 + 26y^2.
 * https://oeis.org/A107189
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 812, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A107189 – primes of the form 3*x^2 + 26*y^2.
    The function a(idx_t n) returns the n-th such prime (n ≥ 1).
    It uses a simple sieve of Eratosthenes for primality testing
    and checks the quadratic form for each candidate prime.  */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>

/* ------------- resonant residues modulo 312 (see OEIS) ------------- */
static unsigned char RESIDUES[312];
static bool residues_initialized = false;

static void init_residues(void)
{
    if (residues_initialized) return;
    static const unsigned int r[] = {
        3, 29, 35, 53, 77, 101, 107, 131,
        155, 173, 179, 251, 269
    };
    for (size_t i = 0; i < sizeof(r)/sizeof(r[0]); ++i)
        RESIDUES[r[i]] = 1;
    residues_initialized = true;
}

/* ------------ floor(sqrt(x)) for unsigned 64‑bit integers ------------ */
static inline unsigned long long isqrt64(unsigned long long x)
{
    unsigned long long r = (unsigned long long)sqrtl((long double)x);
    while ((r + 1) * (r + 1) <= x) ++r;
    while (r * r > x) --r;
    return r;
}

/* -------------- small prime sieve --------------------------------- */
static unsigned char *is_composite = NULL;   /* 0 = prime candidate, 1 = composite */
static unsigned long long sieve_limit   = 0;

static void extend_sieve(unsigned long long limit)
{
    if (limit <= sieve_limit) return;

    unsigned long long new_limit = limit;
    unsigned char *new_arr = (unsigned char*)realloc(is_composite, new_limit + 1);
    if (!new_arr) abort();
    is_composite = new_arr;

    /* initialize all entries to 0 (prime candidate) */
    memset(is_composite, 0, new_limit + 1);
    is_composite[0] = 1;
    if (new_limit >= 1) is_composite[1] = 1;

    for (unsigned long long p = 2; p * p <= new_limit; ++p)
    {
        if (!is_composite[p])
        {
            for (unsigned long long m = p * p; m <= new_limit; m += p)
                is_composite[m] = 1;
        }
    }
    sieve_limit = new_limit;
}

static inline int is_prime64(unsigned long long n)
{
    if (n < 2) return 0;
    if (n <= sieve_limit)
        return !is_composite[(size_t)n];
    /* extend the sieve a bit beyond n */
    extend_sieve(n * 2 + 1000);
    return !is_composite[(size_t)n];
}

/* -------------- test whether a prime can be written 3x^2+26y^2 ------ */
static int is_form_prime(unsigned long long p)
{
    if (p < 3) return 0;
    init_residues();
    if (!RESIDUES[p % 312]) return 0;

    unsigned long long max_y = isqrt64(p / 26ULL);
    for (unsigned long long y = 0; y <= max_y; ++y)
    {
        unsigned long long y2 = y * y;
        unsigned long long rem = p - 26ULL * y2;
        if (rem % 3ULL) continue;
        unsigned long long x2 = rem / 3ULL;
        unsigned long long x = isqrt64(x2);
        if (x * x == x2) return 1;
    }
    return 0;
}

/* -------------- storage for found special primes ------------------- */
static uint64_t *special_primes = NULL;
static size_t special_cnt = 0;
static size_t special_cap = 0;
static uint64_t next_candidate = 3;   /* next odd number to test */

static void ensure_special_capacity(size_t needed_cap)
{
    if (special_cap >= needed_cap) return;
    size_t new_cap = (special_cap == 0) ? 128 : special_cap << 1;
    if (new_cap < needed_cap) new_cap = needed_cap;
    uint64_t *tmp = (uint64_t*)realloc(special_primes, new_cap * sizeof(uint64_t));
    if (!tmp) abort();
    special_primes = tmp;
    special_cap = new_cap;
}

static void generate_special_primes_upto(size_t target)
{
    if (special_cnt >= target) return;

    while (special_cnt < target)
    {
        if (next_candidate <= sieve_limit)
        {
            if (!is_composite[next_candidate] && is_prime64(next_candidate) && is_form_prime(next_candidate))
            {
                ensure_special_capacity(special_cnt + 1);
                special_primes[special_cnt++] = next_candidate;
            }
        }
        else
        {
            /* ensure sieve covers the candidate */
            extend_sieve(next_candidate * 2 + 1000);
            if (!is_composite[next_candidate] && is_prime64(next_candidate) && is_form_prime(next_candidate))
            {
                ensure_special_capacity(special_cnt + 1);
                special_primes[special_cnt++] = next_candidate;
            }
        }
        next_candidate += 2;   /* skip even numbers */
    }
}

/* --------------------------------- a(n) --------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                       /* undefined for offset 1 */
    generate_special_primes_upto((size_t)n);
    return (seq_t)special_primes[(size_t)(n - 1)];
}
