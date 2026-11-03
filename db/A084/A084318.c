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
 * Validated: partial, pass, 66 terms through n = 66 (2026-11-03)
 * Helpers:   build_primes_upto, is_prime_u64, num_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>              /* size_t */
#include <stdint.h>              /* uint64_t */
#include <stdbool.h>             /* bool */
#include <stdlib.h>
#include "../utils/build_primes_upto.h"
#include "../utils/num_digits.h"
#include "../utils/is_prime_u64.h"

/* Compute 10^e for a non‑negative integer exponent e */
static seq_t pow10(idx_t e) {
    seq_t res = 1;
    while (e) {
        res *= 10;
        e--;
    }
    return res;
}

/* Miller–Rabin primality test for seq_t (supports 8/16/32/64/128/ arbitrary-precision) */
/* n fits in uint64_t (n >= 0 here) */
static bool is_prime_big_fits_u64(seq_t v)
{
    if (sizeof(seq_t) <= 8) return true;
    return !((seq_t)UINT64_MAX < v);
}

static bool is_prime_big(seq_t n) {
    if (n < 2) return false;
    /* exact in 64 bits when n fits: the seq_t products below overflow at
       u64/i64 once n passes 2^32, and are slow under big */
    if (is_prime_big_fits_u64(n)) return is_prime_u64((uint64_t)n);
    if (n == 2 || n == 3) return true;
    if ((n & 1) == 0) return false;

    /* write n-1 as d * 2^s with d odd */
    seq_t d = n - 1;
    unsigned s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        s++;
    }

    /* deterministic bases covering 128‑bit values */
    static const int base_list[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    const int num_bases = sizeof(base_list)/sizeof(base_list[0]);

    for (int i = 0; i < num_bases; i++) {
        seq_t a = (seq_t)base_list[i];
        if (a >= n) continue;

        /* compute a^d mod n */
        seq_t x = 1;
        seq_t pow = a % n;
        seq_t exp = d;
        while (exp > 0) {
            if (exp & 1) x = (x * pow) % n;
            pow = (pow * pow) % n;
            exp >>= 1;
        }

        if (x == 1 || x == n-1) continue;

        bool cont = false;
        for (unsigned r = 1; r < s; r++) {
            x = (x * x) % n;
            if (x == n-1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

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
        int   digits     = num_digits(f_val);
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
