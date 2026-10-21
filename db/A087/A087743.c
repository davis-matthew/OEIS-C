/*
 * A087743  Numbers m >= 3 with property that the remainder when m is divided
 *          by k (for 3 <= k <= m-2) is not 1.
 * https://oeis.org/A087743
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3, 4, 5, and for n ≥ 4, a(n) = p_{n-1} + 1 where p_k is
 *            the k‑th prime (p_1 = 2). Equivalently, a(n) are the numbers m ≥
 *            3 such that m−1 is either prime or 4
 * Range:     u8 n <= 55, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* ------------------------------------------------------------------ */
/*  Simple incremental prime list                                     */
/* ------------------------------------------------------------------ */
static uint64_t *s_primes = NULL;      /* array of primes                */
static size_t   s_prime_cnt = 0;       /* number of stored primes         */
static size_t   s_prime_cap = 0;       /* allocated capacity              */
static uint64_t s_next_candidate = 3;  /* next odd number to test         */

static void ensure_prime_count(size_t need)
{
    if (s_primes == NULL) {                 /* first-time init              */
        s_prime_cap = 16;
        s_primes = (uint64_t *)malloc(s_prime_cap * sizeof(uint64_t));
        if (!s_primes) exit(1);
        s_prime_cnt = 1;
        s_primes[0] = 2;                    /* the only even prime           */
    }

    /* Grow the prime list until we have at least 'need' primes. */
    while (s_prime_cnt < need) {
        uint64_t cand = s_next_candidate;
        bool is_prime = true;

        /* Trial division by known primes. */
        for (size_t i = 0; i < s_prime_cnt; ++i) {
            uint64_t p = s_primes[i];
            if (p > cand / p) break;         /* no need to test larger primes   */
            if (cand % p == 0) { is_prime = false; break; }
        }

        if (is_prime) {
            if (s_prime_cnt == s_prime_cap) {
                s_prime_cap <<= 1;
                uint64_t *tmp = (uint64_t *)realloc(s_primes,
                                                     s_prime_cap * sizeof(uint64_t));
                if (!tmp) { free(s_primes); exit(1); }
                s_primes = tmp;
            }
            s_primes[s_prime_cnt++] = cand;
        }
        s_next_candidate += 2;                /* skip even numbers               */
    }
}

/* ------------------------------------------------------------------ */
/*  OEIS A087743: numbers m ≥ 3 with no remainder 1 when divided by
 *  any k (3 ≤ k ≤ m−2).                                                    */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;          /* undefined for n < 1            */

    /* First two terms follow p_{n-1}+1 directly. */
    if (n == 1) return (seq_t)3;         /* m = 3  (m-1 = 2 prime)        */
    if (n == 2) return (seq_t)4;         /* m = 4  (m-1 = 3 prime)        */

    /* Special case: a(3) = 5 (m-1 = 4, a power of two). */
    if (n == 3) return (seq_t)5;

    /* For n ≥ 4, a(n) = p_{n-1} + 1. */
    size_t required_primes = (size_t)(n - 1);   /* need primes[0 .. n-2]   */
    ensure_prime_count(required_primes);

    /* primes[0] = p_1 = 2; thus primes[n-2] = p_{n-1}. */
    return (seq_t)(s_primes[n - 2] + 1);
}
