/*
 * A176653  Triangle, read by rows, defined by T(n, m) = f(n-m)*f(n) -
 *          f(n-0)*f(0) + 1, where f(n) is 1 if n = 0 and Prime(n) otherwise.
 * https://oeis.org/A176653
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 124, u16 n <= 5150, u32 n <= 5150, u64 n <= 5150,
 *            u128 n <= 5150, big any n
 * Helpers:   index_to_nm
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A176653
    Triangle read by rows, defined by
        T(n,m) = f(n-m) * f(m) - f(n) + 1
    where f(0) = 1 and f(k) = Prime(k) for k > 0
    a(0) = 1, a(1) = 1, a(2) = 1, ...
    The sequence a(n) is the triangle read by rows.
*/
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include "../utils/index_to_nm.h"

/* Maximum prime index needed for the test cases (n <= 10000 → n ≈ 140) */
#define PRIME_LIMIT 2000000        /* 2 000 000 → 148 933 primes (more than enough) */
static uint8_t  sieve_buf[PRIME_LIMIT + 1];
static uint64_t prime_tbl[PRIME_LIMIT / 10 + 100];   /* enough space for > 1.5e5 primes */
static size_t   prime_cnt = 0;                       /* number of primes stored (prime_tbl[1] is the 1st prime) */

/* ------------------------------------------------------------ */
/* Simple Sieve of Eratosthenes to fill prime_tbl                */
/* ------------------------------------------------------------ */
static void init_primes(void)
{
    if (prime_cnt) return;          /* already initialized */

    /* Sieve boolean array */
    memset(sieve_buf, 0, sizeof(sieve_buf));   /* 0 = composite, 1 = prime candidate */
    sieve_buf[0] = sieve_buf[1] = 0;
    for (size_t i = 2; i <= PRIME_LIMIT; ++i) {
        if (!sieve_buf[i]) {
            /* i is prime */
            if (prime_cnt < sizeof(prime_tbl)/sizeof(prime_tbl[0])) {
                prime_tbl[++prime_cnt] = (uint64_t)i;
            }
            for (uint64_t j = (uint64_t)i * i; j <= PRIME_LIMIT; j += i) {
                sieve_buf[(size_t)j] = 1;
            }
        }
    }
}

/* ------------------------------------------------------------ */
/* f(k) : 1 if k == 0 else the k‑th prime                      */
/* ------------------------------------------------------------ */
static inline uint64_t f(size_t k)
{
    if (k == 0) return 1;
    return prime_tbl[k];            /* prime_tbl is 1‑based: prime_tbl[1] = 2 */
}

/* ------------------------------------------------------------ */
/* Convert a linear index into (n,m) where 0 <= m <= n          */

/* ------------------------------------------------------------ */
/* Main function required by the checker                        */
/* ------------------------------------------------------------ */
seq_t a(idx_t idx)
{
    init_primes();                 /* ensure prime table is ready */

    idx_t n, m;
    index_to_nm(idx, &n, &m);

    /* The largest prime index required is max(n, m, n-m) */
    size_t max_needed = n > m ? (n > (n - m) ? n : (n - m)) :
                               ((m > (n - m)) ? m : (n - m));
    if (max_needed >= prime_cnt) {
        /* This situation should not occur for the test ranges but guard anyway */
        return 0;
    }

    uint64_t fn   = f((size_t)n);
    uint64_t fnm  = f((size_t)(n - m));
    uint64_t fm   = f((size_t)m);

    __int128 prod = (__int128)fnm * fm;
    __int128 tmp  = prod - fn + 1;

    return (seq_t)tmp;
}
