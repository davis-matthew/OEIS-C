/*
 * A129822  Array read by rows: each row lists three consecutive nonprime odd
 *          numbers.
 * https://oeis.org/A129822
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 51, u16 n <= 10002, u32 n <= 10002, u64 n <= 10002,
 *            u128 n <= 10002, big any n
 * Validated: standard, pass, 68 terms through n = 9747 (2026-11-03)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: a(n) is the nth entry obtained by reading an array whose rows
   consist of three consecutive odd non‑prime numbers spaced by 2.  
   The array is formed by scanning the odd composite numbers in order
   and selecting each triplet (s, s+2, s+4) that is fully composite.
   The sequence is the row‑major traversal of that array. */

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
    /* Each row has 3 entries: positions 0,1,2 correspond to
       the starting composite s, s+2, s+4. */
    idx_t row = (n - 1) / 3;           /* zero‑based row number   */
    int   col = (int)((n - 1) % 3);    /* zero‑based column number*/

    idx_t count = 0;                   /* number of qualifying rows found */

    /* Scan odd composite numbers starting from 9. */
    for (idx_t i = 9; ; i += 2)
    {
        seq_t val  = (seq_t)i;         /* current odd number       */
        if (!is_prime_big(val))        /* composite check           */
        {
            seq_t val2 = val + 2;
            seq_t val4 = val + 4;
            if (!is_prime_big(val2) && !is_prime_big(val4))
            {
                if (count == row)     /* found the desired row   */
                {
                    return val + (seq_t)(2 * col);
                }
                ++count;                /* next qualifying row     */
            }
        }
    }
}
