/*
 * A094048  Let p(n) be the n-th prime congruent to 1 mod 4. Then a(n) = the
 *          least m for which m^2+1=p(n)*k^2 has a solution.
 * https://oeis.org/A094048
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) : least m > 0 such that m²+1 = p(n)·k² where p(n) is the
 *            n‑th prime congruent to 1 (mod 4). The value is obtained as the
 *            minimal solution (x, y) of the negative Pell equation x²−p y² =
 *            −1
 * Range:     u8 n <= 7, u16 n <= 12, u32 n <= 26, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   floor_sqrt, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime.h"
#include "../utils/floor_sqrt.h"

/* --------------------------------------------------------------------- */
/* Helper: multiply two seq_t values in the smallest type that can hold
   the product.  If the compiler provides an unsigned 128‑bit integer
   the multiplication is performed in that wider type; otherwise
   seq_t multiplication is used.  The result is then cast back to
   seq_t.  This avoids overflow for all types that fit into a
   128‑bit integer. */
static inline seq_t mul128(seq_t a, seq_t b)
{
#if defined(__SIZEOF_INT128__)
    return (seq_t)(((unsigned __int128)a) * ((unsigned __int128)b));
#else
    return a * b;
#endif
}

/* --------------------------------------------------------------------- */
/* Cache of primes congruent to 1 (mod 4).  The array holds 64‑bit
   primes because we need to test primality on 64‑bit numbers
   although the returned sequence value can have any width.    */
static uint64_t *primes1 = NULL;
static idx_t    primes_cnt = 0;
static idx_t    primes_cap = 0;
static idx_t    next_candidate = 5;  /* first 1 mod 4 number to test */

/* Ensure the cache contains at least n primes ≡1 (mod 4).            */
static void ensure_primes1mod4(idx_t n)
{
    if (n <= primes_cnt) return;

    while (primes_cnt < n) {
        if (is_prime(next_candidate)) {
            if (primes_cnt >= primes_cap) {
                primes_cap = primes_cap ? primes_cap * 2 : 16;
                uint64_t *tmp = (uint64_t *)realloc(primes1, primes_cap * sizeof(uint64_t));
                if (!tmp) abort();
                primes1 = tmp;
            }
            primes1[primes_cnt++] = next_candidate;
        }
        next_candidate += 4;
    }
}

/* --------------------------------------------------------------------- */
/* Solve x²−D·y² = −1 for square‑free D.  Uses the standard continued
   fraction algorithm for √D.  The minimal positive x satisfying
   x²+1 = D·y² is returned.                                            */
static seq_t negative_pell_solution(seq_t D)
{
    seq_t a0 = floor_sqrt(D);
    seq_t m = 0, d = 1, a = a0;
    seq_t p_prev2 = 0, p_prev1 = 1;
    seq_t q_prev2 = 1, q_prev1 = 0;

    for (;;) {
        seq_t p = mul128(a, p_prev1) + p_prev2;
        seq_t q = mul128(a, q_prev1) + q_prev2;

        seq_t lhs = mul128(p, p);
        lhs = lhs + (seq_t)1;                 /* p² + 1 */
        seq_t rhs = mul128(D, mul128(q, q));  /* D·q² */

        if (lhs == rhs) return p;

        /* Update for next step of continued fraction */
        p_prev2 = p_prev1;  p_prev1 = p;
        q_prev2 = q_prev1;  q_prev1 = q;

        /* Recurrence for √D */
        m = d * a - m;
        d = (D - mul128(m, m)) / d;
        a = (a0 + m) / d;
    }
}

/* --------------------------------------------------------------------- */
/* Public sequence function                                              */
seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* undefined for n < 1   */

    ensure_primes1mod4(n);
    uint64_t p = primes1[n - 1];  /* p(n) */
    return negative_pell_solution((seq_t)p);
}
