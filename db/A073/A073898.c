/*
 * A073898  a(1) = 1; for n>1, a(n) = smallest even or odd number not
 *          occurring earlier accordingly as n is prime or composite.
 * https://oeis.org/A073898
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(1) = 1; for n > 1, let π(n) be the prime‑counting function. If
 *            n is prime: a(n) = 2 * π(n) else a(n) = 2 * (n − π(n)) − 1
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The code computes π(n) on demand by sieving up to the maximum
   value seen so far.  It stores the sieve and prefix counts in
   static arrays that are extended when a larger n is requested. */

#include <stdbool.h>
#include <stdlib.h>

static idx_t sieve_limit = 0;       /* maximum value sieved */
static bool  *is_prime   = NULL;     /* prime flags (0/1) */
static uint32_t *pi       = NULL;     /* cumulative prime count */

/* Expand the sieve to cover at least up to 'need' */
static void ensure_sieve(idx_t need) {
    if (sieve_limit >= need) return;          /* already enough */

    /* Allocate new arrays */
    bool  *new_is_prime = (bool *)malloc((need + 1) * sizeof(bool));
    uint32_t *new_pi = (uint32_t *)malloc((need + 1) * sizeof(uint32_t));
    if (!new_is_prime || !new_pi) return;     /* allocation failed – abort */

    /* Initialize: 0 and 1 are not prime */
    new_is_prime[0] = false;
    if (need >= 1) new_is_prime[1] = false;
    for (idx_t i = 2; i <= need; ++i) new_is_prime[i] = true;

    /* Sieve of Eratosthenes */
    for (idx_t i = 2; (idx_t)(i * i) <= need; ++i) {
        if (new_is_prime[i]) {
            for (idx_t j = i * i; j <= need; j += i) new_is_prime[j] = false;
        }
    }

    /* Compute prefix prime counts */
    uint32_t count = 0;
    new_pi[0] = 0;
    for (idx_t i = 1; i <= need; ++i) {
        if (new_is_prime[i]) ++count;
        new_pi[i] = count;
    }

    /* Replace old arrays */
    free(is_prime);
    free(pi);
    is_prime = new_is_prime;
    pi       = new_pi;
    sieve_limit = need;
}

/* Main sequence function */
seq_t a(idx_t n) {
    if (n == 1) return 1;          /* special case */

    ensure_sieve(n);               /* make sure π(n) is known */

    uint32_t pcount  = pi[n];          /* π(n) */
    uint32_t prev_pc = (n > 0) ? pi[n - 1] : 0; /* π(n‑1) */

    /* n is prime iff π(n) - π(n‑1) == 1 (and n > 1) */
    if (n > 1 && (pcount - prev_pc) == 1) {
        return (seq_t)2 * (seq_t)pcount;          /* 2·π(n)  */
    } else {
        /* 2·(n − π(n)) − 1  */
        seq_t n_seq = (seq_t)n;
        seq_t p_seq = (seq_t)pcount;
        return ((seq_t)2 * (n_seq - p_seq)) - 1;
    }
}
