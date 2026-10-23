/*
 * A046730  a(n) = A002172(n) / 2.
 * https://oeis.org/A046730
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 954, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A046730
   a(n) = the integer a (possibly negative) in the representation
   p = a² + b² of the n‑th prime p ≡ 1 (mod 4), where a is odd and
   a ≡ b + 1 (mod 4). */

/*--- standard headers ----------------------------------------------------*/
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/*--- small integer square root for 64‑bit unsigned integers --------------*/
static uint64_t isqrt_u64(uint64_t n)
{
    uint64_t lo = 0, hi = (uint64_t)1 << 32, mid, sq;
    while (lo <= hi)
    {
        mid = (lo + hi) >> 1;
        sq  = mid * mid;
        if (sq <= n)
        {
            lo = mid + 1;
        }
        else
        {
            hi = mid - 1;
        }
    }
    return lo - 1;
}

/*--- compute the a‑value for a single prime p ≡ 1 (mod 4) ----------------*/
static seq_t compute_a_for_prime(uint64_t p)
{
    uint64_t lim = isqrt_u64(p);
    for (uint64_t a = 1; a <= lim; a += 2)          /* a odd            */
    {
        uint64_t a2 = a * a;
        if (a2 > p) break;
        uint64_t b2 = p - a2;
        if (b2 == 0) continue;
        uint64_t b = isqrt_u64(b2);
        if (b * b != b2) continue;                 /* not a square     */

        seq_t pos = (seq_t)a;                      /* positive a       */
        seq_t neg = -(seq_t)a;                     /* negative a       */
        seq_t bplus = (seq_t)(b + 1);              /* b + 1            */

        if ((pos - bplus) % 4 == 0) return pos;    /* check condition  */
        if ((neg - bplus) % 4 == 0) return neg;
    }
    return (seq_t)0; /* should never reach here */
}

/*--- data structures for caching -----------------------------------------*/
static seq_t *terms       = NULL;     /* a(1), a(2), …                 */
static size_t terms_cnt   = 0;
static size_t terms_cap   = 0;

static uint64_t *prime_vec = NULL;     /* all primes found so far       */
static size_t    prime_cnt   = 0;
static size_t    prime_cap   = 0;

static uint64_t next_candidate = 3;    /* next integer to test for prime */

/*--- primality test using cached primes -----------------------------------*/
static bool is_prime_u64(uint64_t m)
{
    if (m < 2) return false;
    if (m == 2) return true;
    if ((m & 1) == 0) return false;
    uint64_t sqrt_m = isqrt_u64(m);
    for (size_t i = 0; i < prime_cnt && prime_vec[i] <= sqrt_m; ++i)
    {
        if (m % prime_vec[i] == 0) return false;
    }
    return true;
}

/*--- ensure cache contains at least n terms --------------------------------*/
static void ensure_terms(uint64_t n)
{
    if (terms_cnt >= n) return;

    /* initialize prime vector with 2 if needed */
    if (prime_vec == NULL)
    {
        prime_cap   = 16;
        prime_vec   = (uint64_t *)malloc(prime_cap * sizeof(uint64_t));
        if (!prime_vec) exit(EXIT_FAILURE);
        prime_vec[prime_cnt++] = 2;
    }

    while (terms_cnt < n)
    {
        uint64_t m;
        /* find next prime */
        do {
            m = next_candidate++;
        } while (!is_prime_u64(m));

        /* store prime */
        if (prime_cnt == prime_cap)
        {
            size_t new_cap = prime_cap * 2;
            uint64_t *tmp = (uint64_t *)realloc(prime_vec, new_cap * sizeof(uint64_t));
            if (!tmp) exit(EXIT_FAILURE);
            prime_vec = tmp;
            prime_cap = new_cap;
        }
        prime_vec[prime_cnt++] = m;

        /* if p ≡ 1 (mod 4) compute its a‑term */
        if ((m & 3) == 1)
        {
            seq_t val = compute_a_for_prime(m);
            if (terms_cnt == terms_cap)
            {
                size_t new_cap = terms_cap ? terms_cap * 2 : 32;
                seq_t *tmp = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
                if (!tmp) exit(EXIT_FAILURE);
                terms = tmp;
                terms_cap = new_cap;
            }
            terms[terms_cnt++] = val;
        }
    }
}

/*--- public function -----------------------------------------------------*/
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* offset is 1 */
    ensure_terms(n);
    return terms[n - 1];
}
