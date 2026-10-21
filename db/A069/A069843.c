/*
 * A069843  Number of distinct primes obtained by inserting a 2 at all
 *          possible places in n. This includes prefixing.
 * https://oeis.org/A069843
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence A069843:  
   a(n) = number of distinct primes obtained by inserting a digit 2
   in all possible positions of the decimal representation of n
   (including prefix). */

#include <stddef.h>
#include <stdbool.h>

/* Helper: deterministic Miller‑Rabin for 64‑bit integers.  */
static inline bool is_prime_u64(unsigned long long n)
{
    if (n < 2)          return false;
    if (n % 2 == 0)     return n == 2;
    if (n % 3 == 0)     return n == 3;
    unsigned long long d = 5;
    for (unsigned long long i = 5; i * i <= n; i += 6) {
        if (n % i == 0 || n % (i + 2) == 0) return false;
    }
    return true;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;               /* offset starts at 1 */

    /* 1. Extract the decimal digits of n in most‑significant‑first order. */
    uint8_t digits[20];
    idx_t len = 0;
    idx_t tmp = n;
    do {
        digits[len++] = tmp % 10;
        tmp /= 10;
    } while (tmp);
    /* reverse to get most significant first */
    for (idx_t i = 0; i < len / 2; ++i) {
        uint8_t t = digits[i];
        digits[i] = digits[len - 1 - i];
        digits[len - 1 - i] = t;
    }

    /* 2. Enumerate all numbers formed by inserting a 2 at every position. */
    idx_t distinct[30];
    idx_t dcnt = 0;
    for (idx_t pos = 0; pos <= len; ++pos) {
        idx_t val = 0;
        /* digits before the insertion point */
        for (idx_t i = 0; i < pos; ++i)
            val = val * 10 + digits[i];
        /* the inserted 2 */
        val = val * 10 + 2;
        /* digits after the insertion point */
        for (idx_t i = pos; i < len; ++i)
            val = val * 10 + digits[i];

        if (!is_prime_u64(val)) continue;

        /* 3. Count each distinct prime only once. */
        bool seen = false;
        for (idx_t k = 0; k < dcnt; ++k) {
            if (distinct[k] == val) { seen = true; break; }
        }
        if (!seen) distinct[dcnt++] = val;
    }

    return (seq_t)dcnt;
}
