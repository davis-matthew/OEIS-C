/*
 * A039982  Let phi denote the morphism 0 -> 11, 1 -> 10. This sequence is the
 *          limit S(oo) where S(0) = 1; S(n+1) = 1.phi(S(n)).
 * https://oeis.org/A039982
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is the nth bit of the infinite binary sequence * defined by
 *            S(0)=1 and for k≥0, S(k+1) = 1 · φ(S(k)), * where the morphism φ
 *            is 0 → 11 and 1 → 10. * * The implementation stores the
 *            generated bits in a static array * that expands as needed. It
 *            works for any seq_t (uint​8/16/32/64/128 * or an...
 * Range:     u8 n <= 9647, u16 n <= 9647, u32 n <= 9647, u64 n <= 9647,
 *            u128 n <= 9647, big any n
 * Helpers:   alloc_seq_array, seq_free
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include "../utils/alloc_seq_array.h"
#include "../utils/seq_free.h"

seq_t a(idx_t n)
{
    /* static storage for the generated sequence */
    static seq_t *seq = NULL;
    static size_t used = 0;  /* number of bits currently stored */

    /* initialise on first call */
    if (!seq) {
        /* start with a modest capacity; will grow as needed */
        seq = alloc_seq_array(1);
        if (!seq) abort();
        seq[0] = 1;          /* S(0) = 1                         */
        used = 1;
    }

    /* expand until we have at least (n+1) bits            */
    while (used <= n) {
        size_t old_used = used;                /* length of S(k)      */
        size_t new_used = 1 + 2 * old_used;    /* length of S(k+1)    */

        seq_t *next = alloc_seq_array(new_used);
        if (!next) abort();

        next[0] = 1;   /* prepend the leading 1 */

        for (size_t i = 0; i < old_used; ++i) {
            seq_t bit = seq[i];
            /* φ(0)=11, φ(1)=10 */
            next[1 + 2 * i] = 1;
            next[2 + 2 * i] = (bit == 0) ? 1 : 0;
        }

        seq_free(seq);
        seq = next;
        used = new_used;
    }

    return seq[n];
}
