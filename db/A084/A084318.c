/*
 * A084318  Iterate function described in A084317 if started at initial value
 *          n until reaching a fixed point.
 * https://oeis.org/A084318
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = fixed point of iterating f(x) = concatenation of distinct
 *            prime factors of x (ascending order). a(1)=0
 * Range:     u8 n <= 14, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   build_primes_upto, digit_len, is_prime_big, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>              /* size_t */
#include <stdint.h>              /* uint64_t */
#include <stdbool.h>             /* bool */
#include "../utils/digit_len.h"  /* int digit_len(seq_t) */
#include "../utils/pow10.h"      /* seq_t pow10(idx_t) */
#include "../utils/is_prime_big.h"   /* bool is_prime_big(seq_t) */
#include "../utils/build_primes_upto.h" /* void build_primes_upto(idx_t limit,
                                        uint64_t **primes_out, size_t *cnt_out) */

static uint64_t *prime_list = NULL;   /* ascending list of primes */
static size_t    prime_count = 0;     /* number of primes */

/* Initialise the small prime list once.  The limit is chosen to be large
   enough for factorising all numbers that appear in trajectories for
   initial values up to about 10 000. */
static void init_primes(void)
{
    if (prime_list != NULL)
        return;  /* already initialised */

    const idx_t sieve_limit = 1000000;   /* 1 000 000 */
    build_primes_upto(sieve_limit, &prime_list, &prime_count);
}

/* f(x): concatenate the distinct prime factors of x (in ascending order).
   For x < 2 return 0.  Prime inputs are mapped to themselves. */
static seq_t f(seq_t x)
{
    if (x < 2)
        return 0;      /* f(0)=f(1)=0 */

    if (is_prime_big(x))
        return x;      /* prime number is a fixed point */

    seq_t tmp = x;
    seq_t factors[32];
    size_t fac_cnt = 0;

    /* Trial division by precomputed primes. */
    for (size_t k = 0; k < prime_count; ++k) {
        seq_t p_val = (seq_t)prime_list[k];
        seq_t p_sq  = p_val * p_val;
        if (p_sq > tmp)
            break;
        if (tmp % p_val == 0) {
            factors[fac_cnt++] = p_val;
            while (tmp % p_val == 0)
                tmp /= p_val;
        }
    }

    /* Remaining factor, if any, is prime (or > sqrt(original)). */
    if (tmp > 1)
        factors[fac_cnt++] = tmp;

    /* Concatenate the distinct factors. */
    seq_t result = 0;
    for (size_t i = 0; i < fac_cnt; ++i) {
        seq_t f_val      = factors[i];
        int   digits     = digit_len(f_val);
        seq_t pow10_val  = pow10((idx_t)digits);
        result = result * pow10_val + f_val;
    }

    return result;
}

/* Main sequence function.  The index is 1‑based per OEIS; a(1) is the
   first listed term. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;          /* not used, but harmless */
    if (n == 1)
        return 0;          /* a(1)=0 */

    init_primes();

    seq_t val = (seq_t)n;
    while (true) {
        seq_t nxt = f(val);
        if (nxt == val)
            return nxt;
        val = nxt;
    }
}
