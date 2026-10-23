/*
 * A108181  Semiprimes of the form 4n + 1.
 * https://oeis.org/A108181
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 29, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n-th semiprime of the form 4k+1 by simple trial division.
   The function generates numbers that are ≡ 1 (mod 4) and tests whether
   they have exactly two (not necessarily distinct) prime factors. */
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* Return true iff x is a semiprime (product of exactly two prime factors). */
static bool is_semiprime_uint64(uint64_t x)
{
    if (x < 4) return false;          /* smallest semiprime is 2*2 = 4 (not ≡1 mod4) */
    uint64_t cnt = 0;
    uint64_t n = x;
    for (uint64_t p = 2; p * p <= n; ++p) {
        while (n % p == 0) {
            ++cnt;
            if (cnt > 2) return false;
            n /= p;
        }
    }
    if (n > 1) ++cnt;                  /* remaining prime factor */
    return cnt == 2;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;       /* not defined for n=0, just return 0 */

    static seq_t *list   = NULL;
    static idx_t  cap    = 0;
    static idx_t  count  = 0;
    static uint64_t cur  = 9;          /* first integer ≡1 (mod4) that can be semiprime */

    /* Ensure enough space for the requested index. */
    if (n > cap) {
        size_t newcap = cap == 0 ? 16 : cap * 2;
        if (newcap < n) newcap = n;
        seq_t *tmp = (seq_t *)realloc(list, newcap * sizeof(seq_t));
        list = tmp;
        cap  = (idx_t)newcap;
    }

    /* Generate further terms until we have n of them. */
    while (count < n) {
        if ((cur & 3ULL) == 1ULL && is_semiprime_uint64(cur)) {
            list[count] = (seq_t)cur;
            ++count;
        }
        ++cur;
    }

    /* Return the requested term. */
    return list[n - 1];
}
