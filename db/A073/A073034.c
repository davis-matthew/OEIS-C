/*
 * A073034  Take A000040, omit commas: 23571113171923..., select 1-digit
 *          primes.
 * https://oeis.org/A073034
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 67, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth one‑digit prime digit in the concatenation of all primes.
   The function generates primes on the fly and records the digits 2, 3, 5,
   7 as they appear, without any hard‑coded table. */

/* Note: this implementation stops adding digits to the array only after a whole
   prime has been processed, so all digits of that prime are available for
   later calls.  */
#include <stddef.h>
#include <stdbool.h>

static bool is_prime_candidate(idx_t val, const idx_t *primes, size_t primes_cnt) {
    for (size_t i = 0; i < primes_cnt; ++i) {
        idx_t p = primes[i];
        if (p > val / p) break;          /* avoid overflow */
        if (val % p == 0) return false;
    }
    return true;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;               /* undefined, guard against underflow */

    /* static state across calls */
    static idx_t *prime_list = NULL;
    static size_t prime_cnt = 0, prime_cap = 0;
    static idx_t next_candidate = 2;

    static seq_t *digit_list = NULL;
    static size_t digit_cnt = 0, digit_cap = 0;

    /* helper to grow prime array */
    if (prime_cnt + 1 > prime_cap) {
        size_t new_cap = prime_cap ? prime_cap * 2 : 16;
        idx_t *tmp = (idx_t *)realloc(prime_list, new_cap * sizeof(idx_t));
        if (!tmp) return 0;               /* allocation failure */
        prime_list = tmp;
        prime_cap = new_cap;
    }

    /* helper to grow digit array */
    if (digit_cnt + 1 > digit_cap) {
        size_t new_cap = digit_cap ? digit_cap * 2 : 32;
        seq_t *tmp = (seq_t *)realloc(digit_list, new_cap * sizeof(seq_t));
        if (!tmp) return 0;
        digit_list = tmp;
        digit_cap = new_cap;
    }

    /* main loop: produce primes and collect qualifying digits */
    while (digit_cnt < (size_t)n) {
        bool is_prime_flag = false;
        if (next_candidate == 2) {
            is_prime_flag = true;
        } else if ((next_candidate & 1) &&
                   is_prime_candidate(next_candidate, prime_list, prime_cnt)) {
            is_prime_flag = true;
        }

        if (is_prime_flag) {
            /* store the new prime */
            prime_list[prime_cnt++] = next_candidate;

            /* split the prime into its decimal digits (most to least) */
            idx_t tmp = next_candidate;
            idx_t rev[32];
            size_t rev_len = 0;
            if (tmp == 0) {
                rev[rev_len++] = 0;
            } else {
                while (tmp > 0) {
                    rev[rev_len++] = tmp % 10;
                    tmp /= 10;
                }
            }

            for (size_t j = 0; j < rev_len; ++j) {
                seq_t d = rev[rev_len - 1 - j];
                if (d == 2 || d == 3 || d == 5 || d == 7) {
                    /* ensure capacity for each push */
                    if (digit_cnt + 1 > digit_cap) {
                        size_t new_cap = digit_cap ? digit_cap * 2 : 32;
                        seq_t *tmp = (seq_t *)realloc(digit_list,
                                                      new_cap * sizeof(seq_t));
                        if (!tmp) return 0;
                        digit_list = tmp;
                        digit_cap = new_cap;
                    }
                    digit_list[digit_cnt++] = d;
                }
            }
        }

        ++next_candidate;
    }

    return digit_list[n - 1];
}
