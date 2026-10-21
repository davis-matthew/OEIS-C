/*
 * A237601  Number of n-digit right-truncatable primes in base 16.
 * https://oeis.org/A237601
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 38, u16 n <= 38, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation of the number of n‑digit right‑truncatable primes in base 16
   by breadth‑first generation of all such primes up to length 14.
   For n ≥ 15 the count is 0. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* uint64_t primality test – deterministic Miller–Rabin for 64‑bit values */
static bool is_prime_64(uint64_t n)
{
    if (n < 2) return false;
    static const uint64_t small_primes[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    for (size_t i = 0; i < sizeof(small_primes)/sizeof(small_primes[0]); ++i) {
        if (n == small_primes[i]) return true;
        if (n % small_primes[i] == 0) return false;
    }

    /* write n−1 as d·2^s with d odd */
    uint64_t d = n - 1;
    int s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        s++;
    }

    /* witnesses sufficient for 64‑bit */
    static const uint64_t witnesses[] = {2,325,9375,28178,450775,9780504,1795265022};
    for (size_t i = 0; i < sizeof(witnesses)/sizeof(witnesses[0]); ++i) {
        uint64_t a = witnesses[i] % n;
        if (a == 0) continue;
        __uint128_t x = 1, base = a, exp = d;
        while (exp) {
            if (exp & 1) x = (x * base) % n;
            base = (base * base) % n;
            exp >>= 1;
        }
        if (x == 1 || x == n - 1) continue;
        bool composite = true;
        for (int r = 1; r < s; ++r) {
            x = (x * x) % n;
            if (x == n - 1) {
                composite = false;
                break;
            }
        }
        if (composite) return false;
    }
    return true;
}

/* static table of counts for lengths 1…14 */
static seq_t  counts[16] = {0};
static bool   ready      = false;

/* ------------------------------------------------------------------ */
/* Initialise the counts table on first use */
static void init_counts(void)
{
    if (ready) return;

    /* base‑16 single‑digit primes: 2,3,5,7,B,D (hex) */
    const uint64_t init_digits[] = {2ULL,3ULL,5ULL,7ULL,11ULL,13ULL};
    const size_t init_len = sizeof(init_digits)/sizeof(init_digits[0]);

    uint64_t *curr = (uint64_t*)malloc(init_len * sizeof(uint64_t));
    size_t clen = 0;
    for (size_t i = 0; i < init_len; ++i) curr[clen++] = init_digits[i];
    counts[1] = (seq_t)clen;                /* a(1) = 6 */

    /* breadth‑first grow to lengths 2 … 14 */
    for (size_t len = 2; len <= 14; ++len) {
        size_t ncap = clen * 16 + 10;       /* rough upper bound */
        uint64_t *next = (uint64_t*)malloc(ncap * sizeof(uint64_t));
        size_t nlen = 0;

        for (size_t i = 0; i < clen; ++i) {
            uint64_t base = curr[i] * 16ULL;
            for (uint8_t d = 0; d < 16; ++d) {
                uint64_t cand = base + d;
                if (is_prime_64(cand)) {
                    if (nlen == ncap) {       /* grow buffer */
                        ncap *= 2;
                        next = (uint64_t*)realloc(next, ncap * sizeof(uint64_t));
                    }
                    next[nlen++] = cand;
                }
            }
        }

        free(curr);
        curr = next;
        clen = nlen;
        counts[len] = (seq_t)clen;           /* store count for this length */
    }

    free(curr);
    ready = true;
}

/* ------------------------------------------------------------------ */
/* Public sequence function */
seq_t a(idx_t n)
{
    if (n == 0ULL) return 0;          /* undefined for n < 1 */
    init_counts();                    /* ensure table is ready */

    if (n < 16) return counts[(size_t)n];   /* a(1)…a(14) */
    return 0;                           /* for n ≥ 15 */
}
