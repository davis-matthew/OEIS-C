/*
 * A139327  Write the first n^2 odd numbers consecutively in n rows of length
 *          n: a(n) = maximal number of primes in a row.
 * https://oeis.org/A139327
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = maximal number of primes in any row when the first * n^2
 *            odd numbers are written in n consecutive rows of length n. *
 *            Implementation uses a sieve of Eratosthenes (bitset) to test *
 *            primality of the odd numbers up to 2*n^2 - 1 and then scans the
 *            * rows to count primes; the maximum row count...
 * Range:     u8 n <= 74, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <math.h>
#include <stdint.h>
#include <stdbool.h>

static uint8_t *prime_bits = NULL;   /* bitset of prime flags  */
static idx_t prime_bits_limit = 0;   /* maximal number included in the sieve */

/* bit manipulation helpers ------------------------------------------------ */
static inline void set_bit(uint8_t *b, idx_t i)   { b[i>>3] |=  1u << (i&7); }
static inline void clear_bit(uint8_t *b, idx_t i) { b[i>>3] &= ~(1u << (i&7)); }
static inline bool get_bit(uint8_t *b, idx_t i)   { return (b[i>>3] >> (i&7)) & 1u; }

/* Ensure a sieve exists for all numbers up to 'limit' (inclusive).  */
static void ensure_sieve(idx_t limit)
{
    if (limit <= prime_bits_limit)
        return;                     /* already enough */

    /* free previous sieve */
    if (prime_bits)
        free(prime_bits);

    /* allocate new sieve */
    size_t bytes = (limit >> 3) + 1;
    prime_bits = (uint8_t *)malloc(bytes);
    if (!prime_bits)
        abort();                    /* out of memory */

    /* set all bits to 1 (prime) */
    memset(prime_bits, 0xFF, bytes);
    /* clear 0 and 1 */
    clear_bit(prime_bits, 0);
    clear_bit(prime_bits, 1);

    /* sieve only odd numbers (2 stays set as prime) */
    idx_t sqrt_lim = (idx_t)sqrt((double)limit);
    for (idx_t p = 3; p <= sqrt_lim; p += 2)
    {
        if (!get_bit(prime_bits, p))
            continue;
        /* cross out odd multiples of p */
        for (idx_t j = p * p; j <= limit; j += p << 1)
            clear_bit(prime_bits, j);
    }

    prime_bits_limit = limit;
}

/* ------------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;                   /* defined only for n >= 1 */

    /* All odd numbers up to but not including 2*n^2 are in the grid.          */
    idx_t limit = 2 * n * n;
    ensure_sieve(limit);

    seq_t maxcnt = 0, cnt = 0;

    /* Scan all odd numbers in row order */
    /* The j-th odd number (j = 1 .. n^2) has value 2*j-1 */
    for (idx_t j = 1; j <= n * n; ++j)
    {
        idx_t val = 2 * j - 1;
        if (get_bit(prime_bits, val))
            ++cnt;

        if (j % n == 0)             /* row finished */
        {
            if (cnt > maxcnt)
                maxcnt = cnt;
            cnt = 0;
        }
    }

    return maxcnt;
}
