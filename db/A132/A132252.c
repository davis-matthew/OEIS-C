/*
 * A132252  Isolated primes congruent to 19 (mod 30).
 * https://oeis.org/A132252
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = the nth isolated prime ≡ 19 (mod 30) (a prime p of this
 *            form such that p-2 and p+2 are composite)
 * Range:     u8 n <= 1, u16 n <= 519, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <math.h>

/* A static buffer for the isolated primes.  The capacity is far larger
   than needed for the requested indices (≈10 000).  All entries are
   stored as plain uint64_t, which is safe to cast to seq_t when
   returning.  */
#define MAX_ISO 200000
static uint64_t iso_vals[MAX_ISO];
static size_t iso_size = 0;   /* number of isolated primes found */

/* Sieve data */
static uint8_t *prime_flag = NULL;   /* 1 = prime, 0 = composite   */
static idx_t    sieve_limit = 0;     /* highest number sieved       */

/* Ensure that the global sieve covers at least numbers up to 'new_limit'.
   The array is reallocated and recomputed each time.  */
static void extend_sieve(idx_t new_limit)
{
    if (new_limit <= sieve_limit)
        return;

    size_t cap = (size_t)new_limit + 1;
    uint8_t *flag = (uint8_t *)malloc(cap);
    if (!flag)
        abort();

    /* initialise as all primes */
    for (size_t i = 0; i < cap; ++i)
        flag[i] = 1;
    flag[0] = flag[1] = 0;

    idx_t r = (idx_t)sqrt((double)new_limit);
    for (idx_t i = 2; i <= r; ++i) {
        if (flag[i]) {
            for (idx_t j = i * i; j <= new_limit; j += i)
                flag[j] = 0;
        }
    }

    if (prime_flag)
        free(prime_flag);
    prime_flag = flag;
    sieve_limit = new_limit;
}

/* The if of the current lowest candidate ≥19 in arithmetic progression
   with difference 30.  */
static idx_t last_checked = 19;

/* Generate isolated primes until at least 'target' many have been found. */
static void generate_iso_until(idx_t target)
{
    while (iso_size < target) {
        /* Ensure data for the current candidate and its neighbours. */
        if (last_checked + 2 > sieve_limit)
            extend_sieve(last_checked + 2);

        if (prime_flag[last_checked]) {
            bool isolated =    !prime_flag[last_checked - 2] &&
                               !prime_flag[last_checked - 1] &&
                               !prime_flag[last_checked + 1] &&
                               !prime_flag[last_checked + 2];
            if (isolated) {
                if (iso_size >= MAX_ISO)
                    abort(); /* buffer overflow guard */
                iso_vals[iso_size++] = (uint64_t)last_checked;
            }
        }
        last_checked += 30; /* next number ≡ 19 (mod 30) */
    }
}

seq_t a(idx_t n)
{
    if (n == 0)                        /* ignore offset 0 */
        return (seq_t)0;
    generate_iso_until(n);
    return (seq_t)iso_vals[n - 1];
}
