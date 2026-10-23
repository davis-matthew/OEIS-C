/*
 * A191679  Potential magic constants of 9 X 9 magic squares composed of
 *          consecutive primes.
 * https://oeis.org/A191679
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (sum of 81 consecutive primes starting at the n‑th window
 *            whose sum is divisible by 9) / 9
 * Range:     u8 none, u16 n <= 8, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* ---------- primes -------------------------------------------------- */
static uint32_t *prime_array = NULL;      /* stored primes (≥2)     */
static size_t   prime_count = 0;          /* number of primes       */
static size_t   prime_cap   = 0;          /* current capacity       */
static uint32_t prime_candidate = 2;       /* next odd number to test */

static void append_prime(uint32_t p)
{
    if (prime_count == prime_cap) {
        size_t new_cap = prime_cap ? prime_cap * 2 : 16;
        uint32_t *new_p = (uint32_t *)realloc(prime_array,
                                              new_cap * sizeof(uint32_t));
        if (!new_p) exit(1);
        prime_array = new_p;
        prime_cap = new_cap;
    }
    prime_array[prime_count++] = p;
}

static uint32_t next_prime(void)
{
    if (prime_count == 0) {          /* first prime is 2      */
        uint32_t p = 2;
        append_prime(p);
        prime_candidate = 3;         /* next test candidate  */
        return p;
    }

    for (;;) {
        uint32_t cand = prime_candidate;
        prime_candidate += 2;        /* only odd numbers      */

        bool ok = true;
        for (size_t i = 0; i < prime_count; ++i) {
            uint32_t p = prime_array[i];
            uint64_t sq = (uint64_t)p * p;
            if (sq > cand) break;
            if (cand % p == 0) { ok = false; break; }
        }
        if (ok) {
            append_prime(cand);
            return cand;
        }
    }
}

/* ---------- cache of computed a(n) ---------------------------------- */
static seq_t *value_cache = NULL;      /* a(1), a(2), ...        */
static size_t value_count = 0;          /* how many have been stored */
static size_t value_cap   = 0;          /* capacity of the array     */

static void append_value(seq_t val)
{
    if (value_count == value_cap) {
        size_t new_cap = value_cap ? value_cap * 2 : 16;
        seq_t *new_p = (seq_t *)realloc(value_cache,
                                        new_cap * sizeof(seq_t));
        if (!new_p) exit(1);
        value_cache = new_p;
        value_cap = new_cap;
    }
    value_cache[value_count++] = val;
}

/* ---------- sliding window over last 81 primes --------------------- */
static uint32_t window[81];
static size_t window_start = 0; /* index of the oldest prime in the window */
static size_t window_size = 0;  /* current number of primes in the window */
static seq_t  window_sum  = 0;  /* sum of the 81 primes in the window */

/* ---------- main sequence function --------------------------------- */
seq_t a(idx_t n)
{
    if (n == (idx_t)0) return (seq_t)0;   /* undefined for 0, but safe */

    /* generate more terms until we have at least n values */
    while (value_count < (size_t)n) {
        uint32_t p = next_prime();

        if (window_size < 81) {
            window[window_size++] = p;
            window_sum += (seq_t)p;
            if (window_size == 81) {
                if (window_sum % 9 == 0) {
                    append_value(window_sum / 9);
                }
            }
        } else {
            uint32_t old = window[window_start];
            window_sum += (seq_t)p - (seq_t)old;
            window[window_start++] = p;
            if (window_start == 81) window_start = 0;
            if (window_sum % 9 == 0) {
                append_value(window_sum / 9);
            }
        }
    }
    return value_cache[(size_t)n - 1];
}
