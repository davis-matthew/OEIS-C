/*
 * A072904  Nonsquares m such that the discriminant of the quadratic field
 *          Q(sqrt(m)) < m.
 * https://oeis.org/A072904
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 57, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) from OEIS A072904
 * a(n) is the nth positive integer m that is not a perfect square and satisfies
 * the discriminant D(m) of ℚ(√m) being strictly less than m.
 * The function scans successive integers, factoring each to test the
 * condition. It memoises found terms so that calls are fast and order
 * independent. */

#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* padding for n < 1, unused */

    static seq_t *cache = NULL;    /* memoised terms */
    static size_t cache_cap = 0;   /* capacity of cache array */
    static size_t cache_len = 0;   /* number of terms stored */
    static idx_t next_m = 2;       /* next integer to test */

    /* extend cache if needed */
    while (cache_len < (size_t)n) {
        /* ensure adequate capacity */
        if (cache_len + 1 > cache_cap) {
            size_t new_cap = cache_cap ? cache_cap * 2 : 64;
            if (new_cap < cache_len + 1) new_cap = cache_len + 1;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            cache = tmp;
            cache_cap = new_cap;
        }

        /* search for next qualifying integer */
        while (1) {
            seq_t m_val = (seq_t)next_m;  /* candidate integer */

            /* factor next_m to obtain squarefree part A and square factor B */
            idx_t tmp = next_m;
            seq_t A = 1;        /* squarefree part */
            seq_t B = 1;        /* product of primes raised to ⌊e/2⌋ */
            seq_t modA = 1;     /* A modulo 4 (needed for the condition) */

            for (idx_t p = 2; p * p <= tmp; ++p) {
                if (tmp % p == 0) {
                    int exp = 0;
                    while (tmp % p == 0) {
                        tmp /= p;
                        exp++;
                    }
                    if (exp & 1) {            /* odd exponent contributes to A */
                        A *= (seq_t)p;
                        modA = (modA * ((seq_t)(p % 4))) % 4;
                    }
                    for (int k = 0; k < exp / 2; ++k) { /* contributes to B */
                        B *= (seq_t)p;
                    }
                }
            }
            if (tmp > 1) {                  /* remaining prime factor */
                A *= (seq_t)tmp;
                modA = (modA * ((seq_t)(tmp % 4))) % 4;
            }

            /* skip perfect squares: A == 1 */
            if (A == 1) {
                next_m++;
                continue;
            }

            /* check discriminant condition */
            bool good;
            if (modA == 1)          /* squarefree part ≡ 1 (mod 4) */
                good = (B >= 2);    /* need at least one non‑trivial square factor */
            else                     /* squarefree part ≡ 2 or 3 (mod 4) */
                good = (B >= 3);    /* need square factor ≥ 9 */

            if (good) {
                cache[cache_len++] = m_val;   /* store term */
                break;
            }

            next_m++;               /* try next integer */
        }
        next_m++;                   /* start next search from the following integer */
    }

    return cache[n - 1];
}
