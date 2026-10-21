/*
 * A066120  Superduperprimorials: product of first n terms in Chernoff
 *          sequence.
 * https://oeis.org/A066120
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   * a(n) = Product_{i=1..n} prime(i)^((n-i+1)*(n-i+2)/2) * =
 *            a(n-1) * A006939(n), a(0)=1
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 5, u128 n <= 6,
 *            big any n
 * Helpers:   ensure_seq_cap
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/ensure_seq_cap.h"

/* -------------------------------------------------------------------- */
/*   global state (cached values)                                      */
/* -------------------------------------------------------------------- */
static seq_t *seq_a      = NULL;    /* a(n) values                */
static seq_t *prim_arr   = NULL;    /* primorials P_i = ∏_{k=1..i} prime(k)    */
static seq_t *pval_arr   = NULL;    /* P_i values for A006939  */
static size_t seq_cap    = 0, prim_cap = 0, pval_cap = 0;
static size_t seq_len    = 0;        /* number of terms computed so far */

static uint32_t *primes    = NULL;   /* list of primes                */
static size_t   prime_cnt  = 0;
static size_t   prime_cap  = 0;
#define PRIME_LIMIT 200000            /* enough for n up to 10000 */

/* -------------------------------------------------------------------- */
/*  dynamic array helpers                                              */

static void ensure_prime_cap(size_t need)
{
    if (prime_cap < need) {
        size_t new_cap = (prime_cap == 0) ? 128 : (prime_cap << 1);
        if (new_cap < need) new_cap = need;
        primes = (uint32_t *)realloc(primes, new_cap * sizeof(uint32_t));
        prime_cap = new_cap;
    }
}

/* -------------------------------------------------------------------- */
/*  sieve first `n` primes (simple sieve)                              */
/* -------------------------------------------------------------------- */
static void init_primes(void)
{
    static bool done = false;
    if (done) return;
    done = true;

    uint8_t *sieve = (uint8_t *)calloc(PRIME_LIMIT + 1, 1);
    if (!sieve) exit(EXIT_FAILURE);

    for (uint32_t i = 2; i <= PRIME_LIMIT; ++i) {
        if (!sieve[i]) {           /* i is prime */
            ensure_prime_cap(prime_cnt + 1);
            primes[prime_cnt++] = i;
            if ((uint64_t)i * i <= PRIME_LIMIT) {
                for (uint32_t j = i * i; j <= PRIME_LIMIT; j += i)
                    sieve[j] = 1;
            }
        }
    }
    free(sieve);
}

/* -------------------------------------------------------------------- */
/*  compute all terms up to n (inclusive)                              */
/* -------------------------------------------------------------------- */
static void compute_up_to(idx_t n)
{
    if ((idx_t)seq_len > n) return;   /* already computed */

    init_primes();                    /* ensure prime list */

    /* ensure our arrays are large enough */
    ensure_seq_cap(&seq_a,   &seq_cap,   (size_t)n + 1);
    ensure_seq_cap(&prim_arr,&prim_cap,  (size_t)n + 1);
    ensure_seq_cap(&pval_arr,&pval_cap,  (size_t)n + 1);

    if (seq_len == 0) {               /* first time initialization */
        seq_a[0]     = (seq_t)1;
        prim_arr[0]  = (seq_t)1;
        pval_arr[0]  = (seq_t)1;
        seq_len      = 1;
    }

    for (size_t i = seq_len; i <= (size_t)n; ++i) {
        /* prime_i is the i-th prime (1‑indexed) */
        uint32_t prime_i = primes[i - 1];

        prim_arr[i] = prim_arr[i - 1] * (seq_t)prime_i;   /* ∏ primes[1..i] */
        pval_arr[i] = pval_arr[i - 1] * prim_arr[i];     /* A006939(i) */
        seq_a[i]    = seq_a[i - 1] * pval_arr[i];       /* a(i) */
    }
    seq_len = (size_t)n + 1;
}

/* -------------------------------------------------------------------- */
/*  public entry point                                                 */
/* -------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    compute_up_to(n);
    return seq_a[n];
}
