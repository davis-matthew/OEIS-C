/*
 * A158619  Twin prime pairs concatenated in binary representation.
 * https://oeis.org/A158619
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = binary string formed by concatenating the binary
 *            representations of the nth lower and upper twin primes
 * Range:     u8 none, u16 n <= 1, u32 n <= 5, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Helpers:   bitlen_seq_t, is_prime_small, seq_from_str
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/is_prime_small.h"   /* bool is_prime_small(seq_t n); */
#include "../utils/bitlen_seq_t.h"     /* unsigned bitlen_seq_t(seq_t x); */
#include "../utils/seq_from_str.h"     /* seq_t seq_from_str(const char *s); */

/* Return the nth term of OEIS A158619 */
seq_t a(idx_t n) {
    /* static cache of lower twin primes */
    static seq_t *twin_lower = NULL;    /* array holding lower primes of twin pairs */
    static size_t twin_cnt  = 0;         /* number of twins currently stored   */
    static size_t twin_cap  = 0;         /* allocated capacity                 */
    static idx_t  next_candidate = 3;    /* next odd number to test for primality */

    /* Grow the twin list until we have at least n pairs */
    while (twin_cnt < n) {
        /* find next prime */
        while (!is_prime_small(next_candidate))
            next_candidate += 2;

        /* check if the next prime + 2 is also prime */
        if (is_prime_small(next_candidate + 2)) {
            /* grow array if necessary */
            if (twin_cnt == twin_cap) {
                twin_cap = twin_cap ? twin_cap * 2 : 8;
                twin_lower = (seq_t *)realloc(twin_lower,
                                              twin_cap * sizeof(seq_t));
                if (!twin_lower)
                    abort();     /* allocation failed */
            }
            twin_lower[twin_cnt++] = next_candidate;
        }
        next_candidate += 2;   /* advance to the next odd number */
    }

    /* Retrieve the required twin pair */
    seq_t lower = twin_lower[n - 1];
    seq_t upper = lower + 2;

    /* Convert each prime to its binary digit string */
    unsigned lbits = bitlen_seq_t(lower);
    unsigned ubits = bitlen_seq_t(upper);
    size_t pos = 0;
    static char buf[256];          /* plenty of space for our expected sizes   */

    /* lower bits (most significant first) */
    for (unsigned i = 0; i < lbits; ++i) {
        buf[pos++] = ((lower >> (lbits - 1 - i)) & 1) ? '1' : '0';
    }

    /* upper bits */
    for (unsigned i = 0; i < ubits; ++i) {
        buf[pos++] = ((upper >> (ubits - 1 - i)) & 1) ? '1' : '0';
    }
    buf[pos] = '\0';   /* terminate the string */

    /* Interpret the binary string as a decimal integer value */
    return seq_from_str(buf);
}
