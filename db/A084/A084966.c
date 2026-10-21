/*
 * A084966  Minimum numbers of squares needed to write n! as a sum of nonzero
 *          squares.
 * https://oeis.org/A084966
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   For n≥1 let n! = ∏p^e_p. a(n)=1 iff all e_p are even (n! is a
 *            perfect square). a(n)=2 iff every prime p≡3 (mod 4) has even e_p
 *            (n! is a sum of two squares); otherwise a(n)=3 unless n! has the
 *            form 4^a(8b+7), in which case a(n)=4
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The algorithm uses Legendre’s formula for e_p(n!) and elementary modular
   arithmetic.  It runs in O(π(n)+n), fast enough for n up to 10⁴. */

#include <stdlib.h>
#include <stdint.h>

static int *prime_list = NULL;   /* primes up to current maximum n */
static size_t prime_cnt  = 0;    /* number of primes stored */
static idx_t  max_n      = 0;    /* largest n for which primes were generated */

static void generate_primes(idx_t n)
{
    /* Sieve of Eratosthenes up to n */
    size_t limit = (size_t)n;
    unsigned char *sieve = (unsigned char*)malloc((limit + 1) * sizeof(unsigned char));
    if (!sieve) return;              /* allocation failed – let caller handle it */
    for (size_t i = 0; i <= limit; ++i) sieve[i] = 1;
    if (limit >= 0) sieve[0] = sieve[1] = 0;
    for (size_t i = 2; i * i <= limit; ++i)
        if (sieve[i])
            for (size_t j = i * i; j <= limit; j += i) sieve[j] = 0;

    /* count primes */
    size_t cnt = 0;
    for (size_t i = 2; i <= limit; ++i) if (sieve[i]) ++cnt;

    int *new_pr = (int*)malloc(cnt * sizeof(int));
    if (!new_pr) { free(sieve); return; }

    size_t idx = 0;
    for (size_t i = 2; i <= limit; ++i)
        if (sieve[i]) new_pr[idx++] = (int)i;

    free(sieve);
    free(prime_list);
    prime_list = new_pr;
    prime_cnt  = cnt;
    max_n      = n;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;  /* undefined, return 0 for safety */

    /* Grow prime table if needed */
    if (n > max_n) generate_primes(n);
    if (!prime_list) return (seq_t)4;   /* fallback on allocation failure */

    /* Compute exponent of 2 in n! (v2). */
    idx_t v2 = 0;
    idx_t two = 2;
    while (two <= n) {
        v2 += n / two;
        if (two > n / 2) break;     /* avoid overflow of two *= 2 */
        two *= 2;
    }

    /* Compute odd part of n! modulo 8. */
    int odd_mod8 = 1;          /* stays in [0,7] */
    for (idx_t i = 1; i <= n; ++i) {
        idx_t x = i;
        while ((x & 1ULL) == 0) x >>= 1;   /* remove all factors of 2 */
        odd_mod8 = (odd_mod8 * (int)(x & 7ULL)) & 7;
    }

    /* Check if n! is a perfect square. */
    int square = 1;
    for (size_t j = 0; j < prime_cnt; ++j) {
        int p = prime_list[j];
        if ((idx_t)p > n) break;
        idx_t exp = 0;
        idx_t div = p;
        while (div <= n) {
            exp += n / div;
            if (div > n / p) break;
            div *= p;
        }
        if (exp & 1ULL) { square = 0; break; }
    }
    if (square) return (seq_t)1;

    /* Check Legendre's two–square theorem: all primes ≡3 (mod 4) must have even exponent. */
    int two_squares = 1;
    for (size_t j = 0; j < prime_cnt; ++j) {
        int p = prime_list[j];
        if ((idx_t)p > n) break;
        if (p % 4 == 3) {
            idx_t exp = 0;
            idx_t div = p;
            while (div <= n) {
                exp += n / div;
                if (div > n / p) break;
                div *= p;
            }
            if (exp & 1ULL) { two_squares = 0; break; }
        }
    }
    if (two_squares) return (seq_t)2;

    /* Check Legendre's three–square theorem. */
    int three_squares = 1;
    if ((v2 & 1ULL) == 0 && odd_mod8 == 7)
        three_squares = 0;      /* n! is of the form 4^a(8b+7) – cannot be 3 squares */

    if (three_squares) return (seq_t)3;
    return (seq_t)4;
}
