/*
 * A132329  Numbers that cannot be expressed as the difference of the squares
 *          of primes.
 * https://oeis.org/A132329
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Validated: partial, pass, 72 terms through n = 72 (2026-11-03)
 * Helpers:   is_prime_u64, isqrt_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers that cannot be expressed as the difference of squares of primes.
   The function checks each integer sequentially, testing whether it can
   be written as p²−q² for primes p>q. It counts those that cannot and
   returns the nth such number. */
#include <stdbool.h>
#include <stdint.h>
#include "../utils/isqrt_seq.h"
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

seq_t a(idx_t n)
{
    idx_t count = 0;          /* how many non‑expressible numbers we have found */
    seq_t cand   = 1;         /* current candidate to test */

    for (;; ++cand)
    {
        bool expressible = false;

        /* factor cand into a*b with a ≤ b */
        seq_t limit = isqrt_seq(cand);
        for (seq_t a = 1; a <= limit; ++a)
        {
            if (cand % a != 0)
                continue;

            seq_t b = cand / a;
            if (a >= b)               /* need a<b to have q>0 */
                break;
            if ( (a & 1) != (b & 1) ) /* a and b must have same parity */
                continue;

            seq_t p = (a + b) >> 1;   /* p = (a+b)/2  */
            seq_t q = (b - a) >> 1;   /* q = (b-a)/2  */
            if (is_prime_big(p) && is_prime_big(q))
            {
                expressible = true;
                break;
            }
        }

        if (!expressible)
        {
            ++count;
            if (count == n)
                return cand;
        }
    }
}
