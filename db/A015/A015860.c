/*
 * A015860  Numbers k such that phi(k) | sigma(k + 13).
 * https://oeis.org/A015860
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 35, u16 n <= 257, u32 n <= 500, u64 n <= 500,
 *            u128 n <= 500, big any n
 * Helpers:   sigma_uint128
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers k such that φ(k) divides σ(k + 13). */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/sigma_uint128.h"

/* Forward declaration of the external types. */
extern seq_t a(idx_t n);

/* ------------------------------------------------------------------ */
/* Small utilities ---------------------------------------------------- */
/* ------------------------------------------------------------------ */

/* Euler's totient function for 64‑bit integers. */
static uint64_t phi_uint64(uint64_t n)
{
    uint64_t result = n;
    uint64_t p = 2;
    if (n % p == 0) {
        while (n % p == 0) n /= p;
        result = result / p * (p - 1);
    }
    for (p = 3; p * p <= n; p += 2) {
        if (n % p == 0) {
            while (n % p == 0) n /= p;
            result = result / p * (p - 1);
        }
    }
    if (n > 1) {
        result = result / n * (n - 1);
    }
    return result;
}

/* ------------------------------------------------------------------ */
/* Sequence implementation ------------------------------------------- */
/* ------------------------------------------------------------------ */

static uint64_t *found = NULL;          /* array of found k's (uint64_t)      */
static size_t    found_count = 0;       /* number of stored terms             */
static size_t    found_cap   = 0;       /* current capacity of found array    */

static uint64_t   next_k = 1;           /* next k to test                     */

/* Grow the array holding the sequence terms. */
static void grow_found(size_t need)
{
    if (found_cap >= need) return;
    size_t new_cap = found_cap ? found_cap * 2 : 8;
    if (new_cap < need) new_cap = need;
    uint64_t *tmp = (uint64_t *)realloc(found, new_cap * sizeof(uint64_t));
    if (!tmp) exit(EXIT_FAILURE);
    found = tmp;
    found_cap = new_cap;
}

/* Append a new term. */
static void add_found(uint64_t k)
{
    grow_found(found_count + 1);
    found[found_count++] = k;
}

/* ------------------------------------------------------------------ */

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined for index 0 */

    /* Ensure we have at least n terms. */
    while (found_count < (size_t)n) {
        /* Test consecutive k's. */
        for (uint64_t k = next_k; ; ++k) {
            uint64_t phi = phi_uint64(k);

            /* k + 13 fits into 64 bits for the range we need. */
            uint64_t kp13 = k + 13;
            unsigned __int128 sigma = sigma_uint128(kp13);

            if (sigma % phi == 0) {
                add_found(k);
                if (found_count >= (size_t)n) {
                    /* Store the last tested k for the next call. */
                    next_k = k + 1;
                    goto done;
                }
            }
            /* To avoid infinite loops if k becomes huge, 
               the sequence grows steadily; we trust the loop stops. */
        }
    }
done:
    /* Return the requested term. */
    return (seq_t)found[n - 1];
}
