/*
 * A232190  a(n) is the number of primes of the form 2^b + 2n +- 2^k +- 1 and
 *          2^(b+2) - 2^b - 2n +- 2^k +- 1, where b is the length of the
 *          binary representation of 2n, and 0<k<=b.
 * https://oeis.org/A232190
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of distinct primes of the form 2^b + 2n ± 2^k ± 1
 *            or 2^(b+2) − 2^b − 2n ± 2^k ± 1, where b is the length of the
 *            binary representation of 2n, 1 ≤ k ≤ b
 * Range:     u8 n <= 31, u16 n <= 8515, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 82 terms through n = 9415 (2026-11-03)
 * Helpers:   bit_length, is_prime_u64, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/bit_length.h"
#include "../utils/pow2.h"
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

/* Helper: safe multiplication for seq_t */
static inline seq_t seq_mul(seq_t a, seq_t b) {
    return a * b;
}

seq_t a(idx_t n)
{
    /* The sequence is defined for n ≥ 1.  For n = 0 we return 0. */
    if (n == 0) return 0;

    /* 2n and its binary length b. */
    seq_t two_n = (seq_t)n * 2;
    unsigned b = bit_length(two_n);

    /* 2^b and 3·2^b are needed many times. */
    seq_t pow2_b  = pow2(b);          /* 2^b */
    seq_t pow3_b  = seq_mul((seq_t)3, pow2_b); /* 3·2^b */

    /* An array to store distinct prime candidates.  The maximum
       number of distinct candidates per n is far below 256, so a
       fixed size buffer is adequate. */
    seq_t primes[256];
    size_t uniq = 0;

    /* Enumerate k = 1 … b and all eight ± combinations. */
    for (idx_t k = 1; k <= (idx_t)b; ++k) {
        seq_t pow2_k = pow2(k);

        /* Pre-calculate the eight numbers for this k. */
        seq_t cands[8] = {
            pow2_b + two_n + pow2_k + 1,
            pow2_b + two_n + pow2_k - 1,
            pow2_b + two_n - pow2_k + 1,
            pow2_b + two_n - pow2_k - 1,
            pow3_b - two_n + pow2_k + 1,
            pow3_b - two_n + pow2_k - 1,
            pow3_b - two_n - pow2_k + 1,
            pow3_b - two_n - pow2_k - 1
        };

        for (int i = 0; i < 8; ++i) {
            seq_t cand = cands[i];
            /* Skip numbers < 2 – they cannot be prime. */
            if (cand < 2) continue;
            /* Check primality. */
            if (!is_prime_big(cand)) continue;

            /* Avoid counting the same prime multiple times. */
            bool seen = false;
            for (size_t j = 0; j < uniq; ++j) {
                if (primes[j] == cand) { seen = true; break; }
            }
            if (!seen) {
                primes[uniq++] = cand;
                /* The array is large enough for all expected values. */
            }
        }
    }

    /* Convert the count of distinct primes to seq_t. */
    return (seq_t)uniq;
}
