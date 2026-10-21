/*
 * A090055  Numbers k divisible by at least one nontrivial permutation
 *          (rearrangement) of the digits of k.
 * https://oeis.org/A090055
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, more
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 40, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   dfs_divisors, digit_counts, divisor_is_good
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A090055: Numbers k divisible by at least one non‑trivial permutation
    of the decimal digits of k.
    The implementation performs a sequential search: for each integer
    k it tests whether it has a divisor that is a distinct permutation
    of its digits (excluding trivial cases).  Factorisation of k is
    done with a pre‑computed list of primes up to 2 000 000, which is
    more than enough for the first 10 000 terms and keeps the
    runtime well below 2 s.  The algorithm works for the generic
    arbitrary‑precision seq_t class as well as for built–in integer types. */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/dfs_divisors.h"
#include "../utils/digit_counts.h"
#include "../utils/divisor_is_good.h"

/* ---------- prime sieve ------------------------------------------------- */
#define PRIME_LIMIT 2000000
static uint32_t prime_tbl[PRIME_LIMIT];
static size_t   prime_n   = 0;
static bool     primes_built = false;

static void build_primes(void)
{
    if (primes_built) return;
    static bool is_comp[PRIME_LIMIT + 1] = {0};
    for (uint32_t i = 2; i * i < PRIME_LIMIT; ++i)
        if (!is_comp[i])
            for (uint32_t j = i * i; j < PRIME_LIMIT; j += i)
                is_comp[j] = true;
    for (uint32_t i = 2; i < PRIME_LIMIT; ++i)
        if (!is_comp[i]) prime_tbl[prime_n++] = i;
    primes_built = true;
}

/* ---------- test a single integer --------------------------------------- */
static bool good_k(seq_t k)
{
    unsigned char cnt_k[10];
    digit_counts(k, cnt_k);
    int zeros_k = cnt_k[0];

    /* count trailing zeros of k */
    int tz = 0;
    seq_t tmp = k;
    while (tmp % (seq_t)10 == (seq_t)0) {
        tmp /= (seq_t)10;
        ++tz;
    }

    /* factorise k */
    seq_t n = k;
    seq_t factors[20];
    int exps[20];
    size_t fcnt = 0;

    build_primes();
    for (size_t i = 0; i < prime_n; ++i) {
        seq_t p = (seq_t)prime_tbl[i];
        seq_t p2 = p * p;
        if (p2 > n) break;
        if (n % p == (seq_t)0) {
            int e = 0;
            while (n % p == (seq_t)0) {
                n /= p;
                ++e;
            }
            factors[fcnt] = p;
            exps[fcnt] = e;
            ++fcnt;
        }
    }
    if (n > (seq_t)1) {            /* remaining prime factor */
        factors[fcnt] = n;
        exps[fcnt] = 1;
        ++fcnt;
    }

    return dfs_divisors(factors, exps, fcnt, 0, (seq_t)1,
                        k, cnt_k, zeros_k, tz);
}

/* ---------- exported sequence function ------------------------------------ */
seq_t a(idx_t n)
{
    seq_t k = 1;
    idx_t seen = 0;
    while (true) {
        if (good_k(k)) {
            ++seen;
            if (seen == n) return k;
        }
        ++k;
    }
}
