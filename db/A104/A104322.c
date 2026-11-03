/*
 * A104322  Primes which are the reverse concatenation of five consecutive
 *          cubes.
 * https://oeis.org/A104322
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 none, u64 none, u128 none, big any n
 * Validated: partial, pass, 6 terms through n = 6 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A104322: primes that are the reverse concatenation of five
    consecutive cubes.
    For a given starting integer k ≥ 1 we construct the string
    (k+4)^3 · (k+3)^3 · (k+2)^3 · (k+1)^3 · k^3
    where · denotes decimal concatenation (the cubes are written in
    normal decimal order but the whole block is concatenated in reverse
    order).  The resulting integer is tested for primality; all such
    primes are listed in increasing order of k.  a(1) is the first
    prime produced in this way.  The sequence is indexed starting at 1. */

#include <stdbool.h>
#include <stdint.h>

static seq_t pow10(idx_t exp) {
    seq_t res = 1;
    for (idx_t i = 0; i < exp; ++i) res *= (seq_t)10;
    return res;
}

/* Return the number of decimal digits of x. */
static idx_t digit_len(seq_t x)
{
    idx_t len = 0;
    seq_t t = x;
    while (t > 0) {
        t /= 10;
        len++;
    }
    return len;
}

/* Miller–Rabin primality test for arbitrary-precision values. */
static bool isprime_big(seq_t n)
{
    if (n < 2) return false;
    if (n == 2 || n == 3) return true;
    if ((n & 1) == 0) return false;

    seq_t d = n - 1;
    idx_t r = 0;
    while ((d & 1) == 0) {
        d = d / 2;
        r = r + 1;
    }

    /* Bases sufficient for a probabilistic test. */
    const uint64_t bases[] = {2,3,5,7,11,13,17,19,23,29};
    for (idx_t i = 0; i < sizeof(bases)/sizeof(bases[0]); ++i) {
        seq_t a = (seq_t)bases[i];
        if (a >= n) break;

        /* modular exponentiation x = a^d mod n */
        seq_t x = 1;
        seq_t base = a % n;
        seq_t exp  = d;
        while (exp > 0) {
            if ((exp & 1) != 0) x = (x * base) % n;
            base = (base * base) % n;
            exp  = exp >> 1;
        }

        if (x == 1 || x == n - 1) continue;

        bool cont = false;
        for (idx_t j = 1; j < r; ++j) {
            x = (x * x) % n;
            if (x == n - 1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

seq_t a(idx_t n)
{
    /* The first few terms grow very rapidly; we keep a static cache
       and generate terms on demand.  The cache is big enough to hold
       at least 10000 terms for the purposes of the tests. */
    static seq_t  terms[200000];
    static idx_t  size      = 0;   /* number of terms stored          */
    static idx_t  last_k    = 0;   /* last starting cube index used   */

    if (n == 0) return 0;          /* the sequence is 1‑based */

    /* Generate primes until we have at least n of them.  */
    while (size < n) {
        ++last_k;                                 /* current k      */
        seq_t k = (seq_t)last_k;

        /* Compute the five consecutive cubes. */
        seq_t cube[5];
        for (int i = 0; i < 5; ++i) {
            seq_t x = k + (seq_t)i;
            cube[i] = x * x * x;
        }

        /* Build the concatenation in reverse cube order:
           (k+4)^3 · ... · k^3. */
        seq_t concat = 0;
        for (int idx = 4; idx >= 0; --idx) {
            unsigned len = (unsigned)digit_len(cube[idx]); /* number of decimal digits */
            concat = concat * pow10(len) + cube[idx];
        }

        /* Test for primality and store if prime. */
        if (isprime_big(concat)) {
            terms[size++] = concat;
        }
    }

    return terms[n - 1];
}
