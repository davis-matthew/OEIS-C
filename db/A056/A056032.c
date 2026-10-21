/*
 * A056032  Trimorphic but not bimorphic nor automorphic.
 * https://oeis.org/A056032
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) is the nth positive integer n such that n^3 ≡ n (mod 10^d)
 *            (trimorphic) but n^2 ≠ n (mod 10^d) and n^4 ≠ n (mod 10^d) where
 *            d is the number of decimal digits of n. The function builds the
 *            sequence on-demand, caching past terms so that any n ≥ 1 can be
 *            accessed in O(n) total time
 * Range:     u8 n <= 7, u16 n <= 28, u32 n <= 33, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Cache of computed terms. */
    static seq_t *cache      = NULL;
    static size_t cache_cap  = 0;  /* allocated size of cache  */
    static size_t cache_size = 0;  /* number of terms stored   */

    /* Candidate generator state. */
    static seq_t next  = 1;  /* current integer being tested */
    static seq_t pow10 = 10; /* 10^digits of next */
    static idx_t digits = 1; /* number of digits of next */

    /* Ensure we have at least n terms. */
    while (cache_size < n) {
        /* If we have reached a new power of ten, update the digit count
           and modulus before testing.  This keeps 'next' having the
           correct number of digits. */
        if (next == pow10) {
            digits++;
            pow10 = pow10 * 10;   /* 10^digits  */
        }

        seq_t mod = pow10;        /* 10^digits  */

        /* Compute n^2, n^3, n^4 modulo mod using seq_t arithmetic. */
        seq_t sq  = (next * next) % mod;          /* n^2 mod mod */
        seq_t cube = (sq * next) % mod;           /* n^3 mod mod */
        seq_t n4   = (cube * next) % mod;         /* n^4 mod mod */

        /* Trimorphic but not automorphic nor bimorphic? */
        if (cube == next && sq != next && n4 != next) {
            /* Grow the cache if necessary. */
            if (cache_size == cache_cap) {
                size_t new_cap = (cache_cap == 0) ? 4 : cache_cap * 2;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_size++] = next;
        }

        /* Move to the next candidate. */
        next = next + 1;
    }

    return cache[n - 1];
}
