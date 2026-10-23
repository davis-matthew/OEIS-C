/*
 * A046840  Numbers k such that the number of divisors of k divides the sum of
 *          the 4th powers of the divisors of k.
 * https://oeis.org/A046840
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 154, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A046840: numbers k such that d(k) divides σ₄(k)
   (d(k) = number of divisors of k, σ₄(k) = ∑_{d|k} d⁴).
   The function generates terms by testing consecutive integers
   starting from 1.  The sequence is stored in a static array
   and grown lazily to answer arbitrary queries in any order.  */
/* The implementation uses the provided factor_u64 routine
   to factor each k.  Divisibility is tested modulo d(k) */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"

/* Multiply two 64‑bit numbers modulo mod, using 128‑bit intermediate */
static inline uint64_t mul_mod(uint64_t a, uint64_t b, uint64_t mod)
{
    return (uint64_t)((__uint128_t)a * b % mod);
}

/* Static storage for the generated sequence */
static seq_t *seq       = NULL;   /* array of values */
static size_t seq_len   = 0;      /* number of values stored */
static size_t seq_cap   = 0;      /* allocated capacity */

/* Current value of k being tested during generation */
static uint64_t cur_k   = 1;

/* Ensure the array has room for at least n elements */
static void ensure_seq_cap(size_t n)
{
    if (n <= seq_cap)
        return;

    size_t new_cap = seq_cap ? seq_cap : 256;
    while (new_cap < n)
        new_cap <<= 1;

    if (seq == NULL) {
        seq = (seq_t *)malloc(new_cap * sizeof(seq_t));
    } else {
        seq = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
    }
    seq_cap = new_cap;
}

/* Return true if k satisfies d(k) | σ₄(k) */
static bool good_k(uint64_t k)
{
    if (k == 1)                /* d(1)=1, σ₄(1)=1 → divisible */
        return true;

    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64(k, primes, exps, &cnt);

    /* Compute d(k) = ∏(e_i + 1) */
    uint64_t d = 1;
    for (size_t i = 0; i < cnt; ++i)
        d *= (exps[i] + 1);

    if (d == 1)                /* only possible for k=1, already handled */
        return true;

    /* Compute σ₄(k) modulo d */
    uint64_t sigma_mod = 1 % d;   /* will hold product of sums */

    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p   = primes[i];
        uint64_t e   = exps[i];
        uint64_t p2  = mul_mod(p, p, d);          /* p² mod d   */
        uint64_t p4  = mul_mod(p2, p2, d);        /* p⁴ mod d   */

        /* Sum_{j=0..e} (p⁴)^j  modulo d */
        uint64_t term = 1 % d;    /* (p⁴)^0 */
        uint64_t sum  = 1 % d;    /* start with 1 */
        for (uint64_t j = 1; j <= e; ++j) {
            term = mul_mod(term, p4, d);
            sum  += term;
            if (sum >= d)
                sum -= d;
        }

        sigma_mod = mul_mod(sigma_mod, sum, d);
    }

    return sigma_mod == 0;
}

/* Generate terms until we have at least n stored items */
static void generate_until(size_t n)
{
    ensure_seq_cap(n);

    while (seq_len < n) {
        if (good_k(cur_k))
            seq[seq_len++] = (seq_t)cur_k;
        ++cur_k;
    }
}

/* Public entry point used by the test harnesses */
seq_t a(idx_t n)
{
    if (n == 0)           /* sequence starts at 1; out‑of‑range */
        return 0;

    generate_until((size_t)n);
    return seq[(size_t)n - 1];
}
