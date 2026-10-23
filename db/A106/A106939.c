/*
 * A106939  Primes of the form 4x^2+3xy+5y^2, with x and y nonnegative.
 * https://oeis.org/A106939
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 42, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Primes of the form 4x²+3xy+5y² with x,y≥0.
    The function returns the n‑th such prime,  a(1)=5.
    Representation test: p = 4x²+3xy+5y² ⇔
    Δ = 16p – 71y² is a non‑negative perfect square, and
    (√Δ – 3y) ≡ 0 mod 8.  */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* ----- small integer utilities ----------------------------------- */

static uint64_t isqrt128(unsigned __int128 x)
{
    unsigned __int128 r = 0;
    for (int i = 63; i >= 0; --i) {
        unsigned __int128 t = r | ((unsigned __int128)1 << i);
        if (t * t <= x) r = t;
    }
    return (uint64_t)r;      /* floor(sqrt(x)) – fits in 64 bits */
}

/* Deterministic Miller–Rabin for 64‑bit integers. */
static bool is_prime_uint64(uint64_t n)
{
    if (n < 2) return false;
    static const uint64_t small_primes[] =
        {2ULL,3ULL,5ULL,7ULL,11ULL,13ULL,17ULL,19ULL,23ULL,29ULL,31ULL,37ULL};
    for (size_t i = 0; i < sizeof(small_primes)/sizeof(small_primes[0]); ++i) {
        if (n % small_primes[i] == 0) return n == small_primes[i];
    }

    uint64_t d = n - 1, s = 0;
    while ((d & 1ULL) == 0) { d >>= 1; ++s; }

    static const uint64_t witnesses[] = {2ULL, 3ULL, 5ULL, 7ULL, 11ULL, 13ULL};
    for (size_t i = 0; i < sizeof(witnesses)/sizeof(witnesses[0]); ++i) {
        uint64_t a = witnesses[i] % n;
        if (a == 0) continue;
        unsigned __int128 x = 1, base = a, exp = d;
        while (exp) {
            if (exp & 1ULL) x = (unsigned __int128)x * base % n;
            base = (unsigned __int128)base * base % n;
            exp >>= 1;
        }
        if (x == 1 || x == n - 1) continue;
        bool cont = false;
        for (uint64_t r = 1; r < s; ++r) {
            x = (unsigned __int128)x * x % n;
            if (x == n - 1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

/* Test whether the prime can be written as 4x²+3xy+5y² (x,y≥0). */
static bool has_representation(uint64_t p)
{
    for (uint64_t y = 0;; ++y) {
        unsigned __int128 yy     = (unsigned __int128)y * y;
        unsigned __int128 val71  = (unsigned __int128)71 * yy;
        unsigned __int128 left   = (unsigned __int128)16 * p;
        if (left < val71) break;                      /* Δ < 0  */
        unsigned __int128 delta = left - val71;        /* Δ = 16p – 71y² */
        uint64_t s = isqrt128(delta);
        if ((unsigned __int128)s * s != delta) continue;
        if (s < 3 * y) continue;
        if (((s - 3 * y) & 7ULL) == 0) return true;   /* (s‑3y) mod 8 == 0 */
    }
    return false;
}

/* ----- state for memoised generation ----------------------------- */

static uint64_t *prime_list = NULL;
static size_t prime_cap    = 0;
static size_t prime_cnt    = 0;
static uint64_t next_candidate;   /* next odd number to test */

/* Ensure capacity for another stored prime */
static void ensure_prime_capacity(void)
{
    if (prime_cnt >= prime_cap) {
        size_t new_cap = prime_cap ? prime_cap * 2 : 64;
        uint64_t *tmp = (uint64_t *)realloc(prime_list, new_cap * sizeof(uint64_t));
        if (!tmp) exit(EXIT_FAILURE);
        prime_list = tmp;
        prime_cap  = new_cap;
    }
}

/* Initialise state lazily on the first call */
static void initialise(void)
{
    if (prime_list == NULL) {          /* first execution */
        next_candidate = 5;            /* first prime of the form */
        ensure_prime_capacity();
    }
}

/* ----- the required sequence function --------------------------- */

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* out‑of‑range index is not used */

    initialise();                  /* set up static variables */

    /* Grow the list until it contains at least n primes of the form */
    while (prime_cnt < n) {
        if (is_prime_uint64(next_candidate)) {
            if (has_representation(next_candidate)) {
                ensure_prime_capacity();
                prime_list[prime_cnt++] = next_candidate;
            }
        }
        next_candidate += 2;           /* skip even numbers */
    }
    return (seq_t)prime_list[n - 1];
}
