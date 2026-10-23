/*
 * A107169  Primes of the form 3x^2 + 20y^2.
 * https://oeis.org/A107169
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 824, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: returns the nth prime p such that p = 3*x^2 + 20*y^2 for some integers x,y.
   Equivalently p = 3 or p ≡ 23 or 47 (mod 60).  The function generates
   these primes on demand, storing them in a static array.  Subsequent calls
   reuse the stored values, giving O(1) access after an initial linear
   search. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

/* ----- 64‑bit modular multiplication and exponentiation (deterministic) ----- */
static uint64_t mulmod(uint64_t a, uint64_t b, uint64_t mod)
{
    return (__uint128_t)a * b % mod;
}

static uint64_t powmod(uint64_t base, uint64_t exp, uint64_t mod)
{
    uint64_t result = 1;
    base %= mod;
    while (exp) {
        if (exp & 1) result = mulmod(result, base, mod);
        base = mulmod(base, base, mod);
        exp >>= 1;
    }
    return result;
}

/* Deterministic Miller‑Rabin for 64‑bit integers. */
static bool is_prime_uint64(uint64_t n)
{
    if (n < 2) return false;
    static const uint64_t small_primes[] =
        {2ULL,3ULL,5ULL,7ULL,11ULL,13ULL,17ULL,19ULL,23ULL,29ULL,31ULL,37ULL};
    for (size_t i = 0; i < sizeof small_primes / sizeof small_primes[0]; ++i) {
        if (n % small_primes[i] == 0) return n == small_primes[i];
    }
    uint64_t d = n - 1;
    int s = 0;
    while ((d & 1) == 0) { d >>= 1; ++s; }
    static const uint64_t bases[] =
        {2ULL, 325ULL, 9375ULL, 28178ULL, 450775ULL, 9780504ULL, 1795265022ULL};
    for (size_t i = 0; i < sizeof bases / sizeof bases[0]; ++i) {
        uint64_t a = bases[i] % n;
        if (a == 0) continue;
        uint64_t x = powmod(a, d, n);
        if (x == 1 || x == n - 1) continue;
        bool composite = true;
        for (int r = 1; r < s; ++r) {
            x = mulmod(x, x, n);
            if (x == n - 1) { composite = false; break; }
        }
        if (composite) return false;
    }
    return true;
}

/* ----- Storage for the primes of the required form ----- */
static uint64_t *form_primes = NULL;   /* array of primes */
static size_t form_size  = 0;          /* current number of stored primes */
static size_t form_cap   = 0;          /* allocated capacity */
static uint64_t last_checked = 3;      /* last candidate examined */

/* Ensure the array has enough space for at least `need` elements */
static void ensure_form_cap(size_t need)
{
    if (need <= form_cap) return;
    size_t new_cap = form_cap ? form_cap * 2 : 8;
    while (new_cap < need) new_cap <<= 1;
    uint64_t *tmp = (uint64_t *)realloc(form_primes, new_cap * sizeof(uint64_t));
    if (!tmp) abort();
    form_primes = tmp;
    form_cap = new_cap;
}

/* Generate primes until we have at least `target` primes of the required form */
static void generate_form_primes_to(size_t target)
{
    if (form_size >= target) return;   /* already have enough */

    if (form_size == 0) {              /* insert the special first term */
        ensure_form_cap(1);
        form_primes[0] = 3ULL;
        form_size = 1;
    }

    uint64_t cand = last_checked + 2;  /* start from the next odd number */

    while (form_size < target) {
        if (is_prime_uint64(cand)) {
            uint64_t mod60 = cand % 60;
            if (mod60 == 23 || mod60 == 47) {
                ensure_form_cap(form_size + 1);
                form_primes[form_size++] = cand;
            }
        }
        last_checked = cand;
        cand += 2;                    /* next odd candidate */
    }
}

/* ----- The required sequence function ----- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;        /* behaviour for n<1 is irrelevant */
    size_t target = (size_t)n;          /* need n primes in total */
    generate_form_primes_to(target);
    return (seq_t)form_primes[(size_t)(n - 1)];
}
