/*
 * A191999  Denominators of partial products of a Hardy-Littlewood constant.
 * https://oeis.org/A191999
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, frac
 * Formula:   a(n) = denominator of Π_{j=1..n} (1 - χ_2(4; p_j)/(p_j-1)),
 *            where p_j is the j‑th prime and χ_2(4; k) is 0 for k=2, 1 if k≡1
 *            (mod 4), -1 if k≡3 (mod 4)
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 15, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <math.h>

/* Forward declarations for seq_t operations (should be defined by the
   build system).  They are assumed to work with unsigned arithmetic. */
static inline seq_t seq_gcd(seq_t a, seq_t b)
{
    while (b != (seq_t)0) {
        seq_t r = a % b;
        a = b;
        b = r;
    }
    return a;
}

/* Static storage for generated primes. */
static uint32_t  *primes_buf   = NULL;
static size_t     primes_cnt   = 0;
static idx_t      primes_limit = 0;

/* Ensure that at least n primes are available.  We sieve up to an
   upper bound for the n‑th prime using the well known estimate:
     p_n < n (log n + log log n)  for n ≥ 6
   (Rosser–Schoenfeld).  For small n we use a fixed upper bound. */
static void
ensure_primes(idx_t n)
{
    if (n <= primes_cnt)
        return;                         /* already enough */

    idx_t limit;
    if (n < 6)
        limit = 15;                      /* prime number 5 */
    else {
        double dn = (double)n;
        double est = dn * (log(dn) + log(log(dn)));
        limit = (idx_t)(est + 3.0);      /* a safe margin */
    }

    /* Allocate a boolean sieve array. */
    unsigned char *is_comp = (unsigned char*)malloc(limit + 1);
    if (!is_comp) { exit(1); }
    for (idx_t i = 0; i <= limit; ++i)
        is_comp[i] = 0;

    /* Sieve of Eratosthenes */
    for (idx_t i = 2; i * i <= limit; ++i) {
        if (!is_comp[i]) {
            for (idx_t j = i * i; j <= limit; j += i)
                is_comp[j] = 1;
        }
    }

    /* Count primes in the range. */
    size_t new_cnt = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (!is_comp[i])
            ++new_cnt;

    /* Allocate new prime array. */
    uint32_t *new_buf = (uint32_t*)malloc(new_cnt * sizeof(uint32_t));
    if (!new_buf) { free(is_comp); exit(1); }
    size_t pos = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (!is_comp[i])
            new_buf[pos++] = (uint32_t)i;
    free(is_comp);

    /* Replace old buffer. */
    free(primes_buf);
    primes_buf = new_buf;
    primes_cnt = new_cnt;
    primes_limit = limit;
}

/* -------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0)            /* offset is 1; 0 is unused but handled. */
        return 0;

    /* Make sure we have enough primes. */
    ensure_primes(n);

    seq_t num = 1;
    seq_t den = 1;

    for (idx_t i = 0; i < n; ++i) {
        uint32_t p = primes_buf[i];          /* j-th prime */

        /* Dirichlet character modulo 4. */
        int chi;
        if (p == 2)
            chi = 0;
        else if (p % 4 == 1)
            chi = 1;
        else
            chi = -1;

        seq_t factor_den = (seq_t)(p - 1);
        seq_t factor_num = factor_den - (seq_t)chi;  /* (p-1)-χ(p) */

        num *= factor_num;
        den *= factor_den;

        seq_t g = seq_gcd(num, den);
        if (g != (seq_t)1) {
            num /= g;
            den /= g;
        }
    }

    return den;
}
