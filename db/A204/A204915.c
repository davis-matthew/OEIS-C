/*
 * A204915  Least k such that n divides A204914(k), the k-th difference of two
 *          squared primes.
 * https://oeis.org/A204915
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 60, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   mod_square, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A204915(a(n)) = least k such that n divides A204914(k),
   where A204914(k) is the k‑th difference of two squared primes.
   The values of A204914(k) are obtained by enumerating all pairs
   (p,q) of distinct primes with p>q in increasing order of p
   and then of q:   k=1 → (3,2), k=2 → (5,2), k=3 → (5,3),
   k=4 → (7,2), ….
   For a fixed n we generate these differences sequentially until
   one is divisible by n and return the corresponding index k. */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/next_prime.h"
#include "../utils/mod_square.h"

/* ------------------------------------------------------------ */
/*  Prime sequence generation, cached across calls               */
/* ------------------------------------------------------------ */
static uint64_t *prime_list   = NULL;   /* list of primes, prime_list[0] = 2   */
static size_t   prime_cap    = 0;
static size_t   prime_cnt    = 0;

static void init_primes(void)
{
    if (prime_list != NULL) return;          /* already initialised   */
    prime_cap = 16;
    prime_list = (uint64_t *)malloc(prime_cap * sizeof(uint64_t));
    prime_cnt = 0;
    prime_list[prime_cnt++] = 2;             /* first prime          */
    prime_list[prime_cnt++] = 3;             /* second prime         */
}

static void grow_prime_list(void)
{
    if (prime_cnt < prime_cap) return;
    prime_cap <<= 1;
    prime_list = (uint64_t *)realloc(prime_list,
                                     prime_cap * sizeof(uint64_t));
}

/* append one new prime after the current last prime                */
static void add_next_prime(void)
{
    uint64_t next = next_prime(prime_list[prime_cnt - 1] + 1);
    grow_prime_list();
    prime_list[prime_cnt++] = next;
}

/* ------------------------------------------------------------ */
/*  Helper: modular square: (x^2) mod m                         */

/* ------------------------------------------------------------ */
/*  Main sequence function                                       */
/* ------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;          /* undefined for n < 1   */

    init_primes();                       /* ensure prime array    */

    /* indices into prime_list :
        p_index = index of the larger prime
        q_index = index of the smaller prime   */
    size_t p_index = 1;                  /* start with pair (3,2)*/
    size_t q_index = 0;

    uint64_t k = 0;                       /* running index           */
    uint64_t mod_n = (uint64_t)n;        /* use 64‑bit for mod ops */

    while (true) {
        /* make sure the larger prime exists */
        if (p_index >= prime_cnt) add_next_prime();

        uint64_t p = prime_list[p_index];
        uint64_t q = prime_list[q_index];

        uint64_t pm = mod_square(p % mod_n, mod_n);
        uint64_t qm = mod_square(q % mod_n, mod_n);
        uint64_t diff_mod = (pm >= qm) ? (pm - qm) : (pm + mod_n - qm);

        if (diff_mod == 0) {
            return (seq_t)k + 1;        /* k is 0‑based, return 1‑based index   */
        }

        k++;  /* advanced past this pair */

        /* advance to next pair in the enumeration order   */
        if (q_index + 1 < p_index) {
            ++q_index;
        } else {
            ++p_index;
            q_index = 0;
        }
    }
}
