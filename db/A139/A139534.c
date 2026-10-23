/*
 * A139534  Numbers k such that numbers 24*k + 17 and 24*k + 19 are twin
 *          primes.
 * https://oeis.org/A139534
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 2996, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes_upto, grow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function returns the n‑th integer k such that both 24*k+17 and 24*k+19 are prime (OEIS A139534). */
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "../utils/build_primes_upto.h"
#include "../utils/grow_seq.h"

/* Find whether n is prime, using a pre‑computed sieve of primes. */
static bool is_prime_uint64(uint64_t n)
{
    static uint64_t *primes = NULL;
    static size_t prime_cnt = 0;

    if (primes == NULL) {
        /* Pre‑compute primes up to a fixed bound that comfortably covers the
           range needed for n up to ~10 000. */
        size_t cnt;
        build_primes_upto((idx_t)20000000ULL, &primes, &cnt);
        prime_cnt = cnt;
    }

    if (n < 2) return false;
    for (size_t i = 0; i < prime_cnt && (uint64_t)primes[i] * primes[i] <= n; ++i) {
        if (n % primes[i] == 0) return n == primes[i];
    }
    return true;
}

/* Return the n‑th term of A139534.  The sequence is 1‑based with a(1)=0. */
seq_t a(idx_t n)
{
    static seq_t *terms = NULL;
    static size_t term_cap = 0;
    static size_t term_cnt = 0;
    static idx_t k_next = 0; /* next value of k to test */

    if (n == 0) return 0; /* undefined, but avoid undefined behaviour */

    /* Produce terms until we have at least n of them. */
    while (term_cnt < (size_t)n) {
        seq_t val1 = (seq_t)k_next * 24 + 17;
        seq_t val2 = val1 + 2;  /* 24*k + 19 */

        if (is_prime_uint64((uint64_t)val1) && is_prime_uint64((uint64_t)val2)) {
            /* Ensure capacity for the new term. */
            grow_seq(&terms, &term_cap, term_cnt + 1);
            terms[term_cnt++] = (seq_t)k_next;
        }
        ++k_next;
    }

    return terms[n - 1];
}
