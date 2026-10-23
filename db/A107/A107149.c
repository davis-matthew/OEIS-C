/*
 * A107149  Primes of the form 4x^2 + 11y^2.
 * https://oeis.org/A107149
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 552, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   mul_mod, representable_by_form
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes primes of the form 4*x^2 + 11*y^2 by scanning primes in
 * increasing order and testing whether each matches the required
 * quadratic form.  The sequence is memoised so calls with arbitrary
 * indices are served efficiently. */
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include "../utils/mul_mod.h"
#include "../utils/representable_by_form.h"

/* ------------------------------------------------------------ */
/*  Simple deterministic Miller–Rabin primality test for 64‑bit  */
/* ------------------------------------------------------------ */

static uint64_t mod_pow(uint64_t base, uint64_t exp, uint64_t mod)
{
    uint64_t result = 1;
    while (exp) {
        if (exp & 1ULL)
            result = mul_mod(result, base, mod);
        base = mul_mod(base, base, mod);
        exp >>= 1ULL;
    }
    return result;
}

static bool is_prime(uint64_t n)
{
    if (n < 2)                       return false;
    static const uint64_t small[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    for (size_t i = 0; i < sizeof small / sizeof *small; ++i) {
        if (n == small[i])           return true;
        if (n % small[i] == 0ULL)    return false;
    }

    /* Write n-1 as d * 2^s with d odd */
    uint64_t d = n - 1;
    unsigned s = 0;
    while ((d & 1ULL) == 0ULL) {
        d >>= 1ULL;
        ++s;
    }

    /* Deterministic bases for 64‑bit integers */
    static const uint64_t bases[] = {2, 3, 5, 7, 11, 13, 17};
    for (size_t i = 0; i < sizeof bases / sizeof *bases; ++i) {
        uint64_t a = bases[i];
        if (a % n == 0ULL) continue;          /* a == 0 mod n -> n divides a */
        uint64_t x = mod_pow(a, d, n);
        if (x == 1 || x == n - 1) continue;
        bool composite = true;
        for (unsigned r = 1; r < s; ++r) {
            x = mul_mod(x, x, n);
            if (x == n - 1) {
                composite = false;
                break;
            }
        }
        if (composite) return false;
    }
    return true;
}

/* ------------------------------------------------------------ */
/*  Representation test: does p = 4*x^2 + 11*y^2 ?              */

/* ------------------------------------------------------------ */
/*  Memoised storage of terms                                */
/* ------------------------------------------------------------ */
static seq_t *terms = NULL;
static idx_t terms_size   = 0;   /* number of stored terms   */
static idx_t terms_cap    = 0;   /* capacity of the array    */
static uint64_t next_candidate = 3; /* next integer to test   */

static void add_term(seq_t val)
{
    if (terms_size == terms_cap) {
        idx_t new_cap = terms_cap ? terms_cap * 2 : 64;
        seq_t *tmp = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
        if (!tmp) abort();          /* allocation failure */
        terms = tmp;
        terms_cap = new_cap;
    }
    terms[terms_size++] = val;
}

/* ------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return 0;              /* offset is 1 */

    if (n <= terms_size)              /* already computed */
        return terms[n - 1];

    /* Compute further terms until we reach the requested index */
    while (terms_size < n) {
        /* Find the next prime */
        while (!is_prime(next_candidate))
            ++next_candidate;

        /* Test whether the prime fits the quadratic form */
        if (representable_by_form(next_candidate))
            add_term((seq_t)next_candidate);

        ++next_candidate;              /* advance to next number for next round */
    }

    return terms[n - 1];
}
