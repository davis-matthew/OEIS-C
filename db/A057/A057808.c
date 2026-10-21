/*
 * A057808  Number of ways of getting (at least) a straight flush, 4 of a
 *          kind, full house, flush, straight, 3 of a kind, 2 pair, a pair in
 *          poker.
 * https://oeis.org/A057808
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, nonn, full, nice
 * Formula:   number of ways of getting at least a straight flush, 4 of a
 *            kind, full house, flush, straight, 3 of a kind, 2 pair, a pair
 *            in poker. The sequence satisfies a(1)=40, a(2)=624, a(3)=3744,
 *            a(4)=5148, a(5)=10240, a(6)=54280, a(7)=127920, a(8)=1281072.
 *            Values for n>8 are not defined in OEIS and are re...
 * Range:     u8 n <= 1, u16 n <= 6, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n) {
    static const seq_t table[] = {
        40ULL,        /* a(1) */
        624ULL,       /* a(2) */
        3744ULL,      /* a(3) */
        5148ULL,      /* a(4) */
        10240ULL,     /* a(5) */
        54280ULL,     /* a(6) */
        127920ULL,    /* a(7) */
        1281072ULL    /* a(8) */
    };
    if (n == 0) return 0;
    if (n <= (idx_t)(sizeof(table)/sizeof(table[0]))) {
        return table[n - 1];
    }
    return 0;
}
