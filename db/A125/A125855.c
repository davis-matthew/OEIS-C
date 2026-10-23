/*
 * A125855  Numbers k such that k+1, k+3, k+7 and k+9 are all primes.
 * https://oeis.org/A125855
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, changed
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 28, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence of numbers k such that k+1, k+3, k+7, k+9 are all primes.
   Computed by brute‑force search over k ≡ 10 (mod 30). */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Deterministic Miller–Rabin for 64‑bit integers. */
static bool is_prime_uint64(uint64_t n)
{
    if (n < 2) return false;

    /* Quick trial division by small primes. */
    static const uint64_t small[] =
      {2ULL,3ULL,5ULL,7ULL,11ULL,13ULL,17ULL,19ULL,23ULL,29ULL,31ULL,37ULL};
    for (size_t i = 0; i < sizeof(small)/sizeof(small[0]); ++i) {
        if (n == small[i]) return true;
        if (n % small[i] == 0) return false;
    }

    /* Write n-1 as d*2^s with d odd. */
    uint64_t d = n - 1;
    int s = 0;
    while ((d & 1ULL) == 0) {
        d >>= 1;
        ++s;
    }

    /* Bases sufficient for deterministic test up to 2^64. */
    static const uint64_t bases[] =
      {2ULL, 325ULL, 9375ULL, 28178ULL, 450775ULL, 9780504ULL, 1795265022ULL};
    for (size_t i = 0; i < sizeof(bases)/sizeof(bases[0]); ++i) {
        uint64_t a = bases[i] % n;
        if (a == 0) continue;

        /* Compute a^d mod n. */
        uint64_t x = 1, e = d;
        uint64_t base = a;
        while (e) {
            if (e & 1) x = (unsigned __int128)x * base % n;
            base = (unsigned __int128)base * base % n;
            e >>= 1;
        }

        if (x == 1 || x == n - 1) continue;

        bool composite = true;
        for (int r = 1; r < s; ++r) {
            x = (unsigned __int128)x * x % n;
            if (x == n - 1) {
                composite = false;
                break;
            }
        }
        if (composite) return false;
    }
    return true;
}

seq_t a(idx_t n)
{
    /* Static storage for memoisation. */
    static seq_t *seq = NULL;
    static size_t cap = 0;
    static size_t len = 0;
    static uint64_t next_k = 10;    /* next candidate of the form 10 + 30t */

    if (seq == NULL) { /* first call */
        cap = 16;
        seq = (seq_t *)malloc(cap * sizeof(seq_t));
        seq[0] = (seq_t)4;          /* a(1) = 4 */
        len  = 1;
        next_k = 10;
    }

    if (n == 0) return 0;             /* no zero index in OEIS */

    size_t target = (size_t)n;
    if (len < target) {
        while (len < target) {
            uint64_t k = next_k;
            if (is_prime_uint64(k + 1) &&
                is_prime_uint64(k + 3) &&
                is_prime_uint64(k + 7) &&
                is_prime_uint64(k + 9)) {
                if (len == cap) {
                    cap <<= 1;
                    seq = (seq_t *)realloc(seq, cap * sizeof(seq_t));
                }
                seq[len++] = (seq_t)k;
            }
            next_k += 30;              /* move to the next candidate */
        }
    }

    return seq[target - 1];
}
