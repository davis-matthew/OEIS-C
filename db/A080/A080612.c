/*
 * A080612  Numbers m such that (1/p(2*m+1)) * Sum_{k=1..m} p(2*k+1)-p(2*k) >=
 *          (1/p(2*m))* Sum_{k=1..m} p(2*k)-p(2*k-1) where p(k) denotes the
 *          k-th prime.
 * https://oeis.org/A080612
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 31, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A080612:  m is accepted if  
   (1/p(2*m+1)) * Σ_{k=1..m} (p(2k+1)-p(2k))  ≥  (1/p(2*m)) * Σ_{k=1..m} (p(2k)-p(2k-1))   */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/*------------------------------------------------------------------*/
/*  Prime generation – simple incremental sieve                     */
/*------------------------------------------------------------------*/
#define PRIME_CAP 200000  /* large enough for the limits of the problem */

static uint32_t primes[PRIME_CAP];
static int       prime_cnt = 0;

/* ensure that at least k primes are available (k >= 1)          */
static void ensure_prime(idx_t k)
{
    while (prime_cnt < k) {
        uint32_t cand = prime_cnt ? primes[prime_cnt - 1] + 1 : 2;
        bool ok = false;
        while (!ok) {
            ok = true;
            for (int i = 0; i < prime_cnt; ++i) {
                uint32_t p = primes[i];
                if ((uint64_t)p * p > cand)
                    break;
                if (cand % p == 0) { ok = false; break; }
            }
            if (!ok)
                ++cand;
        }
        if (prime_cnt >= PRIME_CAP) {
            /* should not happen for the required n (≤ 10000)          */
            /* abort silently – the build will never reach this point */
            return;
        }
        primes[prime_cnt++] = cand;
    }
}

/* safe access to the k‑th prime (k ≥ 1)                           */
static inline seq_t get_prime(idx_t k)
{
    return (seq_t)primes[(size_t)k - 1];
}

/*------------------------------------------------------------------*/
/*  Sequence storage                                               */
/*------------------------------------------------------------------*/
static seq_t *a_terms = NULL;   /* cumulative list of accepted m values */
static size_t a_size = 0;        /* number of elements stored           */
static size_t a_cap  = 0;        /* allocated capacity                  */

static void append_term(seq_t val)
{
    if (a_size == a_cap) {
        size_t new_cap = a_cap ? a_cap << 1 : 4;
        seq_t *tmp = (seq_t *)realloc(a_terms, new_cap * sizeof(seq_t));
        a_terms = tmp;
        a_cap   = new_cap;
    }
    a_terms[a_size++] = val;
}

/*------------------------------------------------------------------*/
/*  Global computation state                                       */
/*------------------------------------------------------------------*/
static uint64_t last_m  = 0;   /* largest m processed so far             */
static seq_t    sumOdd  = 0;   /* Σ (p(2k+1)-p(2k)) for current m        */
static seq_t    sumEven = 0;   /* Σ (p(2k)-p(2k-1))   for current m        */

/*------------------------------------------------------------------*/
/*  Public function: a(n)                                          */
/*------------------------------------------------------------------*/
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;  /* out of bounds – undefined but harmless */

    /* Generate terms until we have at least n stored          */
    while (a_size < n) {
        uint64_t m = last_m + 1;

        idx_t idx2m  = 2 * m;       /* 2m      */
        idx_t idx2m0 = 2 * m - 1;   /* 2m-1    */
        idx_t idx2m1 = 2 * m + 1;   /* 2m+1    */

        ensure_prime(idx2m1);  /* guarantees that primes up to 2m+1 exist */

        seq_t p_2m  = get_prime(idx2m);
        seq_t p_2m1 = get_prime(idx2m1);
        seq_t p_2m0 = get_prime(idx2m0);

        seq_t diffOdd  = p_2m1 - p_2m;
        seq_t diffEven = p_2m - p_2m0;

        sumOdd  += diffOdd;
        sumEven += diffEven;

        /* inequality: sumOdd / p(2m+1) ≥ sumEven / p(2m)  ⇔
           sumOdd * p(2m)  ≥  sumEven * p(2m+1)                      */
        seq_t left  = sumOdd * p_2m;   /* uses seq_t operators only   */
        seq_t right = sumEven * p_2m1;

        if (left >= right)
            append_term((seq_t)m);     /* store m as seq_t                */

        last_m = m;
    }

    return a_terms[n - 1];
}
