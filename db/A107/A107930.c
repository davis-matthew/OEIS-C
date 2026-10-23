/*
 * A107930  Least number m such that m and m-th prime have only one common
 *          digit = n.
 * https://oeis.org/A107930
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  fini, full, nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 9, u32 n <= 9, u64 n <= 9, u128 n <= 9,
 *            big any n
 * Helpers:   single_bit_pos
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: a(n) = least m such that the decimal representation of m
 * and the m-th prime have exactly one common digit, that digit being n.
 * The sequence is finite (ten terms for n = 0…9) and is computed once
 * by a brute‑force search. */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/single_bit_pos.h"

/* Helper: compute mask of digits present in x */
static inline int digit_mask(uint64_t x)
{
    int mask = 0;
    if (x == 0) return 1;          /* digit 0 */
    while (x > 0) {
        mask |= 1 << (int)(x % 10);
        x /= 10;
    }
    return mask;
}

/* Helper: count number of set bits in a small mask (max 10 bits) */
static inline int popcnt(int mask)
{
    int c = 0;
    while (mask) {
        c += mask & 1;
        mask >>= 1;
    }
    return c;
}

/* Simple primality test (trial division).  Numbers here are < 2000. */
static inline bool is_prime(uint64_t n)
{
    if (n < 2) return false;
    if (n % 2 == 0) return n == 2;
    for (uint64_t d = 3; d * d <= n; d += 2)
        if (n % d == 0)
            return false;
    return true;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    static seq_t cache[10] = {0};
    static bool initialized = false;

    if (!initialized) {
        /* Find all ten values by brute force. */
        uint64_t m = 1;          /* candidate m (index into primes) */
        uint64_t prime_candidate = 2;  /* first prime */

        int remaining = 10;      /* digits 0–9 still missing */

        while (remaining > 0) {
            /* Prime for current m is prime_candidate */
            int m_mask = digit_mask(m);
            int p_mask = digit_mask(prime_candidate);
            int inter = m_mask & p_mask;

            if (inter && popcnt(inter) == 1) {
                int digit = single_bit_pos(inter);
                if (digit >= 0 && digit < 10 && cache[digit] == 0) {
                    cache[digit] = (seq_t)m;
                    --remaining;
                }
            }

            /* Move to next m and next prime. */
            ++m;
            ++prime_candidate;
            while (!is_prime(prime_candidate))
                ++prime_candidate;
        }

        initialized = true;
    }

    if (n < 10)
        return cache[(int)n];
    /* For undefined n ≥ 10, return 0 (sequence defined only for 0–9). */
    return (seq_t)0;
}
