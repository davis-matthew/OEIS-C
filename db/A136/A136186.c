/*
 * A136186  Primes whose decimal and binary reversal are both prime.
 * https://oeis.org/A136186
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 236, u32 n <= 236, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big, rev_decimal
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A136186 – Primes whose decimal and binary reversals are both prime.
 *  The function finds successive primes and retains those for which
 *  both rev_decimal(p) and reverse_binary(p) are prime.  The nth such
 *  prime (n ≥ 1) is returned.  Results are cached for repeated calls.
 */
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/rev_decimal.h"
#include "../utils/is_prime_big.h"

/* reverse the binary representation of n */
static seq_t reverse_binary(seq_t n)
{
    seq_t rev = 0;
    while (n != 0) {
        rev = (rev << 1) | (n & 1);
        n >>= 1;
    }
    return rev;
}

static seq_t *cache = NULL;          /* stored qualifying primes */
static size_t cap   = 0;              /* allocated capacity of cache */
static size_t len   = 0;              /* current number of stored primes */
static seq_t last_checked = 1;       /* last prime examined */

seq_t a(idx_t n)
{
    size_t needed = (size_t)n;

    /* generate terms until we have at least 'needed' of them */
    while (len < needed) {
        /* find next prime after last_checked */
        do {
            last_checked++;
        } while (!is_prime_big(last_checked));

        seq_t prime = last_checked;

        seq_t rev_dec = rev_decimal(prime);
        if (!is_prime_big(rev_dec))
            continue;

        seq_t rev_bin = reverse_binary(prime);
        if (!is_prime_big(rev_bin))
            continue;

        /* store qualifying prime */
        if (len == cap) {
            size_t newcap = (cap == 0) ? 16 : cap * 2;
            seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
            if (!tmp) abort();        /* allocation failure */
            cache = tmp;
            cap   = newcap;
        }
        cache[len++] = prime;
    }

    return cache[needed - 1];
}
