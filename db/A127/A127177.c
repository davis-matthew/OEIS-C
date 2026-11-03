/*
 * A127177  Primes whose "Look And Say" descriptions from right to left (in
 *          the sense of method A, i.e., frequency followed by
 *          digit-indication) are also primes.
 * https://oeis.org/A127177
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = nth prime whose right‑to‑left Look‑And‑Say description is
 *            prime
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Validated: partial, pass, 47 terms through n = 47 (2026-11-03)
 * Helpers:   is_prime_u64, mem, num_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/num_digits.h"
#include "../utils/mem.h"
#include "../utils/is_prime_u64.h"

/* Compute 10^e for a non‑negative integer exponent e */
static seq_t pow10(idx_t e) {
    seq_t res = 1;
    while (e) {
        res *= 10;
        e--;
    }
    return res;
}

/* Miller–Rabin primality test for seq_t (supports 8/16/32/64/128/ arbitrary-precision) */
/* n fits in uint64_t (n >= 0 here) */
static bool is_prime_big_fits_u64(seq_t v)
{
    if (sizeof(seq_t) <= 8) return true;
    return !((seq_t)UINT64_MAX < v);
}

static bool is_prime_big(seq_t n) {
    if (n < 2) return false;
    /* exact in 64 bits when n fits: the seq_t products below overflow at
       u64/i64 once n passes 2^32, and are slow under big */
    if (is_prime_big_fits_u64(n)) return is_prime_u64((uint64_t)n);
    if (n == 2 || n == 3) return true;
    if ((n & 1) == 0) return false;

    /* write n-1 as d * 2^s with d odd */
    seq_t d = n - 1;
    unsigned s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        s++;
    }

    /* deterministic bases covering 128‑bit values */
    static const int base_list[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    const int num_bases = sizeof(base_list)/sizeof(base_list[0]);

    for (int i = 0; i < num_bases; i++) {
        seq_t a = (seq_t)base_list[i];
        if (a >= n) continue;

        /* compute a^d mod n */
        seq_t x = 1;
        seq_t pow = a % n;
        seq_t exp = d;
        while (exp > 0) {
            if (exp & 1) x = (x * pow) % n;
            pow = (pow * pow) % n;
            exp >>= 1;
        }

        if (x == 1 || x == n-1) continue;

        bool cont = false;
        for (unsigned r = 1; r < s; r++) {
            x = (x * x) % n;
            if (x == n-1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

/* Returns the nth term of OEIS A127177. */
seq_t a(idx_t n)
{
    /* static caches -------------------------------------------------------- */
    static seq_t *term_cache = NULL;
    static size_t term_cap   = 0;
    static size_t term_cnt   = 0;

    static uint64_t *prime_cache = NULL;
    static size_t prime_cap     = 0;
    static size_t prime_cnt     = 0;
    static uint64_t next_candidate = 2;

    /* ensure term cache space                                  */
    if (term_cache == NULL) {
        term_cap = 64;
        term_cache = mem_new(seq_t, term_cap);
        term_cnt   = 0;
    }

    /* ensure prime cache space                                 */
    if (prime_cache == NULL) {
        prime_cap = 64;
        prime_cache = (uint64_t *)malloc(prime_cap * sizeof(uint64_t));
        prime_cnt   = 0;
    }

    /* generate terms until we have at least n of them -------- */
    while (term_cnt < n) {
        /* ----------------------------------------------------------------
           Find the next prime number and store it in prime_cache.          */
        uint64_t p;
        while (1) {
            bool is_p = true;

            if (next_candidate < 2) {
                is_p = false;
            } else if (next_candidate == 2) {
                is_p = true;
            } else if (next_candidate % 2 == 0) {
                is_p = false;
            } else {
                for (size_t i = 0; i < prime_cnt; ++i) {
                    uint64_t pr = prime_cache[i];
                    if (pr * pr > next_candidate) break;
                    if (next_candidate % pr == 0) {
                        is_p = false;
                        break;
                    }
                }
            }

            if (is_p) {
                /* store prime for future divisor tests */
                if (prime_cnt == prime_cap) {
                    prime_cap *= 2;
                    prime_cache = (uint64_t *)realloc(prime_cache, prime_cap * sizeof(uint64_t));
                }
                prime_cache[prime_cnt++] = next_candidate;
                p = next_candidate;
                break;
            }

            /* advance to next candidate (odd after 2) */
            next_candidate = (next_candidate == 2) ? 3 : next_candidate + 2;
        }

        /* ----------------------------------------------------------------
           Compute the right‑to‑left Look‑And‑Say description of p.           */
        uint64_t tmp = p;
        seq_t cur = 0;
        int prev_digit = -1;
        int run_len = 0;

        while (tmp > 0) {
            int digit = tmp % 10;
            tmp /= 10;

            if (prev_digit == -1) {
                prev_digit = digit;
                run_len   = 1;
                continue;
            }
            if (digit == prev_digit) {
                ++run_len;
                continue;
            }

            /* finalize previous run */
            seq_t pair_value = (seq_t)run_len * 10 + (seq_t)prev_digit;
            int pair_len = num_digits((seq_t)run_len) + 1;
            cur = cur * pow10((idx_t)pair_len) + pair_value;

            prev_digit = digit;
            run_len    = 1;
        }

        /* finalize last run if any */
        if (prev_digit != -1) {
            seq_t pair_value = (seq_t)run_len * 10 + (seq_t)prev_digit;
            int pair_len = num_digits((seq_t)run_len) + 1;
            cur = cur * pow10((idx_t)pair_len) + pair_value;
        }

        /* ----------------------------------------------------------------
           Test the description for primality; if prime, add p to term cache. */
        if (is_prime_big(cur)) {
            if (term_cnt == term_cap) {
                term_cap *= 2;
                term_cache = mem_resize(seq_t, term_cache, term_cap);
            }
            term_cache[term_cnt++] = (seq_t)p;
        }

        /* advance to the next candidate for prime generation           */
        next_candidate = (p == 2) ? 3 : p + 2;
    }

    return term_cache[n - 1];
}
