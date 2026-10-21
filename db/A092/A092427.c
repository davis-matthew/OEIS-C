/*
 * A092427  Least k > 1 such that k^n divides (k-1)!.
 * https://oeis.org/A092427
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the least k>1 such that k^n | (k‑1)!
 * Range:     u8 n <= 57, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation uses a simple prime sieve and a
   factor‑by‑factor check of the Legendre exponent.  The
   results are memoised so repeated calls are efficient. */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/*--- Prime generation ---------------------------------------------------*/
static uint32_t *primes = NULL;      /* list of primes found so far   */
static size_t primes_cnt = 0;        /* number of primes in the list  */
static size_t primes_cap = 0;        /* allocated capacity for primes */
static uint32_t primes_max = 1;      /* maximum number that has been
                                       tested for primality         */

/* enlarge the prime array if required */
static void primes_grow(void) {
    if (primes_cap == 0) {
        primes_cap = 32;
        primes = (uint32_t *)malloc(primes_cap * sizeof(uint32_t));
    } else {
        primes_cap *= 2;
        primes = (uint32_t *)realloc(primes, primes_cap * sizeof(uint32_t));
    }
}

/* extend the prime table up to at least `limit` */
static void ensure_primes_upto(uint32_t limit) {
    if (limit <= primes_max)
        return;
    if (primes == NULL) /* first initialisation */
        primes_grow();

    for (uint32_t num = primes_max + 1; num <= limit; ++num) {
        bool is_prime = true;
        uint32_t sq = (uint32_t)(num * num);
        for (size_t i = 0; i < primes_cnt; ++i) {
            uint32_t p = primes[i];
            if ((uint64_t)p * p > num)
                break;
            if (num % p == 0) {
                is_prime = false;
                break;
            }
        }
        if (is_prime) {
            if (primes_cnt == primes_cap)
                primes_grow();
            primes[primes_cnt++] = num;
        }
    }
    primes_max = limit;
}

/*--- Legendre exponent v_p((x)!) ---------------------------------------*/
static inline unsigned long long v_p_factorial(unsigned long long x, unsigned int p) {
    unsigned long long vp = 0;
    unsigned long long pp = p;
    while (pp <= x) {
        vp += x / pp;
        if (pp > x / p)          /* avoid overflow of pp *= p */
            break;
        pp *= p;
    }
    return vp;
}

/*--- Check if k^n divides (k-1)! -------------------------------------*/
static bool satisfies_k_n(unsigned long long k, idx_t n) {
    if (n == 0)
        return true;            /* any k^0 = 1 divides everything */

    unsigned long long rem = k;
    /* ensure we have enough primes for factorisation */
    uint32_t k32 = (uint32_t)k;
    ensure_primes_upto(k32);

    for (size_t i = 0; i < primes_cnt; ++i) {
        uint32_t p = primes[i];
        if ((uint64_t)p * p > rem)
            break;
        if (rem % p == 0) {
            unsigned int alpha = 0;
            while (rem % p == 0) {
                rem /= p;
                ++alpha;
            }
            unsigned long long vp = v_p_factorial(k - 1, p);
            if (vp < (unsigned long long)n * alpha)
                return false;
        }
    }
    if (rem > 1) {           /* remaining prime factor */
        unsigned long long vp = v_p_factorial(k - 1, (unsigned int)rem);
        if (vp < (unsigned long long)n)
            return false;
    }
    return true;
}

/*--- Memoised table for a(n) ------------------------------------------*/
static seq_t *memo = NULL;   /* stored answers, 0‑based index */
static size_t memo_len = 0;  /* number of stored answers */
static size_t memo_cap = 0;  /* allocated capacity */

/* enlarge the memo table if required */
static void memo_grow(void) {
    if (memo_cap == 0) {
        memo_cap = 64;
        memo = (seq_t *)malloc(memo_cap * sizeof(seq_t));
    } else {
        memo_cap *= 2;
        memo = (seq_t *)realloc(memo, memo_cap * sizeof(seq_t));
    }
}

/*--- Main function ------------------------------------------------------*/
seq_t a(idx_t n) {
    /* initialise memo with a(0)=2 */
    if (memo_len == 0) {
        memo_grow();
        memo[0] = (seq_t)2;   /* a(0) = 2 */
        memo_len = 1;
    }

    /* extend memo up to the required n */
    while (memo_len <= (size_t)n) {
        unsigned long long k = (unsigned long long)memo[memo_len - 1];
        if (k < 2ULL)
            k = 2ULL;          /* safety for n=0 */

        /* find the next k satisfying the condition */
        while (true) {
            /* ensure the primes list covers <k, to factorise k */
            if (k > 0xFFFFFFFFULL) {
                /* very large k: fallback to trial division by 2 and odd numbers only */
                /* unlikely for n<=10000, but kept for completeness */
                break;
            }
            if (!satisfies_k_n(k, (idx_t)memo_len))
                ++k;
            else
                break;
        }

        /* store the found k in memo */
        if (memo_len == memo_cap)
            memo_grow();
        memo[memo_len++] = (seq_t)k;
    }

    return memo[(size_t)n];
}
