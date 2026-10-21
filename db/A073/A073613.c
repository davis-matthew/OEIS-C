/*
 * A073613  Triangular numbers which are the sum of two squares.
 * https://oeis.org/A073613
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 46, u32 n <= 3675, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A073613:  Triangular numbers which are the sum of two squares.
   The implementation generates triangular numbers in increasing order and
   tests each one for the sum-of-two-squares property by prime factorization.
   The check uses the theorem that a positive integer x can be written as
   a sum of two squares iff every prime p ≡ 3 (mod 4) occurs with an even
   exponent in the prime factorization of x.  We factor the two coprime
   parts a and b that come from m(m+1)/2   (one of m, m+1 is even and is
   divided by 2) and require each part to satisfy the condition.  The
   function keeps a static cache of generated terms, making repeated
   calls fast and order‑independent. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Adjust this constant if you want the sieve to cover larger numbers. */
#define PRIME_SIEVE_LIMIT 1000000

static seq_t *seq = NULL;          /* buffer of sequence values          */
static idx_t  seq_len = 0;         /* number of values in seq            */
static idx_t  seq_cap = 0;         /* allocated capacity                 */
static idx_t  m_current = 0;       /* last triangular index processed    */

static int    *primes = NULL;      /* list of primes                      */
static size_t prime_cnt = 0;       /* number of generated primes           */

/* --------------------------------------------------------------------- */
/*  Prime sieve – creates a list of all primes up to PRIME_SIEVE_LIMIT  */
/* --------------------------------------------------------------------- */
static void init_primes(void)
{
    if (primes != NULL) return;    /* already done */

    size_t limit = PRIME_SIEVE_LIMIT;
    char *sieve = (char *)malloc(limit + 1);
    if (!sieve) return;            /* allocation failed */

    for (size_t i = 0; i <= limit; ++i) sieve[i] = 0;
    primes = (int *)malloc((limit / 10) * sizeof(int));
    if (!primes) {
        free(sieve);
        return;
    }

    for (size_t i = 2; i <= limit; ++i) {
        if (!sieve[i]) {
            primes[prime_cnt++] = (int)i;
            if (i > limit / i) continue;
            for (size_t j = i * i; j <= limit; j += i) sieve[j] = 1;
        }
    }
    free(sieve);
}

/* --------------------------------------------------------------------- */
/*  Test whether a 64‑bit integer x can be expressed as a sum of two squares.
    The function applies the prime‑factorization criterion described above.   */
/* --------------------------------------------------------------------- */
static bool is_sum_two_squares(uint64_t x)
{
    if (x == 0) return true;      /* 0 = 0^2 + 0^2, special case */

    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = (uint64_t)primes[i];
        if (p * p > x) break;
        if (x % p == 0) {
            int e = 0;
            while (x % p == 0) { x /= p; ++e; }
            if ((p % 4 == 3) && (e & 1)) return false;
        }
    }
    /* Remaining factor (if any) is prime */
    if (x > 1 && (x % 4 == 3)) return false;
    return true;
}

/* --------------------------------------------------------------------- */
/*  Ensure the sequence buffer can hold at least 'need' elements.        */
/* --------------------------------------------------------------------- */
static void ensure_seq_capacity(idx_t need)
{
    if (seq_cap >= need) return;
    idx_t new_cap = seq_cap ? seq_cap * 2 : 16;
    while (new_cap < need) new_cap <<= 1;
    seq = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
    seq_cap = new_cap;
}

/* --------------------------------------------------------------------- */
/*  Compute sequence entries until index 'n' becomes available.          */
/* --------------------------------------------------------------------- */
static void generate_up_to(idx_t n)
{
    init_primes();

    /* Initialise with the first term: 0 (m = 0) */
    if (seq_len == 0) {
        ensure_seq_capacity(1);
        seq[0] = (seq_t)0;
        seq_len = 1;
        m_current = 0;
    }

    while (seq_len < n) {
        ++m_current;                       /* next m value */
        seq_t T = ((seq_t)m_current) * ((seq_t)m_current + 1) / 2;

        /* Split into two coprime factors a and b (one of m,m+1 is even) */
        uint64_t a = m_current;
        uint64_t b = m_current + 1;
        if (a % 2 == 0) a /= 2;
        else if (b % 2 == 0) b /= 2;

        if (is_sum_two_squares(a) && is_sum_two_squares(b)) {
            ensure_seq_capacity(seq_len + 1);
            seq[seq_len++] = T;
        }
    }
}

/* --------------------------------------------------------------------- */
/*  Public function required by the specification.                       */
/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* out of bounds, return 0 */
    generate_up_to(n);
    return seq[n - 1];
}
