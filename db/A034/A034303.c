/*
 * A034303  Zeroless primes that become nonprime if any digit is deleted.
 * https://oeis.org/A034303
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, nice
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 784, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   get_digits, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A034303 – Zeroless primes that become nonprime if any digit is deleted
   Code computes the sequence by a search. */

// Required utilities
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/is_prime_big.h"   /* bool is_prime_big(seq_t n); */
#include "../utils/get_digits.h"    /* void get_digits(seq_t x, uint8_t *buf, size_t *len); */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;           /* undefined for n = 0 */

    /* Cached sequence values ------------------------------------------- */
    static seq_t *cache      = NULL;
    static size_t cache_cap  = 0;
    static size_t cache_len  = 0;

    /* Next number to test after the last found term --------------------- */
    static idx_t next = 11;                 /* first candidate ≥ 11 */

    /* Ensure cache holds at least n terms -------------------------------- */
    while (cache_len < n) {
        /* Scan integers starting from 'next' -------------------------------- */
        for (idx_t x = next; ; ++x) {
            if (x < 10) continue;           /* need at least two digits */

            seq_t sx = (seq_t)x;

            /* Extract digits (least‑significant first) ---------------------- */
            uint8_t digits[80];             /* enough for 128‑bit numbers    */
            size_t   len = 0;
            get_digits(sx, digits, &len);
            if (len < 2) continue;          /* safety check */

            /* Reject numbers that contain a zero digit --------------------- */
            bool has_zero = false;
            for (size_t i = 0; i < len; ++i) {
                if (digits[i] == 0) { has_zero = true; break; }
            }
            if (has_zero) continue;

            /* Make digits most‑significant first for easier deletion -------- */
            for (size_t i = 0; i < len/2; ++i) {
                uint8_t tmp = digits[i];
                digits[i]   = digits[len-1-i];
                digits[len-1-i] = tmp;
            }

            /* Candidate must be prime itself -------------------------------- */
            if (!is_prime_big(sx)) continue;

            /* Check every single‑digit deletion ------------------------------ */
            bool ok = true;
            for (size_t del = 0; del < len && ok; ++del) {
                seq_t v = 0;
                for (size_t i = 0; i < len; ++i) {
                    if (i == del) continue;
                    v = v * 10 + digits[i];
                }
                if (is_prime_big(v)) ok = false;
            }
            if (!ok) continue;              /* not part of the sequence */

            /* Store the valid term ------------------------------------------- */
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap ? (cache_cap << 1) : 16;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) abort();
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = sx;
            next = x + 1;                    /* next candidate starts after x */
            break;                            /* found one, loop again if needed */
        }
    }

    return cache[n - 1];
}
