/*
 * A230807  The smallest of n consecutive palindromic numbers which are all
 *          primes.
 * https://oeis.org/A230807
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 2, u32 n <= 4, u64 n <= 4, u128 n <= 4,
 *            big any n
 * Validated: full, pass, 4 terms through n = 4 (2026-11-03)
 * Helpers:   is_prime_u64
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
#include "../utils/is_prime_u64.h"

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

/* Construct an odd or even palindromic number from a half.
   If odd==true the middle digit is taken from half and not mirrored. */
static seq_t make_palind(const uint64_t half, bool odd) {
    seq_t pal = (seq_t)half;
    uint64_t t = odd ? half / 10 : half;
    while (t > 0) {
        pal = pal * 10 + (seq_t)(t % 10);
        t /= 10;
    }
    return pal;
}

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
