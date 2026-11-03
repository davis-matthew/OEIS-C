/*
 * A081447  Smallest squares such that partial sums of the sequence plus 5 are
 *          primes.
 * https://oeis.org/A081447
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Validated: partial, pass, 55 terms through n = 55 (2026-11-03)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search-based implementation: For each n, a(n) is the smallest square
 * that is a multiple of 36, (6m)^2, such that the cumulative sum of the
 * first n terms plus 5 is a prime. This guarantees a(1)=36, a(2)=576,
 * etc., matching OEIS A081447. */

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

#define MAX_CACHE 100000   /* sufficient for n up to about 10000 */

static seq_t terms[MAX_CACHE];   /* memoised terms; index 0 corresponds to a(1) */
static idx_t cached_n = 0;       /* number of terms currently stored */
static seq_t total_sum = 0;      /* sum of all stored terms */

/* Return the nth term (1‑based) of sequence A081447 */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;  /* undefined; safeguard */

    /* compute any missing terms up to n */
    while (cached_n < n)
    {
        /* find smallest (6m)^2 making the new sum +5 prime */
        seq_t chosen = 0;
        for (idx_t m = 1; ; ++m)
        {
            seq_t m_sq = (seq_t)m * (seq_t)m;   /* m^2 */
            seq_t square = 36 * m_sq;           /* (6m)^2 */
            seq_t test = total_sum + square + (seq_t)5;
            if (is_prime_big(test))
            {
                chosen = square;
                break;
            }
        }

        terms[cached_n] = chosen;
        total_sum += chosen;
        ++cached_n;
    }

    return terms[n - 1];
}
