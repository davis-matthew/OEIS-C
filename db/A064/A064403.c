/*
 * A064403  Numbers k such that prime(k) + k and prime(k) - k are both primes.
 * https://oeis.org/A064403
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 575, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   get_prime, isPrime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A064403
   Numbers k such that prime(k) + k and prime(k) - k are both primes.
   Implementation uses the provided get_prime(k) and isPrime(n) helpers
   to avoid rebuilding a sieve from scratch.
*/

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#include "../utils/get_prime.h"   /* extern uint32_t get_prime(idx_t k); */
#include "../utils/isPrime.h"     /* extern bool isPrime(idx_t x);    */

/* State is maintained between calls for efficiency. */
static struct {
    uint32_t *terms;     /* array of k‑values already found */
    size_t  termCnt;     /* number of terms stored */
    size_t  termCap;     /* capacity of the array */
    uint32_t nextK;      /* next k to be tested */
} a_state = {0, 0, 0, 1};

/* Append a new term to the cache. */
static void append_term(uint32_t k)
{
    if (a_state.termCnt == a_state.termCap) {
        size_t newCap = a_state.termCap ? a_state.termCap * 2 : 1;
        uint32_t *tmp = (uint32_t*)realloc(a_state.terms, newCap * sizeof(uint32_t));
        a_state.terms = tmp;
        a_state.termCap = newCap;
    }
    a_state.terms[a_state.termCnt++] = k;
}

/* Main function required by the framework. */
seq_t a(idx_t n)
{
    /* Generate terms until we have at least n of them. */
    while (a_state.termCnt < (size_t)n) {
        uint32_t k = a_state.nextK;
        uint32_t p = get_prime(k);                /* prime(k)          */
        uint32_t plus = p + k;
        uint32_t minus = p > k ? p - k : 0;

        bool ok_plus  = isPrime(plus);
        bool ok_minus = (minus > 1) && isPrime(minus);

        if (ok_plus && ok_minus)
            append_term(k);

        a_state.nextK++;
    }

    return (seq_t)a_state.terms[(size_t)n - 1];
}
