/*
 * A154258  Number of triples <p,s,t> such that p+F_s+(F_t)^2=n, where p is an
 *          odd prime, s and t are greater than one and the Fibonacci number
 *          F_s or F_t is odd.
 * https://oeis.org/A154258
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Number of triples <p,s,t> such that p + F_s + (F_t)^2 =
 *            n, with p odd prime, s,t > 1, and F_s or F_t is odd
 * Range:     u8 n <= 9880, u16 n <= 9880, u32 n <= 9880, u64 n <= 9880,
 *            u128 n <= 9880, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A154258  [see entry for definition]  */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>

/* --------- prime sieve (up to current limit) ---------------- */
static bool *prime_flag = NULL;   /* prime_flag[i] == true  <=>  i prime (i>=2) */
static idx_t prime_limit = 0;     /* largest n for which prime_flag is valid */

/* Extend sieve to size N+1, recomputes from scratch. */
static void sieve_primes(idx_t N)
{
    if (N <= prime_limit) return;

    bool *new_flag = (bool *)malloc((N + 1) * sizeof(bool));
    for (idx_t i = 0; i <= N; ++i) new_flag[i] = true;
    new_flag[0] = false;  new_flag[1] = false;

    for (idx_t i = 2; i * i <= N; ++i) {
        if (new_flag[i]) {
            for (idx_t j = i * i; j <= N; j += i)
                new_flag[j] = false;
        }
    }

    free(prime_flag);
    prime_flag = new_flag;
    prime_limit = N;
}

/* --------- Fibonacci table (values up to current limit) -------- */
static idx_t *fib = NULL;        /* fib[i] = F_i  (F_0=0, F_1=1) */
static size_t fib_len = 0;       /* number of Fibonacci numbers stored (>=2) */
static size_t fib_cap = 0;       /* allocated capacity */

/* Grow fib array so that it contains all Fibonacci numbers <= limit. */
static void ensure_fib(idx_t limit)
{
    if (fib == NULL) {          /* first call */
        fib_cap = 8;
        fib = (idx_t *)malloc(fib_cap * sizeof(idx_t));
        fib[0] = 0;
        fib[1] = 1;
        fib_len = 2;
    }

    while (true) {
        if (fib_len >= fib_cap) {
            fib_cap <<= 1;
            fib = (idx_t *)realloc(fib, fib_cap * sizeof(idx_t));
        }
        idx_t next = fib[fib_len - 1] + fib[fib_len - 2];
        if (next > limit) break;
        fib[fib_len++] = next;
    }
}

/* --------- sequence function ----------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* safe guard */

    ensure_fib(n);
    sieve_primes(n);

    seq_t count = 0;

    for (size_t t = 2; t < fib_len; ++t) {
        idx_t ft   = fib[t];
        idx_t ft_sq = ft * ft;
        if (ft_sq > n) break;        /* further t will only increase square */

        /* parity of F_t once per t */
        bool ft_odd = (ft & 1U) != 0;

        for (size_t s = 2; s < fib_len; ++s) {
            idx_t fs = fib[s];

            idx_t sum = ft_sq + fs;
            if (sum > n) break;   /* beyond n, no need to continue s-loop */

            idx_t rem = n - sum;   /* candidate prime p */

            if (rem < 3) break;           /* no smaller primes possible */

            if ((rem & 1U) == 0) continue;/* p must be odd */

            if (!prime_flag[rem]) continue;/* prime check */

            bool fs_odd = (fs & 1U) != 0;
            if (!(ft_odd || fs_odd)) continue;  /* at least one odd Fibonacci */

            count += 1;
        }
    }
    return count;
}
