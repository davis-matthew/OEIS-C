/*
 * A142584  a(n) = A014217(n+1) - A115360(n+2).
 * https://oeis.org/A142584
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Lucas(n+1) - (1/2)*(1 + (-1)^((n+2) mod 3))
 * Range:     u8 n <= 10, u16 n <= 22, u32 n <= 45, u64 n <= 91, u128 n <= 183,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* static cache for Lucas numbers */
    static seq_t *lucas = NULL;
    static size_t lucas_cap = 0;
    static idx_t lucas_len = 0; /* number of Lucas numbers stored (indices 0..lucas_len-1) */

    /* initialize cache */
    if (lucas == NULL) {
        lucas_cap = 4; /* start with capacity 4, will grow as needed */
        lucas = (seq_t *)malloc(lucas_cap * sizeof(seq_t));
        if (!lucas) return (seq_t)0; /* out of memory; caller must handle */
        lucas[0] = (seq_t)2; /* L0 */
        lucas[1] = (seq_t)1; /* L1 */
        lucas_len = 2;
    }

    /* compute Lucas up to index n+1 if necessary */
    idx_t target = n + 1;
    while (lucas_len <= target) {
        if (lucas_len == lucas_cap) {
            /* grow cache */
            size_t new_cap = lucas_cap * 2;
            seq_t *p = (seq_t *)realloc(lucas, new_cap * sizeof(seq_t));
            if (!p) return (seq_t)0; /* out of memory */
            lucas = p;
            lucas_cap = new_cap;
        }
        lucas[lucas_len] = lucas[lucas_len - 1] + lucas[lucas_len - 2];
        ++lucas_len;
    }

    /* Lucas(n+1) */
    seq_t lucas_val = lucas[target];

    /* adjustment: 1 unless n % 3 == 2 */
    idx_t rem = n % (idx_t)3;
    seq_t adjustment = (rem == 2) ? (seq_t)0 : (seq_t)1;

    return lucas_val - adjustment;
}
