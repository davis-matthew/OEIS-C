/*
 * A038983  Primes p such that 94 is a square mod p.
 * https://oeis.org/A038983
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 24, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   grow_seq, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth prime p such that 94 is a quadratic residue mod p
   (OEIS A038983).  The function performs a lazy prime search using
   a simple incremental sieve and tests each prime with Euler's
   criterion.  It also treats the primes dividing 94 (2 and 47) as
   good because 94 ≡ 0 (mod p) is a perfect square. */
#include <stdbool.h>
#include <stddef.h>
#include "../utils/grow_seq.h"   /* void grow_seq(seq_t **arr, size_t *cap, size_t need); */
#include "../utils/mod_pow.h"    /* seq_t mod_pow(seq_t base, uint64_t exp, seq_t mod); */

static seq_t *primes = NULL;      /* all primes discovered so far          */
static size_t   prime_cnt = 0;
static size_t   prime_cap = 0;

static seq_t *good   = NULL;      /* primes with 94 a quadratic residue */
static size_t   good_cnt = 0;
static size_t   good_cap = 0;

static uint64_t next_candidate = 3;    /* next odd number to test          */
static bool initialized = false;

/* Trivial trial‑division primality test using the primes already found. */
static bool is_prime(seq_t x)
{
    if (x < 2) return false;
    uint64_t xx = (uint64_t)x;
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = (uint64_t)primes[i];
        if (p * p > xx) break;
        if (xx % p == 0) return false;
    }
    return true;
}

/* Returns true iff 94 is a quadratic residue modulo x. */
static bool good_prime(seq_t x)
{
    if (x == 2) return true;                  /* 94 ≡ 0 (mod 2) is a square */
    uint64_t ux = (uint64_t)x;
    if (94 % ux == 0) return true;            /* handles p = 47 */

    uint64_t exp = (ux - 1) >> 1;             /* (x‑1)/2 */
    seq_t residue = mod_pow((seq_t)94, exp, x);
    return residue == 1;
}

/* Initialise static storage (first prime 2 and first good prime 2). */
static void init(void)
{
    grow_seq(&primes, &prime_cap, 1);
    primes[0] = 2;
    prime_cnt = 1;

    grow_seq(&good, &good_cap, 1);
    good[0] = 2;
    good_cnt = 1;
    initialized = true;
}

/* Return the nth term of A038983 (with offset 1). */
seq_t a(idx_t n)
{
    if (n < 1) return 0;

    if (!initialized) init();

    while (good_cnt < (size_t)n) {
        seq_t candidate = (seq_t)next_candidate;

        /* Find the next odd prime. */
        while (!is_prime(candidate)) {
            next_candidate += 2;
            candidate = (seq_t)next_candidate;
        }

        /* Store discovered prime. */
        grow_seq(&primes, &prime_cap, prime_cnt + 1);
        primes[prime_cnt++] = candidate;

        /* Check the quadratic‑residue property. */
        if (good_prime(candidate)) {
            grow_seq(&good, &good_cap, good_cnt + 1);
            good[good_cnt++] = candidate;
        }

        /* Prepare for the next odd number. */
        next_candidate += 2;
    }
    return good[n - 1];
}
