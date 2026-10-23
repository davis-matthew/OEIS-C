/*
 * A185952  Partial products of A002313.
 * https://oeis.org/A185952
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1)=2; for n>1, a(n)=a(n‑1)*p_{n‑1} where p_k is the k‑th prime
 *            ≡1 (mod 4) (starting with 5). This matches OEIS A185952
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 7, u64 n <= 12, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* ---------- Configuration ---------- */
#define MAX_N          10000          /* maximum n for which a(n) is required */
#define MAX_PRIMES     50000          /* buffer size for all primes discovered */

/* ---------- Static tables ---------- */
static seq_t prod[MAX_N + 1] = {0};           /* prod[1] … prod[MAX_N] */
static size_t prod_cnt = 0;                  /* number of products calculated */

/* store every prime found (used for trial division) */
static uint32_t primes_all[MAX_PRIMES];
static size_t   primes_all_cnt = 0;

/* candidate for the next odd number to test for primality */
static uint32_t current_candidate = 3;

/* has the initialization been performed? */
static bool initialized = false;

/* ---------- Helpers ---------- */
/* trial division primality test using the primes already stored */
static bool is_prime_uint32(uint32_t x)
{
    for (size_t i = 0; i < primes_all_cnt; ++i) {
        uint32_t p = primes_all[i];
        if ((uint64_t)p * (uint64_t)p > (uint64_t)x)
            break;
        if (x % p == 0)
            return false;
    }
    return true;
}

/* initialise product[1] and prime tables */
static void init_sequence(void)
{
    prod[1] = 2;              /* a(1)=2 */
    prod_cnt = 1;

    primes_all[primes_all_cnt++] = 2;   /* include 2 in the prime list */

    current_candidate = 3;
    initialized = true;
}

/* ---------- Sequence function ---------- */
seq_t a(idx_t n)
{
    if (!initialized) {
        init_sequence();
    }

    if (n == 0)
        return 0; /* offset is 1; behaviour for n<1 is irrelevant */

    /* generate enough products up to requested n */
    while (prod_cnt < (size_t)n) {
        uint32_t next_prod_prime = 0;

        /* find the next prime ≡1 (mod 4) */
        while (true) {
            uint32_t cand = current_candidate;

            bool prime = is_prime_uint32(cand);
            if (prime) {
                /* store the prime for future trial divisions */
                if (primes_all_cnt < MAX_PRIMES)
                    primes_all[primes_all_cnt++] = cand;

                if (cand % 4 == 1) {
                    next_prod_prime = cand;   /* this contributes to the product */
                    break;
                }
            }

            /* advance to the next odd candidate */
            current_candidate += 2;
        }

        /* compute the next term */
        prod[prod_cnt + 1] = prod[prod_cnt] * (seq_t)next_prod_prime;
        ++prod_cnt;

        /* next candidate starts just after the prime we just used */
        current_candidate = next_prod_prime + 2;
    }

    return prod[n];
}
