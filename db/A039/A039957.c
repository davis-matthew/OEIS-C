/*
 * A039957  Squarefree numbers congruent to 3 mod 4.
 * https://oeis.org/A039957
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   not stated
 * Range:     u8 n <= 52, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: iterate over numbers ≡ 3 (mod 4) and test squarefreeness by trial division. */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* ---------- primes sieve (once) ---------- */
    static uint64_t *const_prime_list = NULL;
    static size_t prime_cnt = 0;
    static size_t prime_cap = 0;
    static bool prime_inited = false;

    if (!prime_inited) {
        /* generate all primes up to LIMIT */
        const uint64_t LIMIT = 100000;            /* more than enough for n ≈ 10 000 */
        bool *is_prime = (bool *)calloc((size_t)LIMIT + 1, sizeof(bool));
        if (!is_prime) return 0;                  /* out‑of‑memory, abort */

        for (uint64_t i = 2; i <= LIMIT; ++i)
            is_prime[(size_t)i] = true;

        for (uint64_t i = 2; i * i <= LIMIT; ++i) {
            if (is_prime[(size_t)i]) {
                for (uint64_t j = i * i; j <= LIMIT; j += i)
                    is_prime[(size_t)j] = false;
            }
        }

        /* count primes and allocate storage */
        size_t estimated = LIMIT / 10;            /* rough estimate */
        prime_cap = estimated;
        const_prime_list = (uint64_t *)malloc(prime_cap * sizeof(uint64_t));
        if (!const_prime_list) { free(is_prime); return 0; }

        for (uint64_t i = 2; i <= LIMIT; ++i) {
            if (is_prime[(size_t)i]) {
                if (prime_cnt == prime_cap) {
                    prime_cap <<= 1;              /* double buffer */
                    uint64_t *tmp = (uint64_t *)realloc(const_prime_list, prime_cap * sizeof(uint64_t));
                    if (!tmp) { free(is_prime); free(const_prime_list); return 0; }
                    const_prime_list = tmp;
                }
                const_prime_list[prime_cnt++] = i;
            }
        }

        free(is_prime);
        prime_inited = true;
    }

    /* ---------- search for the nth term ---------- */
    seq_t count = 0;
    seq_t candidate = 3;                    /* first number ≡ 3 (mod 4) */

    for (;;) {
        bool squarefree = true;

        for (size_t i = 0; i < prime_cnt; ++i) {
            uint64_t p = const_prime_list[i];
            seq_t pp = (seq_t)p * (seq_t)p;   /* p² */
            if (pp > candidate)
                break;
            if (candidate % (seq_t)p == 0) {
                if (candidate % pp == 0) {
                    squarefree = false;       /* p² divides candidate */
                    break;
                }
            }
        }

        if (squarefree) {
            ++count;
            if (count == n)
                return candidate;
        }

        candidate += 4;                      /* keep numbers ≡ 3 (mod 4) */
    }
}
