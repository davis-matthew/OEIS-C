/*
 * A050671  Primes p such that number of primes produced according to rules
 *          stipulated in Honaker's A048853 is 9.
 * https://oeis.org/A050671
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   the sequence consists of all primes p for which Honaker's
 *            A048853 generates exactly nine distinct primes. The function
 *            computes the n‑th such prime by searching primes in ascending
 *            order
 * Range:     u8 none, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   is_prime_small, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime_small.h"
#include "../utils/seq_realloc.h"

static uint64_t pow10[20] = {0U};

/* initialise the table of powers of ten (once) */
static void init_pow10(void) {
    static int done = 0;
    if (done) return;
    pow10[0] = 1ULL;
    for (int i = 1; i < 20; ++i) pow10[i] = pow10[i-1] * 10ULL;
    done = 1;
}

/* Count how many primes are produced by changing exactly one decimal
   digit of the given prime p (no leading zeros). */
static unsigned count_digit_replacements(uint64_t p) {
    uint8_t digits[20];
    size_t len = 0;
    uint64_t tmp = p;
    if (tmp == 0) {
        digits[len++] = 0;
    } else {
        while (tmp > 0) {
            digits[len++] = (uint8_t)(tmp % 10);
            tmp /= 10;
        }
    }

    unsigned cnt = 0;
    for (size_t pos = 0; pos < len; ++pos) {
        uint8_t old_d = digits[pos];
        uint64_t factor = pow10[pos];
        for (uint8_t nd = 0; nd <= 9; ++nd) {
            if (nd == old_d) continue;
            /* avoid leading zero */
            if (pos == len-1 && nd == 0) continue;
            int delta = (int)nd - (int)old_d;
            uint64_t new_val = p + (uint64_t)delta * factor;
            if (is_prime_small((seq_t)new_val)) ++cnt;
        }
    }
    return cnt;
}

/* --- Prime generation and caching --- */
static uint64_t *prime_list   = NULL;
static size_t    prime_cap   = 0;
static size_t    prime_len   = 0;
static uint64_t  next_candidate = 2U;

/* Grow the prime list array to hold at least 'need' entries */
static void ensure_prime_cap(size_t need) {
    if (prime_cap >= need) return;
    size_t newcap = prime_cap ? prime_cap << 1 : 16U;
    while (newcap < need) newcap <<= 1;
    uint64_t *tmp = (uint64_t *)realloc(prime_list, newcap * sizeof(uint64_t));
    if (!tmp) abort();
    prime_list = tmp;
    prime_cap  = newcap;
}

/* --- Sequence cache --- */
static seq_t *seq_cache = NULL;
static size_t seq_len  = 0;

/* Return the n‑th term (n >= 1) of the sequence. */
seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;          /* undefined for n < 1 */

    init_pow10();

    /* grow the cache until it contains n terms */
    while (seq_len < (size_t)n) {
        /* retrieve the next prime */
        uint64_t p;
        for (;;) {
            uint64_t c = next_candidate++;
            bool prime = true;

            ensure_prime_cap(prime_len + 1);
            for (size_t i = 0; i < prime_len; ++i) {
                uint64_t pr = prime_list[i];
                if (pr * pr > c) break;
                if (c % pr == 0) { prime = false; break; }
            }

            if (prime) {
                prime_list[prime_len++] = c;
                p = c;
                break;
            }
        }

        /* test the Honaker condition */
        if (count_digit_replacements(p) == 9) {
            seq_cache = (seq_t *)seq_realloc(seq_cache, seq_len, seq_len + 1);
            seq_cache[seq_len++] = (seq_t)p;
        }
    }

    return seq_cache[n-1];
}
