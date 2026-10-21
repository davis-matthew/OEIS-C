/*
 * A081231  Let p = n-th prime of the form 4k+3, take smallest solution (x,y)
 *          to the Pellian equation x^2 - p*y^2 = 1 with x and y >= 1;
 *          sequence gives value of x.
 * https://oeis.org/A081231
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) is the x-coordinate of the fundamental solution (x,y) to x²
 *            - p·y² = 1 where p is the n‑th prime congruent to 3 (mod 4)
 * Range:     u8 n <= 5, u16 n <= 13, u32 n <= 23, u64 n <= 54, u128 n <= 144,
 *            big any n
 * Helpers:   _floor_isqrt_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/_floor_isqrt_u64.h"

/* ---------- Cache of primes ≡ 3 (mod 4) ---------- */
static bool primes_ready = false;
static uint64_t *primes3 = NULL;
static size_t   primes3_cnt = 0;
static size_t   sieve_limit = 0;

/* ---------- Ensure we have at least n such primes ---------- */
static void ensure_primes3(idx_t n)
{
    if (primes_ready && primes3_cnt >= (size_t)n)
        return;

    size_t limit = sieve_limit ? sieve_limit : 1000;
    while (primes3_cnt < (size_t)n) {
        limit <<= 1;               /* double the upper bound */

        bool *is_prime = (bool *)calloc(limit + 1, sizeof(bool));
        if (!is_prime) {
            /* Out of memory – return 0 so caller gets 0 (failure). */
            return;
        }
        for (size_t i = 2; i <= limit; ++i)
            is_prime[i] = true;

        for (size_t p = 2; p * p <= limit; ++p) {
            if (is_prime[p]) {
                for (size_t m = p * p; m <= limit; m += p)
                    is_prime[m] = false;
            }
        }

        /* Count primes of the required form to size the array. */
        size_t cnt = 0;
        for (size_t i = 2; i <= limit; ++i)
            if (is_prime[i] && (i & 3U) == 3U)
                ++cnt;

        /* Build the list. */
        uint64_t *tmp = (uint64_t *)malloc(cnt * sizeof(uint64_t));
        if (!tmp) {
            free(is_prime);
            return;
        }
        size_t idx = 0;
        for (size_t i = 2; i <= limit; ++i)
            if (is_prime[i] && (i & 3U) == 3U)
                tmp[idx++] = (uint64_t)i;

        free(is_prime);
        free(primes3);
        primes3 = tmp;
        primes3_cnt = cnt;
        sieve_limit = limit;
        primes_ready = true;
    }
}

/* ---------- Fundamental solution for x^2 - D y^2 = 1 ---------- */
static seq_t fundamental_x_for_prime(idx_t D)
{
    idx_t a0 = _floor_isqrt_u64(D);          /* integer part of sqrt(D) */

    /* Build the periodic part of the continued fraction. */
    idx_t m = 0, d = 1, a = a0;
    /* Allocate enough space for the period (worst case ~2*sqrt(D)). */
    idx_t *cf = (idx_t *)malloc(8192 * sizeof(idx_t));
    if (!cf)
        return 0;
    size_t cf_cap = 8192, cf_len = 0;

    /* First step: compute a1 and subsequent terms until the period ends. */
    do {
        m = d * a - m;
        d = (D - m * m) / d;
        a = (a0 + m) / d;
        if (cf_len == cf_cap) {
            cf_cap <<= 1;
            idx_t *tmp = (idx_t *)realloc(cf, cf_cap * sizeof(idx_t));
            if (!tmp) {
                free(cf);
                return 0;
            }
            cf = tmp;
        }
        cf[cf_len++] = a;
    } while (a != 2 * a0);                   /* period finished */

    size_t period = cf_len;
    size_t target = (period & 1U) ? (2 * period - 1) : (period - 1);

    /* Convergents: p_k = a_k * p_{k-1} + p_{k-2} */
    seq_t p_nm2 = 0, p_nm1 = 1;
    seq_t q_nm2 = 1, q_nm1 = 0;              /* q’s are not needed, but kept for symmetry */

    for (size_t k = 0; k <= target; ++k) {
        seq_t a_term =
            (k == 0) ? (seq_t)a0 : (seq_t)cf[(k - 1) % period];
        seq_t p = a_term * p_nm1 + p_nm2;
        seq_t q = a_term * q_nm1 + q_nm2;     /* q unused */

        if (k == target) {
            free(cf);
            return p;                       /* minimal x */
        }
        p_nm2 = p_nm1;  p_nm1 = p;
        q_nm2 = q_nm1;  q_nm1 = q;
    }

    free(cf);
    return 0; /* should never be reached */
}

/* ---------- Public sequence function ---------- */
seq_t a(idx_t n)
{
    if (n == 0U)
        return 0;

    ensure_primes3(n);
    if (primes3_cnt < (size_t)n)
        return 0;                 /* failed to generate enough primes */

    idx_t D = (idx_t)primes3[n - 1];
    return fundamental_x_for_prime(D);
}
