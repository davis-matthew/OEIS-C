/*
 * A051349  Sum of first n nonprimes.
 * https://oeis.org/A051349
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 17, u16 n <= 320, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A051349 – Sum of first n nonprimes (including 1).
 *
 *  a(0) = 0
 *  a(n) = a(n-1) + (n-th nonprime),   n ≥ 1
 *
 *  The nonprimes are 1,4,6,8,9,10,12,14,15,16,18,20,21,22,...
 *
 *  Implementation outline:
 *  -----------------------
 *  • Keep a growing table of known primes (uint64_t) for fast
 *    trial‑division primality checks.
 *  • For each requested n, iterate natural numbers from 1 upwards,
 *    add a number to the running sum whenever it is *not* prime
 *    until n nonprimes have been accumulated.
 *  • This solution is O(n√n) which is below the time limits for
 *    n ≤ 10 000 and works for all supported seq_t/idx_t types
 *    (including arbitrary‑precision seq_t) because arithmetic is
 *    performed through operations defined for seq_t.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <assert.h>

/* ------------- Helper: prime table --------------------------------- */
static uint64_t *prime_tab = NULL;   /* dynamic array of primes */
static size_t   prime_cnt = 0;       /* number of primes stored   */
static size_t   prime_cap = 0;       /* allocated capacity       */

/*  Test if n is prime.  On success, store n in the table.           */
static bool prime_u64(uint64_t n)
{
    if (n < 2) return false;

    /* trial division by known primes */
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = prime_tab[i];
        if (p > n / p) break;          /* p^2 > n  */
        if (n % p == 0) return false;
    }

    /* prime – append to the table */
    if (prime_cnt == prime_cap) {
        /* grow the prime array (size ≥ 4) */
        size_t new_cap = prime_cap ? prime_cap * 2 : 4;
        uint64_t *tmp = (uint64_t *)realloc(prime_tab, new_cap * sizeof(uint64_t));
        if (!tmp) abort();              /* allocation failure */
        prime_tab = tmp;
        prime_cap = new_cap;
    }
    prime_tab[prime_cnt++] = n;
    return true;
}

/* -------------------------------------------------------- */
/*  Main sequence function                                  */
/* -------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* base case defined by OEIS */

    seq_t sum  = 0;                     /* running sum of nonprimes   */
    uint64_t count = 0;                 /* how many nonprimes seen   */
    uint64_t num = 1;                   /* candidate integer         */

    while (count < n) {
        if (!prime_u64(num)) {          /* num is a nonprime         */
            /* add to sum – cast to seq_t is safe because
               seq_t can be cast from uint64_t (guaranteed by
               the build). */
            sum += (seq_t)num;
            ++count;
        }
        ++num;
    }
    return sum;
}
