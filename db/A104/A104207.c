/*
 * A104207  Concatenation of consecutive twin primes.
 * https://oeis.org/A104207
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 0, u32 n <= 3, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   len_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Constructive: a(n) is obtained by concatenating the consecutive
   pair of twin primes (p1,p1+2) and (p2,p2+2), where p1 and p2 are
   the first primes of the (2*n)th and (2*n+1)th twin prime pairs. */
/* seq_t and idx_t are provided by the build system. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/len_uint64.h"

/* local helpers ----------------------------------------------------- */

static seq_t pow10_seq(unsigned int k)
{
    seq_t r = 1;
    while (k--) r *= 10;
    return r;
}

/* prime / twin prime storage --------------------------------------- */

static uint32_t *primes = NULL;
static size_t primes_cnt   = 0;
static size_t primes_cap   = 0;

static uint32_t *twin = NULL;
static size_t twin_cnt   = 0;
static size_t twin_cap   = 0;

/* test if x is prime using the list of primes already found */
static bool is_prime_uint32(uint32_t x)
{
    if (x < 2) return false;
    for (size_t i = 0; i < primes_cnt; ++i) {
        uint32_t p = primes[i];
        if ((uint64_t)p * p > x) break;
        if (x % p == 0) return false;
    }
    return true;
}

/* ensure that twin_cnt > needed, generating primes as required */
static void ensure_twin_count(size_t needed)
{
    if (twin_cnt > needed) return;

    /* start generating from the next integer after the last prime */
    uint32_t candidate = (primes_cnt > 0) ? primes[primes_cnt - 1] + 1 : 2;

    /* optional: reserve more space */
    if (twin_cap < needed + 1024) {
        twin_cap = needed + 1024;
        twin = (uint32_t *)realloc(twin, twin_cap * sizeof(uint32_t));
        if (!twin) return;
    }
    if (primes_cap < 1024 + primes_cnt) {
        primes_cap = 1024 + primes_cnt;
        primes = (uint32_t *)realloc(primes, primes_cap * sizeof(uint32_t));
        if (!primes) return;
    }

    while (twin_cnt <= needed) {
        if (is_prime_uint32(candidate)) {
            /* add to primes list */
            if (primes_cnt == primes_cap) {
                primes_cap *= 2;
                primes = (uint32_t *)realloc(primes, primes_cap * sizeof(uint32_t));
            }
            primes[primes_cnt++] = candidate;

            /* check for twin pair */
            if (primes_cnt >= 2 &&
                primes[primes_cnt - 1] == primes[primes_cnt - 2] + 2) {
                if (twin_cnt == twin_cap) {
                    twin_cap *= 2;
                    twin = (uint32_t *)realloc(twin, twin_cap * sizeof(uint32_t));
                }
                twin[twin_cnt++] = primes[primes_cnt - 2];
            }
        }
        candidate++;
    }
}

/* main sequence function ------------------------------------------- */

seq_t a(idx_t n)
{
    /* we need the (2*n)th and (2*n+1)th twin prime pairs */
    size_t needed = 2*n + 1;
    ensure_twin_count(needed);

    /* safety check – if the limit was too small, just return 0 */
    if (twin_cnt <= 2*n + 1) return 0;

    uint32_t p1 = twin[2*n];
    uint32_t p2 = p1 + 2;
    uint32_t p3 = twin[2*n + 1];
    uint32_t p4 = p3 + 2;

    /* concatenate the four primes */
    seq_t r = p1;
    r = r * pow10_seq(len_uint64(p2)) + p2;
    r = r * pow10_seq(len_uint64(p3)) + p3;
    r = r * pow10_seq(len_uint64(p4)) + p4;

    return r;
}
