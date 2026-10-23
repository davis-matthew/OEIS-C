/*
 * A112381  Zero-free semiprimes.
 * https://oeis.org/A112381
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 59, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   is_semiprime_uint64, push_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Zero‑free semiprimes: the product of exactly two primes that contains no digit 0.
   The nth term a(n) is the nth such integer (a(1) = 4). */
#include <stdint.h>
#include <stdbool.h>

#include "../utils/is_semiprime_uint64.h"
#include "../utils/push_seq.h"

/* Return true if x contains the digit 0. */
static bool has_zero_digit(seq_t x)
{
    while (x > 0)
    {
        if ((x % (seq_t)10) == 0)
            return true;
        x /= 10;
    }
    return false;
}

seq_t a(idx_t n)
{
    /* Memoised list of zero‑free semiprimes. */
    static seq_t *table = NULL;
    static idx_t sz   = 0;   /* number of terms stored */
    static idx_t cap  = 0;   /* capacity of the array */
    static idx_t cand = 2;   /* next integer to test */

    while (sz < n)
    {
        uint64_t u = (uint64_t)cand;
        if (is_semiprime_uint64(u) && !has_zero_digit(cand))
        {
            push_seq(&table, &cap, &sz, (seq_t)cand);
        }
        ++cand;
    }

    return table[n - 1];
}
