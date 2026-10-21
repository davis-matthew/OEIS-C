/*
 * A060881  a(n) = primorial(n) + prime(n + 1), where primorial(n) =
 *          A002110(n).
 * https://oeis.org/A060881
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = primorial(n) + prime(n+1)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 9, u64 n <= 15, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

/* 2.0 s limit: preallocate space for at least 20000 primes and 20001 primorials.
   This covers n ≤ 10000 comfortably. */
#define MAX_PRIMES      20000
#define MAX_PRIMORIAL   20001

seq_t a(idx_t n)
{
    /* static lookup tables and counts */
    static uint32_t primes[MAX_PRIMES];
    static seq_t   primorial[MAX_PRIMORIAL];
    static idx_t   num_primes   = 0;   /* number of primes currently stored */
    static idx_t   num_primorial = 0;   /* number of primorials currently stored */

    /* initialise first prime and primorial[0] */
    if (num_primes == 0) {
        primes[0] = 2;                 /* prime(1) */
        num_primes = 1;

        primorial[0] = (seq_t)1;       /* primorial(0) */
        num_primorial = 1;             /* primorial[0] is ready */
    }

    /* Ensure primes[0 .. n] (i.e. prime(n+1)) are available */
    while (num_primes <= n+1) {
        /* start testing from the next integer after the last prime */
        uint32_t candidate = primes[num_primes-1] + 1;
        if (candidate <= 2) candidate = 2;
        if (candidate % 2 == 0) ++candidate;   /* skip even numbers after 2 */

        /* find the next prime by trial division */
        for (;;) {
            bool is_prime = true;
            for (idx_t i = 0; i < num_primes; ++i) {
                uint32_t p = primes[i];
                uint64_t p2 = (uint64_t)p * p;
                if (p2 > candidate) break;      /* sqrt(candidate) reached */
                if (candidate % p == 0) { is_prime = false; break; }
            }
            if (is_prime) break;                 /* prime found */
            candidate += 2;                      /* try next odd number */
        }

        if (num_primes < MAX_PRIMES) {
            primes[num_primes++] = candidate;
        } else {
            /* should not occur for n ≤ 10000 */
        }
    }

    /* Compute primorials up to index n */
    while (num_primorial <= n) {
        /* primorial(i) = primorial(i‑1) * prime(i) */
        /* prime(i) is primes[i-1] because primes[0] is prime(1) */
        seq_t next = primorial[num_primorial-1] * (seq_t)primes[num_primorial-1];
        primorial[num_primorial++] = next;
    }

    /* a(n) = primorial(n) + prime(n+1) */
    return primorial[n] + (seq_t)primes[n];   /* primes[n] == prime(n+1) */
}
