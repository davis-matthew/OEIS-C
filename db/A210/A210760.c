/*
 * A210760  Primes less than 100000 sorted lexicographically in decimal
 *          representation.
 * https://oeis.org/A210760
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full
 * Formula:   a(n) is the nth prime < 100000 sorted lexicographically by
 *            decimal representation
 * Range:     u8 none, u16 none, u32 n <= 9592, u64 n <= 9592, u128 n <= 9592,
 *            big any n
 * Helpers:   build_primes_upto, digit_len, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

/* Utility functions used */
#include "../utils/build_primes_upto.h"   /* build_primes_upto()         */
#include "../utils/digit_len.h"           /* digit_len()                 */
#include "../utils/pow10.h"               /* pow10()                     */

/* Comparator for sorting primes lexicographically */
static int cmp_lex(const void *p1, const void *p2)
{
    seq_t a = *(const seq_t *)p1;
    seq_t b = *(const seq_t *)p2;

    /* lengths of decimal strings */
    size_t la = digit_len(a);
    size_t lb = digit_len(b);

    size_t min_len = la < lb ? la : lb;
    size_t i;
    for (i = 0; i < min_len; ++i) {
        /* digit at position i from the most significant side */
        seq_t pow = pow10((idx_t)(la - 1 - i));
        seq_t da = (a / pow) % 10;
        pow = pow10((idx_t)(lb - 1 - i));
        seq_t db = (b / pow) % 10;

        if (da < db) return -1;
        if (da > db) return 1;
    }
    /* all common digits equal */
    if (la < lb) return -1;
    if (la > lb) return 1;
    return 0;
}

seq_t a(idx_t n)
{
    /* static cache of lexicographically sorted primes */
    static seq_t *lex_primes = NULL;
    static size_t count = 0;

    if (lex_primes == NULL) {
        uint64_t *primes64;
        size_t pcnt;
        const idx_t LIMIT = 99999; /* all primes < 100000 */

        build_primes_upto(LIMIT, &primes64, &pcnt);

        /* allocate array for seq_t values and copy */
        lex_primes = (seq_t *)malloc(pcnt * sizeof(seq_t));
        count = pcnt;
        for (size_t i = 0; i < pcnt; ++i)
            lex_primes[i] = (seq_t)primes64[i];

        free(primes64); /* free temporary array returned by build_primes_upto */

        /* sort lexicographically */
        qsort(lex_primes, count, sizeof(seq_t), cmp_lex);
    }

    if (n < 1 || (idx_t)n > count)
        return (seq_t)0;   /* undefined for out-of-range, but required to return something */

    return lex_primes[n - 1];
}
