/*
 * A075471  Rounded root-mean-square (RMS) value of first n primes.
 * https://oeis.org/A075471
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = round( sqrt( (1/n) * Σ_{k=1}^n p_k^2 ) ) where p_k is the
 *            k‑th prime (p_1 = 2). The rounding is half‑up
 * Range:     u8 n <= 5, u16 n <= 24, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   _isqrt_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/_isqrt_seq.h"

/* --- static tables of primes and cumulative sums of their squares --- */
static seq_t *primes   = NULL;   /* primes[1] .. primes[max_n]   */
static seq_t *sumSq    = NULL;   /* cumulative sums of squares  */
static idx_t  max_n    = 0;      /* highest prime stored          */
static size_t cap      = 0;      /* capacity of the allocated arrays */

/* Ensure at least `need` entries (index 0 unused). */
static void ensure_capacity(size_t need)
{
    if (cap >= need)
        return;
    size_t newcap = (cap == 0) ? 4 : cap * 2;
    if (newcap < need)
        newcap = need;
    primes = (seq_t *)realloc(primes, newcap * sizeof(seq_t));
    sumSq  = (seq_t *)realloc(sumSq,  newcap * sizeof(seq_t));
    cap    = newcap;
}

/* Expand the tables so that primes[1..n] and sumSq[1..n] are valid. */
static void generate_primes_up_to(idx_t n)
{
    if (n <= max_n)
        return;

    /* We need room for indices 0..n (index 0 unused). */
    ensure_capacity((size_t)n + 1);

    /* Seed the table with the first prime if it is empty. */
    if (max_n == 0) {
        primes[1] = 2;
        sumSq[1]  = (seq_t)4;            /* 2² */
        max_n = 1;
        if (n == 1)
            return;
    }

    /* Starting candidate ---------------------------------------------------------------- */
    seq_t candidate;
    if (max_n == 1)                      /* the second prime is 3. */
        candidate = 3;
    else
        candidate = primes[max_n] + 2;   /* next odd after the last prime */

    if ((candidate & 1) == 0)           /* ensure odd */
        ++candidate;

    /* -------------------------------------------------------------------- */
    for (;;) {
        /* trial division up to √candidate */
        bool isPrime = true;
        seq_t sqrtCand = _isqrt_seq(candidate);
        for (idx_t i = 1; i <= max_n; ++i) {
            seq_t p = primes[i];
            if (p > sqrtCand)
                break;
            if (candidate % p == 0) {
                isPrime = false;
                break;
            }
        }

        if (isPrime) {
            ++max_n;
            primes[max_n] = candidate;
            sumSq[max_n] = sumSq[max_n - 1] + (candidate * candidate);
            if (max_n == n)
                break;
        }
        candidate += 2;   /* skip even numbers */
    }
}

/* Rounded RMS of the first n primes.  For n < 1 the function returns 0. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    generate_primes_up_to(n);

    seq_t S = sumSq[n];              /* Σp_k² for k = 1..n */
    seq_t fourS = S * 4;             /* 4 * S used in the rounding test */

    /* k = floor( sqrt( S / n ) )  (floor of exact mean of squares) */
    seq_t Q = S / n;                  /* exact floor of mean of squares */
    seq_t k = _isqrt_seq(Q);

    /* Compute n * (2*k + 1)² = n * (4*k² + 4*k + 1) */
    seq_t rhs = n * ((k * k) * 4 + (k * 4) + 1);

    /* Round up if sqrt(S/n) >= k + 0.5   ⇔   4*S >= rhs */
    if (fourS >= rhs)
        ++k;

    return k;
}
