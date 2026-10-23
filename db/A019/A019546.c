/*
 * A019546  Primes whose digits are primes; primes having only {2, 3, 5, 7} as
 *          digits.
 * https://oeis.org/A019546
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, changed
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 149, u32 n <= 48, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: primes whose decimal digits are 2, 3, 5 or 7
   a(n) = the n-th prime whose decimal representation uses only
          the digits {2,3,5,7}.  The sequence is infinite.
   This implementation enumerates candidates in increasing numeric
   order by interpreting a base‑4 index with a digit map
   0→2, 1→3, 2→5, 3→7 and testing each candidate for primality. */

/* ------------------------------------------------------------ */
/* Include necessary utilities */
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "../utils/is_prime_big.h"

/* ------------------------------------------------------------ */
/* Map from a base‑4 digit to the allowed decimal digit            */
static const uint8_t digit_map[4] = { 2, 3, 5, 7 };

/* ------------------------------------------------------------ */
/* State of the enumeration: current length and current index   */
static idx_t cur_len = 1;   /* number of decimal digits to produce */
static idx_t cur_idx = 0;   /* current base‑4 index within this length */

/* ------------------------------------------------------------ */
/* Pre‑computed powers of 4 to accelerate digit extraction      */
static idx_t pow4[64];
static bool pow_initialized = false;

static void init_pow4(void) {
    if (!pow_initialized) {
        pow4[0] = 1;
        for (int i = 1; i < 64; ++i) {
            pow4[i] = pow4[i - 1] * (idx_t)4;
        }
        pow_initialized = true;
    }
}

/* ------------------------------------------------------------ */
/* Return the next positive integer consisting only of 2,3,5,7   */
static seq_t next_candidate(void) {
    init_pow4();

    seq_t val     = 0;
    idx_t tmp     = cur_idx;

    /* Extract each base‑4 digit from most significant to least.
       pow4[pos] = 4^pos, so tmp / pow4[pos] gives the digit at that
       position (0‑based from the right). */
    for (int pos = cur_len - 1; pos >= 0; --pos) {
        uint8_t choice = (uint8_t)((tmp / pow4[pos]) % 4);
        val = val * (seq_t)10 + (seq_t)digit_map[choice];
    }

    /* Advance to the next index/length for the next call. */
    ++cur_idx;
    if (cur_idx == pow4[cur_len]) {
        cur_idx = 0;
        ++cur_len;
    }
    return val;
}

/* ------------------------------------------------------------ */
/* Simple cache of already found primes (enough for n≈10 000).   */
static seq_t cache[65536];
static size_t cache_len = 0;

/* ------------------------------------------------------------ */
/* External prime test from the utilities.                      */
extern bool is_prime_big(seq_t n);

/* ------------------------------------------------------------ */
/* Return the n-th prime whose digits are only 2,3,5,7.         */
seq_t a(idx_t n) {
    if (n == 0) return 0;          /* undefined for n=0; safety */

    size_t target = (size_t)n;

    if (target <= cache_len) {
        return cache[target - 1];
    }

    /* Generate candidates until we have the required term. */
    while (cache_len < target) {
        seq_t cand = next_candidate();
        if (is_prime_big(cand)) {
            cache[cache_len++] = cand;
        }
    }
    return cache[target - 1];
}
