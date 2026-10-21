/*
 * A230807  The smallest of n consecutive palindromic numbers which are all
 *          primes.
 * https://oeis.org/A230807
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 2, u32 none, u64 n <= 4, u128 n <= 4,
 *            big any n
 * Helpers:   is_prime_big, make_palind
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Finds the smallest of n consecutive palindromic primes.
   For n >= 5 the sequence is empty, so this function returns 0.  The
   function generates palindromic numbers in strictly increasing order,
   tests each for primality (using the provided is_prime_big routine),
   keeps a sliding window of the last four prime palindromes, and
   records the first run of 1, 2, 3 and 4 consecutive prime palindromes.
   Subsequent calls can be made in any order. */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime_big.h"
#include "../utils/make_palind.h"

static seq_t sol[5] = {0, 0, 0, 0, 0};      /* sol[1] .. sol[4] are results */
static bool cached[5] = {false, false, false, false, false};
static idx_t cached_max = 0;

/* Generate palindromes in increasing order and record smallest runs. */
static void precompute(void) {
    if (cached_max >= 4) return;         /* already computed */

    idx_t pal_idx = 1;                     /* index of current palindrome */
    uint64_t prime_idx[4] = {0};
    seq_t   prime_val[4] = {0};
    int      prime_cnt = 0;                /* number of primes in buffer */

    for (int len = 1; ; ++len) {
        int half_len;
        uint64_t start, end;
        bool odd = (len % 2 != 0);

        if (odd) {
            half_len = (len + 1) / 2;
            start = 1;
            for (int i = 1; i < half_len; ++i) start *= 10;
            end   = start * 10 - 1;
        } else {
            half_len = len / 2;
            start = 1;
            for (int i = 1; i < half_len; ++i) start *= 10;
            end   = start * 10 - 1;
        }

        for (uint64_t h = start; h <= end; ++h) {
            seq_t pal = make_palind(h, odd);
            if (is_prime_big(pal)) {
                /* store the new prime in the sliding window */
                if (prime_cnt < 4) {
                    prime_idx[prime_cnt] = pal_idx;
                    prime_val[prime_cnt] = pal;
                    ++prime_cnt;
                } else {
                    for (int i = 0; i < 3; ++i) {
                        prime_idx[i] = prime_idx[i + 1];
                        prime_val[i] = prime_val[i + 1];
                    }
                    prime_idx[3] = pal_idx;
                    prime_val[3] = pal;
                }

                /* record answers */
                if (!cached[1]) { sol[1] = pal; cached[1] = true; }
                for (int k = 2; k <= prime_cnt && k <= 4; ++k) {
                    int start_i = prime_cnt - k;
                    if (prime_idx[start_i] + k - 1 == prime_idx[prime_cnt - 1]) {
                        if (!cached[k]) {
                            sol[k] = prime_val[start_i];
                            cached[k] = true;
                        }
                    }
                }

                if (cached[1] && cached[2] && cached[3] && cached[4]) {
                    cached_max = 4;
                    return;
                }
            }
            ++pal_idx;
        }
    }
}

seq_t a(idx_t n) {
    if (n == 0) return 0;
    if (n >= 5)   return 0;
    precompute();
    return sol[n];
}
