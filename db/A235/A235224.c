/*
 * A235224  a(0) = 0, and for n > 0, a(n) = largest k such that A002110(k-1)
 *          <= n, where A002110(k) gives the k-th primorial number.
 * https://oeis.org/A235224
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = largest k such that A002110(k-1) <= n
 * Range:     u8 n <= 571, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where A002110(k) = product of the first k primes, A002110(0)=1 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/* ---------- helper: generate successive primes ---------- */
static uint64_t primes_buf[64];
static size_t primes_count = 0;

static uint64_t next_prime_small(void) {
    /* Return the next prime after the last one stored in primes_buf.
       If no prime has been stored yet, start with 2. */
    if (primes_count == 0) {
        primes_buf[0] = 2;
        primes_count = 1;
        return 2;
    }

    uint64_t candidate = primes_buf[primes_count - 1] + 1;
    for (;;) {
        bool is_prime = true;
        for (size_t j = 0; j < primes_count; ++j) {
            uint64_t p = primes_buf[j];
            if (p > candidate / p) /* p*p > candidate, no need to test further */
                break;
            if (candidate % p == 0) {
                is_prime = false;
                break;
            }
        }
        if (is_prime) {
            if (primes_count < sizeof(primes_buf) / sizeof(primes_buf[0])) {
                primes_buf[primes_count++] = candidate;
            }
            return candidate;
        }
        ++candidate;
    }
}

/* ---------- storage for primorials ---------- */
static seq_t *primorials = NULL;
static size_t primorial_cap = 0;
static size_t primorial_count = 0;

static void ensure_primorial_capacity(size_t needed) {
    if (needed <= primorial_cap)
        return;
    size_t new_cap = primorial_cap ? primorial_cap * 2 : 8;
    if (new_cap < needed)
        new_cap = needed;
    seq_t *new_arr = (seq_t *)malloc(new_cap * sizeof(seq_t));
    if (!new_arr)
        exit(EXIT_FAILURE);
    if (primorials) {
        memcpy(new_arr, primorials, primorial_count * sizeof(seq_t));
        free(primorials);
    }
    primorials = new_arr;
    primorial_cap = new_cap;
}

seq_t a(idx_t n) {
    if (n == 0)
        return (seq_t)0;

    /* initialise primorials once */
    if (!primorials) {
        ensure_primorial_capacity(1);
        primorials[0] = (seq_t)1;   /* P0 = 1 */
        primorial_count = 1;
    }

    /* extend primorial list until next one would exceed n */
    while (primorials[primorial_count - 1] <= (seq_t)n) {
        uint64_t p = next_prime_small();            /* next prime */
        seq_t next = primorials[primorial_count - 1] * (seq_t)p;
        ensure_primorial_capacity(primorial_count + 1);
        primorials[primorial_count++] = next;
    }

    return (seq_t)(primorial_count - 1);   /* a(n) = index of largest primorial <= n */
}
