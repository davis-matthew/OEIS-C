/*
 * A161386  Number of letters in n-th prime (in Spanish).
 * https://oeis.org/A161386
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, word
 * Formula:   not stated
 * Range:     u8 n <= 54, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) = number of letters in Spanish name of nth prime. */
/* The calculation uses trial division to generate primes on demand. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* Helper: Tests primality by trial division up to sqrt(n). */
static bool is_prime_trial(seq_t n) {
    if (n < 2) return false;
    if (n % 2 == 0) return n == 2;
    seq_t r = n;
    for (seq_t d = 3; d * d <= r; d += 2) {
        if (n % d == 0) return false;
    }
    return true;
}

/* ------------ Spanish letter counts for 0‑999,999 ------------- */
static const uint8_t unitLen[10] = {0, 3, 3, 4, 6, 5, 4, 5, 4, 5};          /* unitLen[d]   */
static const uint8_t teenLen[10] = {4, 4, 4, 5, 7, 6, 9,10, 9,10};          /* teenLen[d]   */
static const uint8_t tensLen[10] = {0,0,0,7,8,9,7,7,7,7};                  /* tensLen[t]   */
static const uint8_t hundredLen[10] = {0,0,10,11,13,10,11,11,11,11};        /* hundredLen[h] */

/* Recursive count of letters for num < 1 000 000. */
static seq_t count_letters(idx_t num) {
    if (num < 10) {
        return unitLen[(int)num];
    }
    if (num < 20) {
        return teenLen[(int)(num - 10)];
    }
    if (num < 100) {
        if (num < 30) {
            if (num == 20) return 6;                /* "veinte" */
            return 6 + unitLen[(int)(num - 20)];
        }
        uint8_t t = (uint8_t)(num / 10);
        uint8_t u = (uint8_t)(num % 10);
        uint8_t len = tensLen[t];
        if (u == 0) return len;
        /* add "y" between tens and units for 41, 51, etc. */
        return len + 1 + unitLen[u];
    }
    if (num < 1000) {
        uint8_t h = (uint8_t)(num / 100);
        idx_t rem = num % 100;
        if (h == 1) {
            if (rem == 0) return 4;                 /* "cien" */
            return 6 + count_letters(rem);           /* "ciento..." */
        } else {
            uint8_t hl = hundredLen[h];
            if (rem == 0) return hl;
            return hl + count_letters(rem);
        }
    }
    if (num < 1000000) {
        idx_t thou = num / 1000;
        idx_t rem = num % 1000;
        if (thou == 1) {
            if (rem == 0) return 3;                 /* "mil" */
            return 3 + count_letters(rem);
        } else {
            seq_t tl = count_letters(thou);
            if (rem == 0) return tl + 3;
            return tl + 3 + count_letters(rem);
        }
    }
    /* Not needed for the tested range */
    return 0;
}

/* ------------------------------------------------------------ */
seq_t a(idx_t n) {
    if (n == 0) return 0;   /* undefined for 0, return 0 safely */

    /* Static prime cache */
    static idx_t *primes = NULL;
    static size_t primes_cap = 0;
    static size_t primes_cnt = 0;

    /* Grow the cache until we have at least n primes */
    if (primes_cnt < n) {
        /* Reserve space if needed */
        if (n > primes_cap) {
            size_t new_cap = primes_cap == 0 ? 16 : primes_cap * 2;
            while (new_cap < n) new_cap *= 2;
            idx_t *tmp = (idx_t *)realloc(primes, new_cap * sizeof(idx_t));
            if (!tmp) exit(1);                /* allocation failure */
            primes = tmp;
            primes_cap = new_cap;
        }

        /* Start searching from the last prime + 1 (or 2 if none yet) */
        idx_t start = primes_cnt == 0 ? 2 : primes[primes_cnt - 1] + 1;
        for (idx_t p = start; primes_cnt < n; ++p) {
            if (is_prime_trial(p)) {
                primes[primes_cnt++] = p;
            }
        }
    }

    /* primes are zero‑indexed, n is one‑indexed */
    idx_t prime = primes[n - 1];
    return count_letters(prime);
}
