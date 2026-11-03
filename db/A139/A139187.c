/*
 * A139187  Smallest twin prime member A001359 of the form k!/n-1.
 * https://oeis.org/A139187
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = the smallest integer k!/n - 1 such that * (k!/n - 1) and
 *            (k!/n + 1) are twin primes. * This implementation searches k
 *            starting at 1 until a valid pair * is found, memoizing results
 *            for subsequent calls
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Validated: partial, pass, 6 terms through n = 6 (2026-11-03)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

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

/* Cache for results up to 10000 (typical test range) */
static seq_t cache[10001];
static bool seen[10001] = {0};

seq_t a(idx_t n)
{
    /* Handle trivial invalid input - sequence defined for n >= 1 */
    if (n == 0)
        return 0;

    /* Return cached value if available */
    if (n <= 10000 && seen[n])
        return cache[n];

    seq_t fact = 1;              /* current factorial value (k!) */
    idx_t   mod  = 1 % n;        /* n modulo for divisibility test */

    for (idx_t k = 1; ; ++k)
    {
        /* Increment factorial: k! = (k-1)! * k */
        fact = fact * k;

        /* Update modulo progressively */
        mod = (mod * (k % n)) % n;

        /* Only interesting if n divides k! (i.e., mod == 0) */
        if (mod == 0)
        {
            /* Candidate twin prime:  p = fact / n - 1  */
            seq_t p  = fact / n - 1;
            seq_t p2 = p + 2;   /* upper twin prime */

            /* Check twin prime condition */
            if (is_prime_big(p) && is_prime_big(p2))
            {
                if (n <= 10000)
                {
                    cache[n] = p;
                    seen[n]  = true;
                }
                return p;
            }
        }
        /* The loop is guaranteed to terminate for all n ≥ 1
         * because the sequence is defined and an answer exists.
         */
    }
}
