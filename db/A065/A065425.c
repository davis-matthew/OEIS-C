/*
 * A065425  Smallest odd prime greater than n such that the decimal expansion
 *          of its base n conversion is also prime.
 * https://oeis.org/A065425
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 67, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = smallest odd prime > n such that the decimal concatenation of its base‑n digits is prime. */

#include <stdint.h>
#include <stdbool.h>

/* ---------- small primality test (works for values up to 64‑bit) ---------- */
static bool is_prime_uint64(uint64_t x)
{
    if (x < 2)     return false;
    if (x == 2 || x == 3) return true;
    if ((x & 1) == 0) return false;
    for (uint64_t d = 3; d * d <= x; d += 2) {
        if (x % d == 0) return false;
    }
    return true;
}

/* ---------- find the next odd prime > start ---------- */
static uint64_t next_prime_uint64(uint64_t start)
{
    if (start <= 2) return 3;
    if ((start & 1) == 0) start++;        /* make odd */
    for (;;) {
        if (is_prime_uint64(start)) return start;
        start += 2;
    }
}

/* ---------- number of decimal digits of a non‑negative integer d (d < 10000) ---------- */
static unsigned int dec_len(uint32_t d)
{
    if (d >= 1000) return 4;
    if (d >= 100)  return 3;
    if (d >= 10)   return 2;
    return 1;
}

/* ---------- compute the integer obtained by concatenating decimal strings of base‑n digits ---------- */
static uint64_t base_n_to_decimal(uint64_t p, uint64_t n)
{
    /* collect base‑n digits (least significant first) */
    uint32_t digits[64];
    int cnt = 0;
    uint64_t tmp = p;
    if (tmp == 0) {
        digits[cnt++] = 0;
    } else {
        while (tmp > 0) {
            digits[cnt++] = (uint32_t)(tmp % n);
            tmp /= n;
        }
    }

    /* assemble decimal concatenation */
    uint64_t rep = 0;
    for (int i = cnt - 1; i >= 0; --i) {
        uint32_t d = digits[i];
        unsigned int len = dec_len(d);
        /* multiply current rep by 10^len */
        switch (len) {
            case 4: rep *= 10000; break;
            case 3: rep *= 1000;  break;
            case 2: rep *= 100;   break;
            case 1: rep *= 10;    break;
        }
        rep += d;
    }
    return rep;
}

/* ---------- main sequence function ---------- */
seq_t a(idx_t n)
{
    if (n < 2) return (seq_t)0;      /* offset is 2 */

    uint64_t candidate = next_prime_uint64(n + 1);

    for (;;) {
        uint64_t rep = base_n_to_decimal(candidate, n);
        if (is_prime_uint64(rep)) {
            return (seq_t)candidate;
        }
        candidate = next_prime_uint64(candidate + 1);
    }
}
