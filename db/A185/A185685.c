/*
 * A185685  Irregular triangle, read by rows, of primes with prefix n and
 *          digits "7" appended, otherwise 0.
 * https://oeis.org/A185685
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, less, tabf
 * Formula:   a(n) is built row‑by‑row. For each integer r ≥ 1 generate the
 *            numbers r*10+7, (r*10+7)*10+7, … . All primes encountered before
 *            the first composite in the row are output in order. If a row
 *            yields no primes, a single 0 is output
 * Range:     u8 n <= 6, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Validated: partial, pass, 70 terms through n = 70 (2026-11-03)
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

seq_t a(idx_t n)
{
    idx_t count = 0;                          /* number of terms produced so far */
    unsigned __int128 max_seq = (unsigned __int128)((seq_t)~0); /* maximal representable value */

    for (idx_t row = 1; ; ++row)
    {
        unsigned __int128 cur = row;          /* start generating from the row number */
        bool any_prime = false;               /* whether a prime has been produced in this row */
        bool overflow = false;

        while (true)
        {
            cur = cur * 10 + 7;               /* append a digit 7 */

            if (cur > max_seq)
            {
                overflow = true;              /* number cannot be represented in seq_t */
                break;
            }

            seq_t num = (seq_t)cur;

            if (is_prime_big(num))
            {
                any_prime = true;
                ++count;
                if (count == n)
                    return num;               /* prime found */
                /* continue searching for primes in this row */
            }
            else /* composite encountered – row ends */
            {
                if (!any_prime)
                {
                    ++count;
                    if (count == n)
                        return (seq_t)0;        /* row had no primes */
                }
                break;                       /* move to the next row */
            }
        }

        if (overflow && !any_prime)
        {
            /* row exceeded representable range and produced no primes */
            ++count;
            if (count == n)
                return (seq_t)0;
        }
        /* otherwise proceed to the next row */
    }
}
