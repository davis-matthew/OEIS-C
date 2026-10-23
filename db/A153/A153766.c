/*
 * A153766  Numbers k such that 8k-9 is prime.
 * https://oeis.org/A153766
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 78, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A153766: Numbers k such that 8k-9 is prime.
   The function returns the n‑th such k for n ≥ 1.  The sequence
   is generated lazily and memoised; each call works independently
   of the order in which it is invoked.                                            */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* ------------------------------------------------------------------ */
/*  Deterministic Miller–Rabin primality test for 64‑bit unsigned ints */
static bool is_prime_uint64(uint64_t n)
{
    if (n < 2)                           return false;
    if (n % 2 == 0)                       return n == 2;
    if (n % 3 == 0)                       return n == 3;
    if (n % 5 == 0)                       return n == 5;
    if (n % 7 == 0)                       return n == 7;
    if (n % 11 == 0)                      return n == 11;
    if (n % 13 == 0)                      return n == 13;
    /* write n-1 as d*2^s with d odd */
    uint64_t d = n - 1;
    unsigned s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        ++s;
    }
    /* test set of bases that is deterministic for <2^64 */
    static const uint64_t bases[] = {2, 3, 5, 7, 11, 13, 17};
    for (size_t i = 0; i < sizeof(bases)/sizeof(bases[0]); ++i) {
        uint64_t a = bases[i];
        if (a % n == 0) return true;   /* base ≡ 0 (mod n) ⇒ n is prime */
        /* compute a^d mod n   */
        uint64_t x = 1;
        uint64_t base = a % n;
        uint64_t exp = d;
        while (exp) {
            if (exp & 1)
                x = (uint64_t)((__uint128_t)x * base % n);
            base = (uint64_t)((__uint128_t)base * base % n);
            exp >>= 1;
        }
        if (x == 1 || x == n - 1) continue;
        bool cont = false;
        for (unsigned r = 1; r < s; ++r) {
            x = (uint64_t)((__uint128_t)x * x % n);
            if (x == n - 1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}
/* ------------------------------------------------------------------ */

seq_t a(idx_t n)
{
    /* n is 1‑based; 0 is outside the defined range */
    if (n == 0) return 0;

    /* static memoisation state */
    static seq_t *seq = NULL;
    static size_t seq_len = 0;   /* how many terms we currently know */
    static size_t seq_cap = 0;   /* capacity of seq array       */
    static uint64_t next_k = 2;  /* candidate k to test next     */

    /* grow the cache array if necessary */
    if (seq_len == seq_cap) {
        size_t new_cap = seq_cap == 0 ? 8 : seq_cap * 2;
        seq_t *tmp = (seq_t*)realloc(seq, new_cap * sizeof(seq_t));
        if (!tmp) return 0;       /* allocation failure – return 0 */
        seq = tmp;
        seq_cap = new_cap;
    }

    /* generate terms until we have at least n values */
    while (seq_len < (size_t)n) {
        /* compute 8*next_k-9 without overflow (next_k fits in uint64_t) */
        uint64_t candidate = 8ULL * next_k - 9ULL;
        if (is_prime_uint64(candidate)) {
            seq[seq_len++] = (seq_t)next_k;
        }
        ++next_k;
    }

    return seq[n - 1];
}
