/*
 * A129739  Primes in Somos-4 sequence (A006720).
 * https://oeis.org/A129739
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   primes in the Somos‑4 sequence. Recurrence: a(n)*a(n‑4) =
 *            a(n‑1)*a(n‑3) + a(n‑2)² with a(1) = a(2) = a(3) = a(4) = 1. The
 *            function returns the n‑th prime appearing in that sequence
 * Range:     u8 n <= 5, u16 n <= 5, u32 n <= 7, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Validated: partial, pass, 10 terms through n = 10 (2026-11-03)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
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

static seq_t primes[20000];
static size_t prime_cnt = 0;
static seq_t last[4];
static bool initialized = false;

seq_t a(idx_t n)
{
    /* n ≥ 1 by contract; guard against zero for safety */
    if (n == 0) return 0;

    if (!initialized) {
        last[0] = last[1] = last[2] = last[3] = (seq_t)1;
        prime_cnt = 0;
        initialized = true;
    }

    /* Generate terms until we have at least n primes collected. */
    while (prime_cnt < (size_t)n) {
        /* Compute next term of the Somos‑4 recurrence */
        seq_t num  = last[3] * last[1] + last[2] * last[2];
        seq_t next = num / last[0];

        /* Shift the four‑term window to keep a(n‑4)…a(n‑1) for the next step */
        last[0] = last[1];
        last[1] = last[2];
        last[2] = last[3];
        last[3] = next;

        /* Store the term if it is prime. */
        if (is_prime_big(next)) {
            if (prime_cnt < 20000) {
                primes[prime_cnt++] = next;
            }
        }
    }

    return primes[n - 1];
}
